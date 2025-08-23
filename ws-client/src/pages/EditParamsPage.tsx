import Layout from "../components/Layout";
import { useEffect, useState } from "react";
import { useWebSocketContext } from "../context/WebSocketContext";

export default function EditParamsPage() {
  const [params, setParams] = useState<Record<string, any>>({});
  const [loading, setLoading] = useState(true);
  const { status } = useWebSocketContext();
  const [form, setForm] = useState<Record<string, any>>({});
  const [enviado, setEnviado] = useState(false);

  useEffect(() => {
    fetch("/config.json")
      .then((res) => res.json())
      .then((data) => {
        setParams(data);
        // Inicializar el formulario solo con los valores
        const initialForm: Record<string, any> = {};
        Object.entries(data).forEach(([key, obj]: [string, any]) => {
          initialForm[key] = obj.value;
        });
        setForm(initialForm);
        setLoading(false);
      });
  }, []);

  const handleChange = (key: string, value: string) => {
    setForm((prev) => ({ ...prev, [key]: value }));
  };

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    setEnviado(false);
    // Construir el payload con value y tag
    const payload: Record<string, any> = {};
    Object.entries(params).forEach(([key, obj]: [string, any]) => {
      payload[key] = { value: form[key], tag: obj.tag };
    });
    // Enviar mensaje por websocket
    if (status === "connected") {
      const msg = {
        type: "update",
        payload,
      };
      // @ts-ignore
      window.wsSend && window.wsSend(JSON.stringify(msg));
      setEnviado(true);
    } else {
      alert("No active WebSocket connection");
    }
  };

  // Expose wsSend globally to facilitate sending from context
  useEffect(() => {
    // @ts-ignore
    window.wsSend = (data: string) => {
      // @ts-ignore
      if (window.ws && window.ws.readyState === 1) {
        // @ts-ignore
        window.ws.send(data);
      } else {
        alert("WebSocket is not connected");
      }
    };
  }, []);

  if (loading)
    return (
      <Layout>
        <div>Loading parameters...</div>
      </Layout>
    );

  return (
    <Layout>
      <div className="max-w-2xl mx-auto py-8">
        <h2 className="text-3xl font-bold mb-6">Edit Parameters</h2>
        <form
          onSubmit={handleSubmit}
          className="bg-gray-800 p-6 rounded shadow"
        >
          <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
            {Object.entries(params)
              .filter(([_, obj]: [string, any]) => obj.editable)
              .map(([key, obj]: [string, any]) => (
                <div key={key} className="flex flex-col gap-1">
                  <label className="font-semibold text-sm" htmlFor={key}>
                    {obj.title}
                  </label>
                  <input
                    id={key}
                    type="text"
                    value={form[key]}
                    onChange={(e) => handleChange(key, e.target.value)}
                    className="bg-gray-900 border border-gray-700 rounded px-2 py-1 text-gray-100"
                  />
                </div>
              ))}
          </div>
          <div className="flex justify-end">
            <button
              type="submit"
              className="mt-6 px-4 py-2 bg-blue-600 hover:bg-blue-700 rounded text-white font-semibold"
              disabled={status !== "connected"}
            >
              Send Changes
            </button>
          </div>
          {enviado && (
            <div className="text-green-400 mt-2">Parameters sent!</div>
          )}
        </form>
      </div>
    </Layout>
  );
}
