#pragma once

#include <atomic>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class PSOManager {
   public:
    static PSOManager &getInstance();

    // Arranca una ejecución única con los parámetros ya cargados en Paramlist
    void startSingle();

    // Arranca modo script con N ejecuciones consecutivas
    void startScript(int runs, const std::string& scriptName = "");

    // Señales de control
    void pause();
    void resume();
    void stop();

    // Telemetría: registrar por-iteración de forma segura
    static void appendTelemetry(const nlohmann::json &payload);

   private:
    PSOManager();
    ~PSOManager();

    // Gestores internos
    void runOnce();

    // Gestión de directorios/archivos de resultados
    void openNewRunDirectory();
    void closeRunDirectory();
    void writeParamsSnapshot();
    void writeSummary();
    void updateIndex();

    // Estado de ejecución
    std::atomic<bool> m_running;
    std::atomic<bool> m_stopping;

    // Resultados actuales
    std::string m_currentRunId;
    std::string m_currentRunDir;
    std::string m_startedAt;
    std::string m_finishedAt;
    std::ofstream m_telemetryStream;
    std::mutex m_ioMutex;

    // Agregados para summary rápido
    double m_lastAverage;
    double m_lastBest;
    int m_lastIteration;
    double m_lastTotalSeconds;

    // Script
    int m_totalRuns;
    int m_currentRunIndex;
    std::string m_scriptId;  // ID único para el grupo de script
    std::string m_scriptName; // Nombre descriptivo del script
};
