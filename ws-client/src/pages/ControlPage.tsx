import Layout from "../components/Layout";
import { useEffect, useState } from "react";

function ParamsModal({
  open,
  onClose,
  params,
  onSubmit,
  scriptMode,
  scriptRuns,
  setScriptRuns,
}: any) {
  const [form, setForm] = useState<Record<string, any>>({});

  useEffect(() => {
    if (open) {
      // Inicializa el formulario con los valores actuales
      const initial: Record<string, any> = {};
      Object.entries(params).forEach(([key, obj]: [string, any]) => {
        initial[key] = obj.value;
      });
      setForm(initial);
    }
  }, [open, params]);

  const handleChange = (key: string, value: string) => {
    setForm((prev) => ({ ...prev, [key]: value }));
  };

  const handleSubmit = (e: any) => {
    e.preventDefault();
    onSubmit(form);
  };

  if (!open) return null;

  return (
    <div className="fixed inset-0 bg-black bg-opacity-40 flex items-center justify-center z-50">
      <div className="bg-gray-800 rounded-lg shadow-lg p-8 w-full max-w-2xl relative">
        <h3 className="text-2xl font-bold mb-4 text-center">
          Configurar parámetros
        </h3>
        <form onSubmit={handleSubmit}>
          <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
            {Object.entries(params).map(([key, obj]: [string, any]) => (
              <div key={key} className="flex flex-col gap-1">
                <label className="font-semibold text-sm" htmlFor={key}>
                  {obj.title}
                </label>
                <input
                  id={key}
                  type="text"
                  value={form[key] ?? ""}
                  onChange={(e) => handleChange(key, e.target.value)}
                  className="bg-gray-900 border border-gray-700 rounded px-2 py-1 text-gray-100"
                />
              </div>
            ))}
          </div>
          {scriptMode && (
            <div className="mt-6 flex items-center gap-2">
              <label className="font-semibold text-sm">Ejecuciones:</label>
              <input
                type="number"
                min={1}
                value={scriptRuns}
                onChange={(e) => setScriptRuns(Number(e.target.value))}
                className="bg-gray-900 border border-gray-700 rounded px-2 py-1 w-20 text-gray-100"
              />
            </div>
          )}
          <div className="flex justify-end gap-4 mt-8">
            <button
              type="button"
              onClick={onClose}
              className="px-4 py-2 bg-gray-600 hover:bg-gray-700 rounded text-white font-semibold"
            >
              Cancelar
            </button>
            <button
              type="submit"
              className="px-4 py-2 bg-blue-600 hover:bg-blue-700 rounded text-white font-semibold"
            >
              Aceptar
            </button>
          </div>
        </form>
      </div>
    </div>
  );
}

export default function ControlPage() {
  const [params, setParams] = useState<Record<string, any>>({});
  const [loading, setLoading] = useState(true);
  const [scriptRuns, setScriptRuns] = useState(1);
  const [modalOpen, setModalOpen] = useState(false);
  const [scriptMode, setScriptMode] = useState(false);

  useEffect(() => {
    fetch("/config.json")
      .then((res) => res.json())
      .then((data) => {
        setParams(data);
        setLoading(false);
      });
  }, []);

  const handleOpenModal = (isScript: boolean) => {
    setScriptMode(isScript);
    setModalOpen(true);
  };

  const handleCloseModal = () => {
    setModalOpen(false);
  };

  const handleSubmitParams = (form: Record<string, any>) => {
    // Aquí se enviaría la configuración al backend
    setModalOpen(false);
  };

  if (loading)
    return (
      <Layout>
        <div>Cargando parámetros...</div>
      </Layout>
    );

  return (
    <Layout>
      <ParamsModal
        open={modalOpen}
        onClose={handleCloseModal}
        params={params}
        onSubmit={handleSubmitParams}
        scriptMode={scriptMode}
        scriptRuns={scriptRuns}
        setScriptRuns={setScriptRuns}
      />
      <div className="max-w-3xl mx-auto py-8">
        <h2 className="text-3xl font-bold mb-6 text-center">
          Cuadro de mando del algoritmo
        </h2>
        <div className="bg-gray-800 rounded-lg shadow-lg p-6 flex flex-col gap-8">
          {/* Controles principales */}
          <div className="flex flex-col md:flex-row gap-6 justify-center items-center">
            <button
              onClick={() => handleOpenModal(false)}
              className="px-6 py-3 bg-green-600 hover:bg-green-700 rounded text-white font-bold text-lg shadow transition"
            >
              Lanzar
            </button>
            <button className="px-6 py-3 bg-yellow-500 hover:bg-yellow-600 rounded text-black font-bold text-lg shadow transition">
              Pausar
            </button>
            <button className="px-6 py-3 bg-red-600 hover:bg-red-700 rounded text-white font-bold text-lg shadow transition">
              Parar
            </button>
          </div>

          {/* Modo script */}
          <div className="flex flex-col md:flex-row gap-4 items-center justify-center bg-gray-900 rounded p-4 mt-4">
            <span className="font-semibold text-lg text-white mr-2">
              Modo script:
            </span>
            <input
              type="number"
              min={1}
              value={scriptRuns}
              onChange={(e) => setScriptRuns(Number(e.target.value))}
              className="bg-gray-800 border border-gray-700 rounded px-2 py-1 w-24 text-gray-100 text-center mr-2"
            />
            <span className="text-gray-300">ejecuciones</span>
            <button
              onClick={() => handleOpenModal(true)}
              className="ml-4 px-4 py-2 bg-blue-600 hover:bg-blue-700 rounded text-white font-semibold"
            >
              Ejecutar script
            </button>
          </div>
        </div>
      </div>
    </Layout>
  );
}
