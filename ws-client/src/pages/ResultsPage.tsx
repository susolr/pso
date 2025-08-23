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
  scriptId?: string;
  scriptRun?: number;
  totalScriptRuns?: number;
  scriptName?: string;
};

type ScriptGroup = {
  scriptId: string;
  scriptName: string;
  runs: RunIndexItem[];
  totalRuns: number;
  avgBest: number;
  avgAverage: number;
  avgTime: number;
};

export default function ResultsPage() {
  const [runs, setRuns] = useState<RunIndexItem[]>([]);
  const [scriptGroups, setScriptGroups] = useState<ScriptGroup[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    fetch("/results/runs/index.json")
      .then((res) => {
        if (!res.ok) throw new Error("index.json not found");
        return res.json();
      })
      .then((data) => {
        if (Array.isArray(data)) {
          const runsData = data as RunIndexItem[];
          setRuns(runsData);

          // Agrupar por scripts
          const groups = new Map<string, RunIndexItem[]>();
          runsData.forEach((run) => {
            if (run.scriptId) {
              if (!groups.has(run.scriptId)) {
                groups.set(run.scriptId, []);
              }
              groups.get(run.scriptId)!.push(run);
            }
          });

          // Convertir a array de grupos
          const scriptGroupsData: ScriptGroup[] = Array.from(
            groups.entries()
          ).map(([scriptId, runs]) => {
            const avgBest =
              runs.reduce((sum, run) => sum + (run.lastBest || 0), 0) /
              runs.length;
            const avgAverage =
              runs.reduce((sum, run) => sum + (run.lastAverage || 0), 0) /
              runs.length;
            const avgTime =
              runs.reduce((sum, run) => sum + (run.totalSeconds || 0), 0) /
              runs.length;

            return {
              scriptId,
              scriptName: runs[0]?.scriptName || `Script ${scriptId}`,
              runs: runs.sort(
                (a, b) => (a.scriptRun || 0) - (b.scriptRun || 0)
              ),
              totalRuns: runs.length,
              avgBest,
              avgAverage,
              avgTime,
            };
          });

          setScriptGroups(scriptGroupsData);
        }
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
          Historical Results
        </h2>
        {loading && <div>Loading...</div>}
        {error && (
          <div className="text-red-400">Error loading index: {error}</div>
        )}
        {/* Script groups */}
        {scriptGroups.length > 0 && (
          <div className="mb-8">
            <h3 className="text-2xl font-bold mb-4">Script Groups</h3>
            <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
              {scriptGroups.map((group) => (
                <div
                  key={group.scriptId}
                  className="bg-gray-800 rounded p-4 border border-gray-700"
                >
                  <div className="flex justify-between items-start mb-2">
                    <div className="text-lg font-semibold">
                      {group.scriptName}
                    </div>
                    <Link
                      to={`/script/${group.scriptId}`}
                      className="text-xs bg-gray-700 hover:bg-gray-600 px-2 py-1 rounded transition"
                    >
                      View Aggregated Charts
                    </Link>
                  </div>
                  <div className="text-sm text-gray-300 mb-3">
                    {group.totalRuns} runs • {group.avgTime.toFixed(2)}s average
                  </div>
                  <div className="text-sm text-gray-300 mb-3">
                    Best: {group.avgBest.toFixed(2)} • Average:{" "}
                    {group.avgAverage.toFixed(2)}
                  </div>
                  <div className="flex flex-wrap gap-2">
                    {group.runs.map((run) => (
                      <Link
                        key={`${run.scriptId}-${run.scriptRun}`}
                        to={`/results/${run.id}`}
                        className="text-xs bg-gray-700 hover:bg-gray-600 px-2 py-1 rounded transition"
                      >
                        #{run.scriptRun}
                      </Link>
                    ))}
                  </div>
                </div>
              ))}
            </div>
          </div>
        )}

        {/* Individual runs */}
        <div>
          <h3 className="text-2xl font-bold mb-4">
            {scriptGroups.length > 0 ? "Individual Runs" : "All Runs"}
          </h3>
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            {runs
              .filter((run) => !run.scriptId)
              .map((run) => (
                <Link
                  key={run.id}
                  to={`/results/${run.id}`}
                  className="block bg-gray-800 rounded p-4 hover:bg-gray-700 transition border border-gray-700"
                >
                  <div className="text-lg font-semibold">Run {run.id}</div>
                  <div className="text-sm text-gray-300">
                    {run.startedAt || "(no date)"}
                    {run.totalSeconds !== undefined && (
                      <span> • {run.totalSeconds.toFixed(2)} s</span>
                    )}
                  </div>
                  <div className="text-sm text-gray-300 mt-1">
                    best: {run.lastBest ?? "-"} • average:{" "}
                    {run.lastAverage ?? "-"}
                  </div>
                </Link>
              ))}
            {!loading &&
              !error &&
              runs.filter((run) => !run.scriptId).length === 0 &&
              scriptGroups.length === 0 && (
                <div className="text-gray-400">No runs recorded.</div>
              )}
          </div>
        </div>
      </div>
    </Layout>
  );
}
