import Layout from "../components/Layout";
import { useEffect, useState } from "react";
import { Link } from "react-router-dom";

type RunIndexItem = {
  id: string;
  startedAt?: string;
  finishedAt?: string;
  totalSeconds?: number;
  lastBest?: number;
  lastAverage?: number;
};

export default function ResultsPage() {
  const [runs, setRuns] = useState<RunIndexItem[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    fetch("/results/runs/index.json")
      .then((res) => {
        if (!res.ok) throw new Error("No se encontró index.json");
        return res.json();
      })
      .then((data) => {
        if (Array.isArray(data)) setRuns(data as RunIndexItem[]);
        setLoading(false);
      })
      .catch((e) => {
        setError(e.message);
        setLoading(false);
      });
  }, []);

  return (
    <Layout>
      <div className="max-w-4xl mx-auto py-8">
        <h2 className="text-3xl font-bold mb-6 text-center">
          Resultados históricos
        </h2>
        {loading && <div>Cargando...</div>}
        {error && (
          <div className="text-red-400">Error cargando índice: {error}</div>
        )}
        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
          {runs.map((run) => (
            <Link
              key={run.id}
              to={`/results/${run.id}`}
              className="block bg-gray-800 rounded p-4 hover:bg-gray-700 transition border border-gray-700"
            >
              <div className="text-lg font-semibold">Ejecución {run.id}</div>
              <div className="text-sm text-gray-300">
                {run.startedAt || "(sin fecha)"}
                {run.totalSeconds !== undefined && (
                  <span> • {run.totalSeconds.toFixed(2)} s</span>
                )}
              </div>
              <div className="text-sm text-gray-300 mt-1">
                best: {run.lastBest ?? "-"} • average: {run.lastAverage ?? "-"}
              </div>
            </Link>
          ))}
          {!loading && !error && runs.length === 0 && (
            <div className="text-gray-400">No hay ejecuciones registradas.</div>
          )}
        </div>
      </div>
    </Layout>
  );
}
