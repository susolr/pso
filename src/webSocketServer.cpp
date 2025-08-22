/**
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of Atom repository.
 *
 * This work has been funded by:
 *
 * University of Granada under grant number PPJIA-2023-25.\n
 * Spanish 'Ministerio de Ciencia, Innovación y Universidades' under grants number
 * PID2022-137461NB-C32 and PID2020-119478GB-I00.\n Spanish 'Ministerio de Universidades' as part of
 * the program of mobility stays for professors and researchers in foreign higher education and
 * research centers under grant number CAS22/00332.\n European Regional Development Fund (ERDF).
 *
 * @file webSocketServer.h
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief Function declarations of the CSV reader
 * @copyright Atom (c) 2024 University of Granada
 */

#include "webSocketServer.h"
#include "paramlist.h"

#include <iostream>
#include <nlohmann/json.hpp>

WebSocketServer& WebSocketServer::getInstance() {
    static WebSocketServer instance;
    return instance;
}

WebSocketServer::WebSocketServer() : m_running(false) {
    m_server.init_asio();

    // Desactivar logs internos de websocketpp
    m_server.clear_access_channels(websocketpp::log::alevel::all);
    m_server.clear_error_channels(websocketpp::log::elevel::all);

    m_server.set_open_handler([this](connection_hdl hdl) { this->onOpen(hdl); });

    m_server.set_close_handler([this](connection_hdl hdl) { this->onClose(hdl); });

    m_server.set_message_handler([this](connection_hdl hdl, server::message_ptr msg) {
        this->handleMessage(msg->get_payload());
    });
}

WebSocketServer::~WebSocketServer() { stop(); }

void WebSocketServer::start(uint16_t port) {
    if (m_running.load()) return;

    m_running.store(true);
    m_thread = std::thread(&WebSocketServer::run, this, port);
}

void WebSocketServer::run(uint16_t port) {
    try {
        m_server.listen(port);
        m_server.start_accept();
        std::cout << "[WS] Servidor WebSocket iniciado en el puerto " << port << std::endl;
        m_server.run();
    } catch (const std::exception& e) {
        std::cerr << "[WS] Error en servidor WebSocket: " << e.what() << std::endl;
    }
}

void WebSocketServer::broadcast(const std::string& message) {
    std::lock_guard<std::mutex> lock(m_connectionsMutex);

    // Usar un set temporal para guardar handles inválidos
    std::set<connection_hdl, std::owner_less<connection_hdl>> invalid_handles;
    for (const auto& hdl : m_connections) {
        try {
            m_server.send(hdl, message, websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cerr << "[WS] Error enviando mensaje: " << e.what() << std::endl;
            invalid_handles.insert(hdl);
        }
    }
    // Limpiar handles inválidos
    for (const auto& hdl : invalid_handles) {
        m_connections.erase(hdl);
    }
}

void WebSocketServer::stop() {
    if (!m_running.load()) return;

    try {
        m_server.stop_listening();

        {
            std::lock_guard<std::mutex> lock(m_connectionsMutex);
            m_connections.clear();
        }

        m_server.stop();
        if (m_thread.joinable()) {
            m_thread.join();
        }
    } catch (const std::exception& e) {
        std::cerr << "[WS] Error al detener servidor: " << e.what() << std::endl;
    }

    m_running.store(false);
}

void WebSocketServer::onOpen(connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(m_connectionsMutex);
    m_connections.insert(hdl);
    std::cout << "[WS] Cliente conectado (" << m_connections.size() << " activos)" << std::endl;
}

void WebSocketServer::onClose(connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(m_connectionsMutex);
    m_connections.erase(hdl);
    std::cout << "[WS] Cliente desconectado (" << m_connections.size() << " activos)" << std::endl;
}

void WebSocketServer::handleMessage(const std::string& message) {
    try {
        auto jsonMsg = nlohmann::json::parse(message);
        std::string type = jsonMsg.value("type", "");
        if (type == "update") {
            handleUpdate(jsonMsg["payload"].dump());
        } else {
            std::cerr << "[WS] Tipo de mensaje no soportado: " << type << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[WS] Error al procesar mensaje entrante: " << e.what() << std::endl;
    }
}

void WebSocketServer::handleUpdate(const std::string& payload) {
    std::cout << "[WS] Recibido update con payload: " << payload << std::endl;
    try {
        auto jsonPayload = nlohmann::json::parse(payload);
        Paramlist* paramlist = Paramlist::getInstance();
        for (auto it = jsonPayload.begin(); it != jsonPayload.end(); ++it) {
            std::string tag = it.value()["tag"];
            std::string value = it.value()["value"].dump();
            // Eliminar comillas si es string
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.size() - 2);
            }
            std::cout << "[WS] Actualizando " << tag << " a " << value << std::endl;
            paramlist->setValor(tag, value);
        }
    } catch (const std::exception& e) {
        std::cerr << "[WS] Error al procesar update: " << e.what() << std::endl;
    }
}
