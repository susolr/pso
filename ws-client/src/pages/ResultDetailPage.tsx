import Layout from "../components/Layout";
import { useEffect, useMemo, useRef, useState } from "react";
import { useParams, useNavigate } from "react-router-dom";
import { Line } from "react-chartjs-2";
import "chart.js/auto";
import html2canvas from "html2canvas";

type TelemetryRow = {
  iteration: number;
  average?: number;
  best?: number;
  executionTime?: number;
  position?: number[];
};

type GraphKey = "classification" | "executionTime" | "position";

export default function ResultDetailPage() {
  const { id } = useParams();
  const navigate = useNavigate();
  const [summary, setSummary] = useState<any>(null);
  const [params, setParams] = useState<any>(null);
  const [rows, setRows] = useState<TelemetryRow[]>([]);
  const [selectedGraph, setSelectedGraph] =
    useState<GraphKey>("classification");
  const chartRef = useRef<any>(null);

  useEffect(() => {
    if (!id) return;
    fetch(`/results/runs/${id}/summary.json`)
      .then((r) => r.json())
      .then(setSummary)
      .catch(() => {});
    fetch(`/results/runs/${id}/params.json`)
      .then((r) => r.json())
      .then(setParams)
      .catch(() => {});
    fetch(`/results/runs/${id}/telemetry.ndjson`)
      .then((r) => r.text())
      .then((text) => {
        const lines = text.split("\n").filter(Boolean);
        const data = lines.map((l) => JSON.parse(l) as TelemetryRow);
        setRows(data);
      })
      .catch(() => {});
  }, [id]);

  const labels = rows.map((r) => r.iteration);
  const average = rows.map((r) => r.average ?? 0);
  const best = rows.map((r) => r.best ?? 0);
  const exec = rows.map((r) => r.executionTime ?? 0);
  const posActive = rows.map((r) =>
    Array.isArray(r.position) ? r.position.reduce((a, b) => a + b, 0) : 0
  );

  const dataByType = useMemo(
    () => ({
      classification: {
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
      },
      executionTime: {
        labels,
        datasets: [
          {
            label: "Tiempo de ejecución (s)",
            data: exec,
            borderColor: "#f59e42",
            backgroundColor: "#f59e4288",
            tension: 0.2,
          },
        ],
      },
      position: {
        labels,
        datasets: [
          {
            label: "Número de features activas",
            data: posActive,
            borderColor: "#a855f7",
            backgroundColor: "#a855f788",
            tension: 0.2,
          },
        ],
      },
    }),
    [
      labels.join(","),
      average.join(","),
      best.join(","),
      exec.join(","),
      posActive.join(","),
    ]
  ) as Record<GraphKey, any>;

  const exportPNG = () => {
    const chart = chartRef.current;
    if (!chart) return;
    const url = chart.toBase64Image();
    const a = document.createElement("a");
    a.href = url;
    a.download = `run_${id}_${selectedGraph}.png`;
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
      a.download = `run_${id}_${selectedGraph}.svg`;
      a.click();
      URL.revokeObjectURL(url);
    } catch (error) {
      console.error("Error al exportar SVG:", error);
    }
  };

  const exportCSV = () => {
    const header = "iteration,average,best,executionTime,numActiveFeatures\n";
    const body = rows
      .map(
        (r) =>
          `${r.iteration},${r.average ?? ""},${r.best ?? ""},${
            r.executionTime ?? ""
          },${
            Array.isArray(r.position)
              ? r.position.reduce((a, b) => a + b, 0)
              : ""
          }`
      )
      .join("\n");
    const blob = new Blob([header + body], { type: "text/csv" });
    const url = URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = `run_${id}_telemetry.csv`;
    a.click();
    URL.revokeObjectURL(url);
  };

  return (
    <Layout>
      <div className="max-w-5xl mx-auto py-8 space-y-6">
        <div className="flex items-center justify-between">
          <button
            onClick={() => navigate("/results")}
            className="flex items-center gap-2 px-4 py-2 bg-gray-700 hover:bg-gray-600 rounded text-white transition-colors"
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
          <h2 className="text-3xl font-bold">Detalle ejecución {id}</h2>
          <div className="w-32"></div> {/* Espaciador para centrar el título */}
        </div>
        {summary && (
          <div className="bg-gray-800 rounded p-4 flex flex-wrap gap-6 justify-center">
            <div>
              Iteraciones:{" "}
              <span className="font-mono">{summary.lastIteration ?? "-"}</span>
            </div>
            <div>
              Average:{" "}
              <span className="font-mono">
                {summary.lastAverage?.toFixed?.(3) ??
                  summary.lastAverage ??
                  "-"}
              </span>
            </div>
            <div>
              Best:{" "}
              <span className="font-mono">
                {summary.lastBest?.toFixed?.(3) ?? summary.lastBest ?? "-"}
              </span>
            </div>
            <div>
              Duración:{" "}
              <span className="font-mono">
                {summary.totalSeconds?.toFixed?.(2) ??
                  summary.totalSeconds ??
                  "-"}{" "}
                s
              </span>
            </div>
          </div>
        )}
        <div className="bg-gray-800 rounded p-4">
          <div className="mb-4 flex items-center justify-between">
            <select
              value={selectedGraph}
              onChange={(e) => setSelectedGraph(e.target.value as GraphKey)}
              className="bg-gray-900 border border-gray-700 rounded px-3 py-2"
            >
              <option value="classification">Clasificación</option>
              <option value="executionTime">Tiempo de ejecución</option>
              <option value="position">Número de features activas</option>
            </select>
            <div className="flex gap-2">
              <button
                onClick={exportPNG}
                className="px-3 py-2 bg-blue-600 hover:bg-blue-700 rounded text-white"
              >
                Exportar PNG
              </button>
              <button
                onClick={exportSVG}
                className="px-3 py-2 bg-purple-600 hover:bg-purple-700 rounded text-white"
              >
                Exportar SVG
              </button>
              <button
                onClick={exportCSV}
                className="px-3 py-2 bg-green-600 hover:bg-green-700 rounded text-white"
              >
                Exportar CSV
              </button>
            </div>
          </div>
          <Line
            ref={chartRef}
            data={dataByType[selectedGraph]}
            options={{
              responsive: true,
              plugins: { legend: { position: "top" } },
            }}
          />
        </div>
        {params && (
          <div className="bg-gray-800 rounded p-4">
            <h3 className="text-xl font-bold mb-2">Parámetros</h3>
            <div className="grid grid-cols-1 md:grid-cols-2 gap-3">
              {Object.entries(params).map(([tag, val]) => (
                <div
                  key={tag}
                  className="flex justify-between border-b border-gray-700 pb-1"
                >
                  <span className="text-gray-300">{tag}</span>
                  <span className="font-mono">{val as any}</span>
                </div>
              ))}
            </div>
          </div>
        )}
      </div>
    </Layout>
  );
}
