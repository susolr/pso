import Layout from "../components/Layout";
import { useEffect, useState } from "react";

export default function ParamsViewPage() {
  const [params, setParams] = useState<Record<string, any>>({});
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    fetch("/config.json")
      .then((res) => res.json())
      .then((data) => {
        setParams(data);
        setLoading(false);
      });
  }, []);

  if (loading)
    return (
      <Layout>
        <div>Cargando parámetros...</div>
      </Layout>
    );

  return (
    <Layout>
      <div className="max-w-3xl mx-auto py-8">
        <h2 className="text-3xl font-bold mb-6 text-center">
          Vista de parámetros
        </h2>
        <div className="bg-gray-800 rounded-lg shadow-lg p-6">
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4 bg-gray-900 rounded p-4">
            {Object.entries(params).map(([key, obj]: [string, any]) => (
              <div
                key={key}
                className="flex flex-col gap-1 border-b border-gray-700 pb-2"
              >
                <span className="text-gray-400 text-xs font-mono">{key}</span>
                <span className="font-semibold text-sm text-white">
                  {obj.title}
                </span>
                <span className="text-gray-200">{obj.value}</span>
              </div>
            ))}
          </div>
        </div>
      </div>
    </Layout>
  );
}
