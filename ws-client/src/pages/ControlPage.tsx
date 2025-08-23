import Layout from "../components/Layout";
import { useEffect, useState } from "react";
import { useWebSocketContext } from "../context/WebSocketContext";

function ParamsModal({
  open,
  onClose,
  params,
  onSubmit,
  scriptMode,
  scriptRuns,
  setScriptRuns,
  scriptName,
  setScriptName,
}: any) {
  const [form, setForm] = useState<Record<string, any>>({});

  useEffect(() => {
    if (open) {
      // Initialize form with current values (ALL parameters)
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
          Configure Parameters
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
            <div className="mt-6 space-y-4">
              <div className="flex items-center gap-2">
                <label className="font-semibold text-sm">Runs:</label>
                <input
                  type="number"
                  min={1}
                  value={scriptRuns}
                  onChange={(e) => setScriptRuns(Number(e.target.value))}
                  className="bg-gray-900 border border-gray-700 rounded px-2 py-1 w-20 text-gray-100"
                />
              </div>
              <div className="flex flex-col gap-1">
                <label className="font-semibold text-sm">Script Name:</label>
                <input
                  type="text"
                  value={scriptName}
                  onChange={(e) => setScriptName(e.target.value)}
                  placeholder="Ex: Experiment A - 5 runs"
                  className="bg-gray-900 border border-gray-700 rounded px-2 py-1 text-gray-100"
                />
              </div>
            </div>
          )}
          <div className="flex justify-end gap-4 mt-8">
            <button
              type="button"
              onClick={onClose}
              className="px-4 py-2 bg-gray-600 hover:bg-gray-700 rounded text-white font-semibold"
            >
              Cancel
            </button>
            <button
              type="submit"
              className="px-4 py-2 bg-blue-600 hover:bg-blue-700 rounded text-white font-semibold"
            >
              Accept
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
  const [scriptName, setScriptName] = useState("");
  const [modalOpen, setModalOpen] = useState(false);
  const [scriptMode, setScriptMode] = useState(false);
  const [live, setLive] = useState<boolean>(true);
  const { status, send } = useWebSocketContext();

  useEffect(() => {
    fetch("/config.json")
      .then((res) => res.json())
      .then((data) => {
        setParams(data);
        if (data.WSRealtime && typeof data.WSRealtime.value !== "undefined") {
          setLive(!!Number(data.WSRealtime.value));
        }
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

  const buildParamsPayload = (form: Record<string, any>) => {
    // Mapear a { clave: { value, tag } }
    const payload: Record<string, any> = {};
    Object.entries(params).forEach(([key, obj]: [string, any]) => {
      payload[key] = { value: form[key] ?? obj.value, tag: obj.tag };
    });
    return payload;
  };

  const handleSubmitParams = (form: Record<string, any>) => {
    const payload = buildParamsPayload(form);
    if (scriptMode) {
      send({
        type: "control/scriptRun",
        payload: {
          numRuns: scriptRuns,
          scriptName: scriptName || `Script of ${scriptRuns} runs`, // Default name if empty
          params: payload,
        },
      });
    } else {
      send({ type: "control/start", payload: { params: payload } });
    }
    setModalOpen(false);
  };

  const handlePause = () => send({ type: "control/pause", payload: {} });
  const handleResume = () => send({ type: "control/resume", payload: {} });
  const handleStop = () => send({ type: "control/stop", payload: {} });

  const handleToggleLive = (val: boolean) => {
    setLive(val);
    send({ type: "control/toggle_live", payload: { enabled: val } });
  };

  if (loading)
    return (
      <Layout>
        <div>Loading parameters...</div>
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
        scriptName={scriptName}
        setScriptName={setScriptName}
      />
      <div className="max-w-3xl mx-auto py-8">
        <h2 className="text-3xl font-bold mb-6 text-center">
          Algorithm Control Panel
        </h2>
        <div className="bg-gray-800 rounded-lg shadow-lg p-6 flex flex-col gap-8">
          {/* Connection status and live toggle */}
          <div className="flex items-center justify-between bg-gray-900 p-4 rounded">
            <div>
              WS Status: <span className="font-semibold">{status}</span>
            </div>
            <div className="flex items-center gap-2">
              <label className="text-sm">Live Telemetry</label>
              <input
                type="checkbox"
                checked={live}
                onChange={(e) => handleToggleLive(e.target.checked)}
              />
            </div>
          </div>

          {/* Main controls */}
          <div className="flex flex-col md:flex-row gap-6 justify-center items-center">
            <button
              onClick={() => handleOpenModal(false)}
              className="px-6 py-3 bg-green-600 hover:bg-green-700 rounded text-white font-bold text-lg shadow transition"
            >
              Launch
            </button>
            <button
              onClick={handlePause}
              className="px-6 py-3 bg-green-600 hover:bg-green-700 rounded text-white font-bold text-lg shadow transition"
            >
              Pause
            </button>
            <button
              onClick={handleResume}
              className="px-6 py-3 bg-amber-600 hover:bg-amber-700 rounded text-white font-bold text-lg shadow transition"
            >
              Resume
            </button>
            <button
              onClick={handleStop}
              className="px-6 py-3 bg-red-600 hover:bg-red-700 rounded text-white font-bold text-lg shadow transition"
            >
              Stop
            </button>
          </div>

          {/* Script mode */}
          <div className="flex flex-col md:flex-row gap-4 items-center justify-center bg-gray-900 rounded p-4 mt-4">
            <span className="font-semibold text-lg text-white mr-2">
              Script Mode:
            </span>
            <input
              type="number"
              min={1}
              value={scriptRuns}
              onChange={(e) => setScriptRuns(Number(e.target.value))}
              className="bg-gray-800 border border-gray-700 rounded px-2 py-1 w-24 text-gray-100 text-center mr-2"
            />
            <span className="text-gray-300">runs</span>
            <button
              onClick={() => handleOpenModal(true)}
              className="ml-4 px-4 py-2 bg-blue-600 hover:bg-blue-700 rounded text-white font-semibold"
            >
              Run Script
            </button>
          </div>
        </div>
      </div>
    </Layout>
  );
}
