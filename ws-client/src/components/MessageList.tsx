type Message = {
  type: string;
  payload: any;
};

type Props = {
  messages: Message[];
};

export default function MessageList({ messages }: Props) {
  return (
    <div>
      <h3 className="text-xl font-semibold mb-2">📨 Mensajes recibidos</h3>
      <div className="space-y-2 max-h-96 overflow-y-auto border border-gray-700 rounded p-2 bg-gray-800">
        {messages.map((msg, idx) => (
          <div
            key={idx}
            className="p-3 rounded bg-gray-700 shadow border border-gray-600"
          >
            <p className="font-semibold">Tipo: {msg.type}</p>
            <pre className="text-sm text-gray-300">
              {JSON.stringify(msg.payload, null, 2)}
            </pre>
          </div>
        ))}
        {messages.length === 0 && (
          <p className="text-gray-400 italic">No hay mensajes aún</p>
        )}
      </div>
    </div>
  );
}
