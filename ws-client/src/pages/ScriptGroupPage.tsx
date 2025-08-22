import Layout from "../components/Layout";
import { useEffect, useMemo, useRef, useState } from "react";
import { useParams, useNavigate } from "react-router-dom";
import { Line } from "react-chartjs-2";
import "chart.js/auto";

type TelemetryRow = {
  iteration: number;
  average?: number;
  best?: number;
  executionTime?: number;
  position?: number[];
};

type ScriptRun = {
  id: string;
  scriptRun: number;
  telemetry: TelemetryRow[];
  totalSeconds?: number;
  lastBest?: number;
  lastAverage?: number;
};

export default function ScriptGroupPage() {
  const { scriptId } = useParams();
  const navigate = useNavigate();
  const [scriptRuns, setScriptRuns] = useState<ScriptRun[]>([]);
  const [scriptName, setScriptName] = useState("");
  const [loading, setLoading] = useState(true);
  const [selectedGraph, setSelectedGraph] = useState<
    "classification" | "executionTime" | "position"
  >("classification");
  const chartRef = useRef<any>(null);

  useEffect(() => {
    if (!scriptId) return;

    // Cargar el índice para obtener información del script
    fetch("/results/runs/index.json")
      .then((res) => res.json())
      .then((indexData) => {
        const scriptRunsData = indexData.filter(
          (run: any) => run.scriptId === scriptId
        );
        if (scriptRunsData.length === 0) {
          throw new Error("Script no encontrado");
        }

        setScriptName(scriptRunsData[0].scriptName || `Script ${scriptId}`);

        // Cargar telemetría de cada ejecución
        const promises = scriptRunsData.map((run: any) =>
          fetch(`/results/runs/${run.id}/telemetry.ndjson`)
            .then((res) => res.text())
            .then((text) => {
              const lines = text.split("\n").filter(Boolean);
              const telemetry = lines.map((l) => JSON.parse(l) as TelemetryRow);
              return {
                id: run.id,
                scriptRun: run.scriptRun,
                telemetry,
                totalSeconds: run.totalSeconds,
                lastBest: run.lastBest,
                lastAverage: run.lastAverage,
              };
            })
        );

        return Promise.all(promises);
      })
      .then((runsData) => {
        setScriptRuns(runsData.sort((a, b) => a.scriptRun - b.scriptRun));
        setLoading(false);
      })
      .catch((error) => {
        console.error("Error cargando script:", error);
        setLoading(false);
      });
  }, [scriptId]);

  // Calcular datos agregados
  const aggregatedData = useMemo(() => {
    if (scriptRuns.length === 0) return null;

    const maxIterations = Math.max(
      ...scriptRuns.map((run) => run.telemetry.length)
    );
    const iterations = Array.from({ length: maxIterations }, (_, i) => i);

    // Calcular promedios por iteración
    const avgBest = iterations.map((iteration) => {
      const values = scriptRuns
        .map((run) => run.telemetry[iteration]?.best)
        .filter((val) => val !== undefined);
      return values.length > 0
        ? values.reduce((a, b) => a + b, 0) / values.length
        : 0;
    });

    const avgAverage = iterations.map((iteration) => {
      const values = scriptRuns
        .map((run) => run.telemetry[iteration]?.average)
        .filter((val) => val !== undefined);
      return values.length > 0
        ? values.reduce((a, b) => a + b, 0) / values.length
        : 0;
    });

    const avgExecutionTime = iterations.map((iteration) => {
      const values = scriptRuns
        .map((run) => run.telemetry[iteration]?.executionTime)
        .filter((val) => val !== undefined);
      return values.length > 0
        ? values.reduce((a, b) => a + b, 0) / values.length
        : 0;
    });

    const avgPosition = iterations.map((iteration) => {
      const values = scriptRuns
        .map((run) => {
          const pos = run.telemetry[iteration]?.position;
          return Array.isArray(pos) ? pos.reduce((a, b) => a + b, 0) : 0;
        })
        .filter((val) => val !== undefined);
      return values.length > 0
        ? values.reduce((a, b) => a + b, 0) / values.length
        : 0;
    });

    return {
      iterations,
      avgBest,
      avgAverage,
      avgExecutionTime,
      avgPosition,
    };
  }, [scriptRuns]);

  const chartData = useMemo(() => {
    if (!aggregatedData) return null;

    const dataByType = {
      classification: {
        labels: aggregatedData.iterations,
        datasets: [
          {
            label: "Average (promedio)",
            data: aggregatedData.avgAverage,
            borderColor: "#3b82f6",
            backgroundColor: "#3b82f688",
            tension: 0.2,
          },
          {
            label: "Best (promedio)",
            data: aggregatedData.avgBest,
            borderColor: "#22d3ee",
            backgroundColor: "#22d3ee88",
            tension: 0.2,
          },
        ],
      },
      executionTime: {
        labels: aggregatedData.iterations,
        datasets: [
          {
            label: "Tiempo de ejecución (promedio)",
            data: aggregatedData.avgExecutionTime,
            borderColor: "#f59e42",
            backgroundColor: "#f59e4288",
            tension: 0.2,
          },
        ],
      },
      position: {
        labels: aggregatedData.iterations,
        datasets: [
          {
            label: "Número de features activas (promedio)",
            data: aggregatedData.avgPosition,
            borderColor: "#a855f7",
            backgroundColor: "#a855f788",
            tension: 0.2,
          },
        ],
      },
    };

    return dataByType[selectedGraph];
  }, [aggregatedData, selectedGraph]);

  const exportPNG = () => {
    const chart = chartRef.current;
    if (!chart) return;
    const url = chart.toBase64Image();
    const a = document.createElement("a");
    a.href = url;
    a.download = `script_${scriptId}_${selectedGraph}.png`;
    a.click();
  };

  const exportSVG = () => {
    const chart = chartRef.current;
    if (!chart) return;

    try {
      // Obtener el canvas de Chart.js directamente
      const canvas = chart.canvas;
      const width = canvas.width;
      const height = canvas.height;

      // Crear un canvas temporal para procesar la imagen
      const tempCanvas = document.createElement("canvas");
      tempCanvas.width = width;
      tempCanvas.height = height;
      const tempCtx = tempCanvas.getContext("2d");

      if (!tempCtx) {
        throw new Error("No se pudo crear el contexto del canvas temporal");
      }

      // Dibujar el canvas original en el temporal
      tempCtx.drawImage(canvas, 0, 0);

      // Crear un SVG que contenga la imagen del canvas como base64
      const svgContent = `
        <svg width="${width}" height="${height}" xmlns="http://www.w3.org/2000/svg">
          <defs>
            <style>
              text { font-family: Arial, sans-serif; }
            </style>
          </defs>
          <image 
            width="${width}" 
            height="${height}" 
            href="${tempCanvas.toDataURL("image/png")}"
            preserveAspectRatio="xMidYMid meet"
          />
        </svg>
      `;

      // Crear blob y descargar
      const blob = new Blob([svgContent], { type: "image/svg+xml" });
      const url = URL.createObjectURL(blob);
      const a = document.createElement("a");
      a.href = url;
      a.download = `script_${scriptId}_${selectedGraph}.svg`;
      a.click();
      URL.revokeObjectURL(url);
    } catch (error) {
      console.error("Error al exportar SVG:", error);
    }
  };

  if (loading) {
    return (
      <Layout>
        <div className="max-w-5xl mx-auto py-8">
          <div>Cargando datos del script...</div>
        </div>
      </Layout>
    );
  }

  if (!aggregatedData) {
    return (
      <Layout>
        <div className="max-w-5xl mx-auto py-8">
          <div className="text-red-400">
            Error: No se pudieron cargar los datos del script
          </div>
        </div>
      </Layout>
    );
  }

  return (
    <Layout>
      <div className="max-w-5xl mx-auto py-8 space-y-6">
        <div className="flex items-center justify-between">
          <button
            onClick={() => navigate("/results")}
            className="flex items-center gap-2 px-4 py-2 bg-gray-600 hover:bg-gray-700 rounded text-white font-semibold transition"
          >
            <svg
              className="w-4 h-4"
              fill="none"
              stroke="currentColor"
              viewBox="0 0 24 24"
            >
              <path
                strokeLinecap="round"
                strokeLinejoin="round"
                strokeWidth={2}
                d="M15 19l-7-7 7-7"
              />
            </svg>
            Volver a resultados
          </button>
          <h2 className="text-3xl font-bold">
            Gráficas Agregadas: {scriptName}
          </h2>
          <div className="w-32"></div>
        </div>

        {/* Información del script */}
        <div className="bg-gray-800 rounded-lg p-6 border border-gray-700">
          <h3 className="text-xl font-semibold mb-4">Información del Script</h3>
          <div className="grid grid-cols-2 md:grid-cols-4 gap-4 text-sm">
            <div>
              <div className="text-gray-400">Número de ejecuciones:</div>
              <div className="font-semibold">{scriptRuns.length}</div>
            </div>
            <div>
              <div className="text-gray-400">Tiempo total:</div>
              <div className="font-semibold">
                {scriptRuns
                  .reduce((sum, run) => sum + (run.totalSeconds || 0), 0)
                  .toFixed(2)}
                s
              </div>
            </div>
            <div>
              <div className="text-gray-400">Tiempo promedio:</div>
              <div className="font-semibold">
                {(
                  scriptRuns.reduce(
                    (sum, run) => sum + (run.totalSeconds || 0),
                    0
                  ) / scriptRuns.length
                ).toFixed(2)}
                s
              </div>
            </div>
            <div>
              <div className="text-gray-400">Mejor resultado:</div>
              <div className="font-semibold">
                {Math.max(
                  ...scriptRuns.map((run) => run.lastBest || 0)
                ).toFixed(2)}
                %
              </div>
            </div>
          </div>
        </div>
        <div className="bg-gray-800 rounded-lg p-6">
          <div className="grid grid-cols-2 md:grid-cols-4 gap-4 text-center">
            <div>
              <div className="text-2xl font-bold text-blue-400">
                {scriptRuns.length}
              </div>
              <div className="text-sm text-gray-300">Ejecuciones</div>
            </div>
            <div>
              <div className="text-2xl font-bold text-green-400">
                {aggregatedData.avgBest[
                  aggregatedData.avgBest.length - 1
                ]?.toFixed(2) || "0"}
              </div>
              <div className="text-sm text-gray-300">Best Final (promedio)</div>
            </div>
            <div>
              <div className="text-2xl font-bold text-yellow-400">
                {aggregatedData.avgAverage[
                  aggregatedData.avgAverage.length - 1
                ]?.toFixed(2) || "0"}
              </div>
              <div className="text-sm text-gray-300">
                Average Final (promedio)
              </div>
            </div>
            <div>
              <div className="text-2xl font-bold text-purple-400">
                {aggregatedData.avgExecutionTime
                  .reduce((a, b) => a + b, 0)
                  .toFixed(2)}
              </div>
              <div className="text-sm text-gray-300">
                Suma de tiempos por iteracion (promedio)
              </div>
            </div>
          </div>
        </div>

        {/* Selector de gráfica */}
        <div className="flex items-center gap-4">
          <label className="font-semibold">Gráfica:</label>
          <select
            value={selectedGraph}
            onChange={(e) => setSelectedGraph(e.target.value as any)}
            className="bg-gray-700 border border-gray-600 rounded px-3 py-1 text-white"
          >
            <option value="classification">Clasificación (promedio)</option>
            <option value="executionTime">
              Tiempo de ejecución (promedio)
            </option>
            <option value="position">Features activas (promedio)</option>
          </select>
        </div>

        {/* Gráfica */}
        <div className="bg-gray-800 rounded-lg p-6">
          <div className="flex justify-between items-center mb-4">
            <h3 className="text-xl font-semibold">
              {selectedGraph === "classification" && "Clasificación Promedio"}
              {selectedGraph === "executionTime" &&
                "Tiempo de Ejecución Promedio"}
              {selectedGraph === "position" && "Features Activas Promedio"}
            </h3>
            <div className="flex gap-2">
              <button
                onClick={exportPNG}
                className="px-3 py-1 bg-blue-600 hover:bg-blue-700 rounded text-white text-sm"
              >
                Exportar PNG
              </button>
              <button
                onClick={exportSVG}
                className="px-3 py-1 bg-green-600 hover:bg-green-700 rounded text-white text-sm"
              >
                Exportar SVG
              </button>
            </div>
          </div>
          <div className="h-96">
            {chartData && (
              <Line
                ref={chartRef}
                data={chartData}
                options={{
                  responsive: true,
                  maintainAspectRatio: false,
                  plugins: {
                    legend: {
                      position: "top" as const,
                      labels: {
                        color: "white",
                      },
                    },
                  },
                  scales: {
                    x: {
                      title: {
                        display: true,
                        text: "Iteración",
                        color: "white",
                      },
                      ticks: {
                        color: "white",
                      },
                      grid: {
                        color: "rgba(255, 255, 255, 0.1)",
                      },
                    },
                    y: {
                      title: {
                        display: true,
                        text:
                          selectedGraph === "classification"
                            ? "Porcentaje"
                            : selectedGraph === "executionTime"
                            ? "Tiempo (s)"
                            : "Features activas",
                        color: "white",
                      },
                      ticks: {
                        color: "white",
                      },
                      grid: {
                        color: "rgba(255, 255, 255, 0.1)",
                      },
                    },
                  },
                }}
              />
            )}
          </div>
        </div>

        {/* Lista de ejecuciones individuales */}
        <div className="bg-gray-800 rounded-lg p-6">
          <h3 className="text-xl font-semibold mb-4">
            Ejecuciones Individuales
          </h3>
          <div className="grid grid-cols-2 md:grid-cols-4 gap-4">
            {scriptRuns.map((run) => (
              <div
                key={run.id}
                className="bg-gray-700 rounded p-3 text-center hover:bg-gray-600 transition cursor-pointer"
                onClick={() => navigate(`/results/${run.id}`)}
              >
                <div className="text-lg font-bold text-blue-400">
                  #{run.scriptRun}
                </div>
                <div className="text-sm text-gray-300">
                  {run.telemetry.length} iteraciones
                </div>
                <div className="text-xs text-gray-400 mt-1">
                  Best:{" "}
                  {run.telemetry[run.telemetry.length - 1]?.best?.toFixed(2) ||
                    "0"}
                </div>
              </div>
            ))}
          </div>
        </div>
      </div>
    </Layout>
  );
}
