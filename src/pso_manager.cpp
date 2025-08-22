#include "pso_manager.h"

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>

#include "paramlist.h"
#include "pso.h"
#include "webSocketServer.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

PSOManager &PSOManager::getInstance() {
    static PSOManager instance;
    return instance;
}

PSOManager::PSOManager() : m_running(false), m_stopping(false), m_lastAverage(0.0), m_lastBest(0.0), m_lastIteration(0), m_lastTotalSeconds(0.0), m_totalRuns(0), m_currentRunIndex(0), m_scriptId(""), m_scriptName("") {}
PSOManager::~PSOManager() { 
    // No cerrar el directorio en el destructor para evitar duplicados
    // closeRunDirectory(); 
}

static std::string nowTimestamp() {
    auto t = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(t);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d-%H%M%S");
    return oss.str();
}

void PSOManager::openNewRunDirectory() {
    if (!m_scriptId.empty()) {
        // Para scripts: scriptId_NumeroEjecucion
        m_currentRunId = m_scriptId + "_" + std::to_string(m_currentRunIndex + 1);
    } else {
        // Para ejecuciones individuales: timestamp
        m_currentRunId = nowTimestamp();
    }
    
    m_currentRunDir = std::string("results/runs/") + m_currentRunId;
    fs::create_directories(m_currentRunDir);
    // abrir telemetry
    m_telemetryStream.open(m_currentRunDir + "/telemetry.ndjson", std::ios::out | std::ios::trunc);
    m_startedAt = m_currentRunId;
    writeParamsSnapshot();
}

void PSOManager::closeRunDirectory() {
    if (m_telemetryStream.is_open()) m_telemetryStream.close();
    m_finishedAt = nowTimestamp();
    writeSummary();
    updateIndex();
}

void PSOManager::writeParamsSnapshot() {
    auto all = Paramlist::getInstance()->getAllByTag();
    json j;
    for (const auto &kv : all) j[kv.first] = kv.second;
    std::ofstream f(m_currentRunDir + "/params.json");
    f << j.dump(2);
}

void PSOManager::writeSummary() {
    json s = {
        {"id", m_currentRunId},
        {"startedAt", m_startedAt},
        {"finishedAt", m_finishedAt},
        {"lastIteration", m_lastIteration},
        {"lastAverage", m_lastAverage},
        {"lastBest", m_lastBest},
        {"totalSeconds", m_lastTotalSeconds},
    };
    
    // Añadir información de script si es parte de un grupo
    if (!m_scriptId.empty()) {
        s["scriptId"] = m_scriptId;
        s["scriptRun"] = m_currentRunIndex + 1;
        s["totalScriptRuns"] = m_totalRuns;
        if (!m_scriptName.empty()) {
            s["scriptName"] = m_scriptName;
        }
    }
    
    std::ofstream f(m_currentRunDir + "/summary.json");
    f << s.dump(2);
}

void PSOManager::updateIndex() {
    const std::string indexPath = "results/runs/index.json";
    json idx = json::array();
    // Leer si existe
    if (fs::exists(indexPath)) {
        try {
            std::ifstream in(indexPath);
            idx = json::parse(in);
            if (!idx.is_array()) idx = json::array();
        } catch (...) {
            idx = json::array();
        }
    }
    // Añadir entrada de este run
    json entry = {
        {"id", m_currentRunId},
        {"startedAt", m_startedAt},
        {"finishedAt", m_finishedAt},
        {"totalSeconds", m_lastTotalSeconds},
        {"lastBest", m_lastBest},
        {"lastAverage", m_lastAverage}
    };
    
    // Añadir información de script si es parte de un grupo
    if (!m_scriptId.empty()) {
        entry["scriptId"] = m_scriptId;
        entry["scriptRun"] = m_currentRunIndex + 1;
        entry["totalScriptRuns"] = m_totalRuns;
        if (!m_scriptName.empty()) {
            entry["scriptName"] = m_scriptName;
        }
    }
    
    idx.push_back(entry);
    // Guardar
    fs::create_directories("results/runs");
    std::ofstream out(indexPath);
    out << idx.dump(2);
}

void PSOManager::appendTelemetry(const json &payload) {
    auto &mgr = getInstance();
    std::lock_guard<std::mutex> lg(mgr.m_ioMutex);
    if (!mgr.m_telemetryStream.is_open()) return;
    mgr.m_lastIteration = payload.value("iteration", mgr.m_lastIteration);
    mgr.m_lastAverage = payload.value("average", mgr.m_lastAverage);
    mgr.m_lastBest = payload.value("best", mgr.m_lastBest);
    json row = payload;
    mgr.m_telemetryStream << row.dump() << "\n";
}

void PSOManager::startSingle() {
    if (m_running.load()) return;
    m_stopping.store(false);
    m_running.store(true);
    m_totalRuns = 1;
    m_currentRunIndex = 0;
    openNewRunDirectory();
    WebSocketServer::getInstance().resetControl();

    runOnce();
    closeRunDirectory();
    m_running.store(false);
}

void PSOManager::startScript(int runs, const std::string& scriptName) {
    if (m_running.load()) return;
    m_stopping.store(false);
    m_running.store(true);
    m_totalRuns = runs;
    m_scriptId = "script_" + nowTimestamp();
    m_scriptName = scriptName.empty() ? "Script de " + std::to_string(runs) + " ejecuciones" : scriptName;
    
    for (m_currentRunIndex = 0; m_currentRunIndex < m_totalRuns; ++m_currentRunIndex) {
        if (m_stopping.load()) break;
        openNewRunDirectory();
        WebSocketServer::getInstance().resetControl();
        runOnce();
        closeRunDirectory();
    }
    m_running.store(false);
}

void PSOManager::runOnce() {
    auto t0 = std::chrono::high_resolution_clock::now();
    PSO p;
    p.crearCumulo();
    p.ejecutar();
    auto t1 = std::chrono::high_resolution_clock::now();
    m_lastTotalSeconds = std::chrono::duration<double>(t1 - t0).count();
}

void PSOManager::pause() { /* integrado en WebSocketServer flags */ }
void PSOManager::resume() {}
void PSOManager::stop() { m_stopping.store(true); }
