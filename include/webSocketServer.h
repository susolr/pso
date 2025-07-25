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

#pragma once

#include <atomic>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::connection_hdl;

class WebSocketServer {
   public:
    static WebSocketServer& getInstance();

    // Inicia el servidor en un hilo aparte
    void start(uint16_t port = 8080);

    // Envía datos a todos los clientes conectados
    void broadcast(const std::string& message);

    // Detiene el servidor
    void stop();

   private:
    WebSocketServer();
    ~WebSocketServer();

    // No copiar ni mover
    WebSocketServer(const WebSocketServer&) = delete;
    WebSocketServer& operator=(const WebSocketServer&) = delete;

    void run(uint16_t port);

    // Callbacks
    void onOpen(connection_hdl hdl);
    void onClose(connection_hdl hdl);

    // Nuevo: manejar mensajes entrantes
    void handleMessage(const std::string& message);
    void handleUpdate(const std::string& payload);

    server m_server;
    std::set<connection_hdl, std::owner_less<connection_hdl>> m_connections;
    std::mutex m_connectionsMutex;

    std::thread m_thread;
    std::atomic<bool> m_running;
};
