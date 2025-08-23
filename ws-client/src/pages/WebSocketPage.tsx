import Layout from "../components/Layout";
import MessageList from "../components/MessageList";
import { useWebSocketContext } from "../context/WebSocketContext";

export default function WebSocketPage() {
  const { messages } = useWebSocketContext();
  return (
    <Layout>
      <div className="max-w-3xl mx-auto space-y-6">
        <h2 className="text-3xl font-bold mb-4">📡 WebSocket Messages</h2>
        <MessageList messages={messages} />
      </div>
    </Layout>
  );
}
