type Props = {
  status:
    | "disconnected"
    | "connecting"
    | "connected"
    | "reconnecting"
    | "error";
};

export default function StatusPanel({ status }: Props) {
  const statusColor = {
    connected: "bg-green-600",
    connecting: "bg-yellow-500",
    reconnecting: "bg-orange-500",
    disconnected: "bg-gray-600",
    error: "bg-red-600",
  }[status];

  const statusText = {
    connected: "🟢 Conectado",
    connecting: "🟡 Conectando...",
    reconnecting: "🟠 Reconectando...",
    disconnected: "🔴 Desconectado",
    error: "❌ Error",
  }[status];

  return (
    <div
      className={`flex items-center justify-center w-full h-14 rounded shadow text-white text-lg font-semibold ${statusColor}`}
    >
      {statusText}
    </div>
  );
}
