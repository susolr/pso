import Layout from "../components/Layout";
import { Line } from "react-chartjs-2";
import type { ChartData, ChartOptions } from "chart.js";
import { useWebSocketContext } from "../context/WebSocketContext";
import {
  Chart as ChartJS,
  LineElement,
  PointElement,
  LinearScale,
  Title,
  Tooltip,
  Legend,
  CategoryScale,
} from "chart.js";

// Registro de componentes necesarios para Chart.js v3+
ChartJS.register(
  LineElement,
  PointElement,
  LinearScale,
  Title,
  Tooltip,
  Legend,
  CategoryScale
);

import { useState } from "react";

export default function ChartPage() {
  const { messages } = useWebSocketContext();
  // Filtrar solo mensajes de tipo 'telemetry' y con payload válido
  const telemetry = messages.filter(
    (msg) =>
      msg.type === "telemetry" &&
      msg.payload &&
      typeof msg.payload.iteration === "number"
  );

  const labels = telemetry.map((msg) => msg.payload.iteration);
  const best = telemetry.map((msg) => msg.payload.best);
  const average = telemetry.map((msg) => msg.payload.average);

  // Rango por defecto: X de 0 a max iteración, Y de 0 a 100
  const [xMin, setXMin] = useState(0);
  const [xMax, setXMax] = useState(500);
  const [yMin, setYMin] = useState(0);
  const [yMax, setYMax] = useState(100);

  // Filtrar datos según rango seleccionado
  const filtered = telemetry.filter(
    (msg) => msg.payload.iteration >= xMin && msg.payload.iteration <= xMax
  );
  const filteredLabels = filtered.map((msg) => msg.payload.iteration);
  const filteredBest = filtered.map((msg) => msg.payload.best);
  const filteredAverage = filtered.map((msg) => msg.payload.average);

  const chartData: ChartData<"line"> = {
    labels: filteredLabels,
    datasets: [
      {
        label: "Best (%)",
        data: filteredBest,
        fill: false,
        borderColor: "#3b82f6",
        backgroundColor: "#60a5fa",
        tension: 0.3,
      },
      {
        label: "Average (%)",
        data: filteredAverage,
        fill: false,
        borderColor: "#f59e42",
        backgroundColor: "#fbbf24",
        tension: 0.3,
      },
    ],
  };

  const options: ChartOptions<"line"> = {
    responsive: true,
    plugins: {
      legend: { display: true },
      tooltip: {
        callbacks: {
          label: function (context) {
            return `${context.dataset.label}: ${context.parsed.y.toFixed(2)}%`;
          },
        },
      },
    },
    scales: {
      x: {
        title: { display: true, text: "Iteración" },
        ticks: { color: "#fff" },
        grid: { color: "#444" },
        min: xMin,
        max: xMax,
      },
      y: {
        beginAtZero: true,
        title: { display: true, text: "%" },
        min: yMin,
        max: yMax,
        ticks: {
          callback: (value) => `${value}%`,
          color: "#fff",
        },
        grid: { color: "#444" },
      },
    },
  };

  return (
    <Layout>
      <div className="max-w-7xl mx-auto py-8 px-0 flex flex-row gap-0 items-start">
        {/* Controles verticales a la izquierda, muy estrechos */}
        <div
          className="flex flex-col gap-3 min-w-[90px] max-w-[90px] bg-gray-800 rounded-lg p-2 shadow-md ml-0 self-center"
          style={{ justifyContent: "center", display: "flex" }}
        >
          <label className="block text-xs font-semibold mb-1">
            Iteración mínima
            <input
              type="number"
              className="bg-gray-900 border border-gray-600 rounded px-1 py-1 w-full mt-1 text-xs"
              value={xMin}
              min={labels.length > 0 ? Math.min(...labels) : 0}
              max={xMax}
              onChange={(e) => setXMin(Number(e.target.value))}
            />
          </label>
          <label className="block text-xs font-semibold mb-1">
            Iteración máxima
            <input
              type="number"
              className="bg-gray-900 border border-gray-600 rounded px-1 py-1 w-full mt-1 text-xs"
              value={xMax}
              min={xMin}
              max={labels.length > 0 ? Math.max(...labels) : 0}
              onChange={(e) => setXMax(Number(e.target.value))}
            />
          </label>
          <label className="block text-xs font-semibold mb-1">
            % mínimo
            <input
              type="number"
              className="bg-gray-900 border border-gray-600 rounded px-1 py-1 w-full mt-1 text-xs"
              value={yMin}
              min={0}
              max={yMax}
              onChange={(e) => setYMin(Number(e.target.value))}
            />
          </label>
          <label className="block text-xs font-semibold mb-1">
            % máximo
            <input
              type="number"
              className="bg-gray-900 border border-gray-600 rounded px-1 py-1 w-full mt-1 text-xs"
              value={yMax}
              min={yMin}
              max={100}
              onChange={(e) => setYMax(Number(e.target.value))}
            />
          </label>
        </div>
        {/* Gráfica y título */}
        <div className="flex-1 flex flex-col items-center justify-center">
          <h2 className="text-3xl font-bold mb-6 text-center w-full">
            📈 Gráfica de datos recibidos
          </h2>
          <div
            className="bg-gray-900 rounded-lg p-4 shadow-lg w-full"
            style={{ height: "700px", maxWidth: "100%" }}
          >
            <div style={{ width: "100%", height: "100%" }}>
              <Line data={chartData} options={options} />
            </div>
          </div>
        </div>
      </div>
    </Layout>
  );
}
