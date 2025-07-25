import { useWebSocketContext } from "../context/WebSocketContext";

export default function WebSocketStatusBar() {
  const { wsUrl, setWsUrl, status, connect, disconnect } =
    useWebSocketContext();
  return (
    <div className="flex items-center gap-4 text-sm">
      <span
        className={`px-2 py-1 rounded font-bold ${
          status === "connected"
            ? "bg-green-600 text-white"
            : status === "connecting"
            ? "bg-yellow-500 text-black"
            : status === "reconnecting"
            ? "bg-orange-500 text-white"
            : status === "error"
            ? "bg-red-600 text-white"
            : "bg-gray-600 text-white"
        }`}
      >
        {status}
      </span>
      <div className="flex flex-col gap-1">
        <input
          type="text"
          value={wsUrl}
          onChange={(e) => setWsUrl(e.target.value)}
          className="bg-gray-900 border border-gray-700 rounded px-2 py-1 w-48 text-xs text-gray-100"
          disabled={
            status === "connected" ||
            status === "connecting" ||
            status === "reconnecting"
          }
          title="WebSocket URL"
        />
        <div className="flex gap-2 mt-1">
          <button
            onClick={connect}
            disabled={
              status === "connected" ||
              status === "connecting" ||
              status === "reconnecting"
            }
            className={`px-2 py-1 rounded text-xs font-semibold transition ${
              status === "connected" ||
              status === "connecting" ||
              status === "reconnecting"
                ? "bg-gray-700 cursor-not-allowed text-gray-400"
                : "bg-blue-600 hover:bg-blue-700 text-white"
            }`}
          >
            Conectar
          </button>
          <button
            onClick={disconnect}
            disabled={status === "disconnected"}
            className={`px-2 py-1 rounded text-xs font-semibold transition ${
              status === "disconnected"
                ? "bg-gray-700 cursor-not-allowed text-gray-400"
                : "bg-red-600 hover:bg-red-700 text-white"
            }`}
          >
            Desconectar
          </button>
        </div>
      </div>
    </div>
  );
}
