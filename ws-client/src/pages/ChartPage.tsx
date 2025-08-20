import Layout from "../components/Layout";
import { useEffect, useState } from "react";
import { Line } from "react-chartjs-2";
import "chart.js/auto";
import { useWebSocketContext } from "../context/WebSocketContext";

const GRAPH_TYPES = [
  { key: "classification", label: "Clasificación" },
  { key: "executionTime", label: "Tiempo de ejecución" },
  { key: "position", label: "Vector de posición" },
];

export default function ChartPage() {
  const { messages } = useWebSocketContext();
  const [selectedGraph, setSelectedGraph] = useState("classification");
  const [executionTimes, setExecutionTimes] = useState<number[]>([]);
  const [positions, setPositions] = useState<number[][]>([]);
  const [labels, setLabels] = useState<number[]>([]);
  const [average, setAverage] = useState<number[]>([]);
  const [best, setBest] = useState<number[]>([]);

  useEffect(() => {
    // Filtrar solo mensajes de telemetría
    const telemetry = messages.filter((msg: any) => msg.type === "telemetry");
    setLabels(telemetry.map((msg: any) => msg.payload.iteration));
    setAverage(telemetry.map((msg: any) => msg.payload.average));
    setBest(telemetry.map((msg: any) => msg.payload.best));
    setExecutionTimes(
      telemetry
        .map((msg: any) => msg.payload.executionTime)
        .filter((v: any) => v !== undefined)
    );
    setPositions(
      telemetry
        .map((msg: any) => msg.payload.position)
        .filter((v: any) => v !== undefined)
    );
  }, [messages]);

  // Datos para la gráfica de clasificación (average + best)
  const classificationData = {
    labels,
    datasets: [
      {
        label: "Average",
        data: average,
        borderColor: "#3b82f6",
        backgroundColor: "#3b82f688",
        tension: 0.2,
      },
      {
        label: "Best",
        data: best,
        borderColor: "#22d3ee",
        backgroundColor: "#22d3ee88",
        tension: 0.2,
      },
    ],
  };

  // Datos para la gráfica de tiempo de ejecución
  const executionTimeData = {
    labels,
    datasets: [
      {
        label: "Tiempo de ejecución (s)",
        data: executionTimes,
        borderColor: "#f59e42",
        backgroundColor: "#f59e4288",
        tension: 0.2,
      },
    ],
  };

  // Datos para la gráfica de vector de posición (ejemplo: suma de componentes en cada iteración)
  const positionData = {
    labels,
    datasets: [
      {
        label: "Número de features activas",
        data: positions.map((vec) =>
          Array.isArray(vec) ? vec.reduce((a, b) => a + b, 0) : 0
        ),
        borderColor: "#a855f7",
        backgroundColor: "#a855f788",
        tension: 0.2,
      },
    ],
  };

  let chartData = classificationData;
  let chartTitle = "Evolución de la clasificación";
  if (selectedGraph === "executionTime") {
    chartData = executionTimeData;
    chartTitle = "Evolución del tiempo de ejecución";
  } else if (selectedGraph === "position") {
    chartData = positionData;
    chartTitle = "Evolución del número de features activas";
  }

  // Panel de control contextual
  let controlPanel = null;
  let extraSection = null;
  if (selectedGraph === "classification") {
    const minAvg = Math.min(...average);
    const maxAvg = Math.max(...average);
    const minBest = Math.min(...best);
    const maxBest = Math.max(...best);
    controlPanel = (
      <div className="flex flex-col md:flex-row gap-4 justify-center items-center mb-4 bg-gray-900 rounded p-4">
        <div>
          <span className="font-semibold text-blue-400">Average</span>: min{" "}
          <span className="font-mono">{minAvg.toFixed(3)}</span>, max{" "}
          <span className="font-mono">{maxAvg.toFixed(3)}</span>
        </div>
        <div>
          <span className="font-semibold text-cyan-300">Best</span>: min{" "}
          <span className="font-mono">{minBest.toFixed(3)}</span>, max{" "}
          <span className="font-mono">{maxBest.toFixed(3)}</span>
        </div>
      </div>
    );
  } else if (selectedGraph === "executionTime") {
    const min = executionTimes.length ? Math.min(...executionTimes) : 0;
    const max = executionTimes.length ? Math.max(...executionTimes) : 0;
    const avg = executionTimes.length
      ? executionTimes.reduce((a, b) => a + b, 0) / executionTimes.length
      : 0;
    controlPanel = (
      <div className="flex flex-col md:flex-row gap-4 justify-center items-center mb-4 bg-gray-900 rounded p-4">
        <div>
          Media: <span className="font-mono">{avg.toFixed(3)} s</span>
        </div>
        <div>
          Mínimo: <span className="font-mono">{min.toFixed(3)} s</span>
        </div>
        <div>
          Máximo: <span className="font-mono">{max.toFixed(3)} s</span>
        </div>
      </div>
    );
  } else if (selectedGraph === "position") {
    const sums = positions.map((vec) =>
      Array.isArray(vec) ? vec.reduce((a, b) => a + b, 0) : 0
    );
    const min = sums.length ? Math.min(...sums) : 0;
    const max = sums.length ? Math.max(...sums) : 0;
    const avg = sums.length ? sums.reduce((a, b) => a + b, 0) / sums.length : 0;
    controlPanel = (
      <div className="flex flex-col md:flex-row gap-4 justify-center items-center mb-4 bg-gray-900 rounded p-4">
        <div>
          Número medio de features activas:{" "}
          <span className="font-mono">{avg.toFixed(2)}</span>
        </div>
        <div>
          Mínimo: <span className="font-mono">{Math.floor(min)}</span>
        </div>
        <div>
          Máximo: <span className="font-mono">{Math.floor(max)}</span>
        </div>
      </div>
    );
    // Nueva sección: mostrar features activas por iteración como colapsables
    extraSection = (
      <div className="mt-6 bg-gray-900 rounded p-4">
        <h4 className="text-lg font-bold mb-2 text-center">
          Features activas por iteración
        </h4>
        <div className="space-y-2">
          {positions.map((vec, idx) => {
            const active = Array.isArray(vec)
              ? vec
                  .map((v, i) => (v === 1 ? i : null))
                  .filter((i) => i !== null)
              : [];
            return (
              <details
                key={idx}
                className="bg-gray-800 rounded border border-gray-700"
              >
                <summary className="cursor-pointer px-3 py-2 select-none flex justify-between items-center">
                  <span>
                    Iteración {labels[idx]} — {active.length} features activas
                  </span>
                  <span className="text-gray-400 text-xs">
                    (clic para expandir)
                  </span>
                </summary>
                <div className="px-3 py-3 border-t border-gray-700">
                  {active.length > 0 ? (
                    <div className="font-mono text-sm break-words whitespace-pre-wrap">
                      {active.join(", ")}
                    </div>
                  ) : (
                    <span className="text-gray-400 text-sm">
                      Ninguna feature activa
                    </span>
                  )}
                </div>
              </details>
            );
          })}
        </div>
      </div>
    );
  }

  return (
    <Layout>
      <div className="max-w-3xl mx-auto py-8">
        <h2 className="text-3xl font-bold mb-6 text-center">
          Gráficas en tiempo real
        </h2>
        <div className="mb-6 flex justify-center">
          <select
            value={selectedGraph}
            onChange={(e) => setSelectedGraph(e.target.value)}
            className="bg-gray-800 border border-gray-700 rounded px-4 py-2 text-gray-100 text-lg"
          >
            {GRAPH_TYPES.map((g) => (
              <option key={g.key} value={g.key}>
                {g.label}
              </option>
            ))}
          </select>
        </div>
        {controlPanel}
        <div className="bg-gray-800 rounded-lg shadow-lg p-6">
          <h3 className="text-xl font-bold mb-4 text-center">{chartTitle}</h3>
          <Line
            key={`${selectedGraph}-${labels.length}`}
            data={chartData}
            redraw
            options={{
              responsive: true,
              plugins: { legend: { position: "top" } },
            }}
          />
        </div>
        {extraSection}
      </div>
    </Layout>
  );
}
