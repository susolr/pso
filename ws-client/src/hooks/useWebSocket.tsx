import { useEffect, useRef, useState } from "react";

export type Message = {
  type: string;
  payload: any;
};

export type ConnectionStatus =
  | "disconnected"
  | "connecting"
  | "connected"
  | "reconnecting"
  | "error";

export function useWebSocket(initialUrl: string) {
  const [wsUrl, setWsUrl] = useState(initialUrl);
  const [status, setStatus] = useState<ConnectionStatus>("disconnected");
  const [messages, setMessages] = useState<Message[]>([]);
  const wsRef = useRef<WebSocket | null>(null);
  const reconnectIntervalRef = useRef<number | null>(null);

  const connect = () => {
    if (wsRef.current && (status === "connected" || status === "connecting")) {
      console.warn("🔄 An active connection already exists.");
      return;
    }

    console.log(`🌐 Conectando a ${wsUrl} ...`);
    setStatus("connecting");

    const ws = new WebSocket(wsUrl);

    ws.onopen = () => {
      console.log("✅ Conectado al servidor WebSocket");
      setStatus("connected");
      if (reconnectIntervalRef.current) {
        clearInterval(reconnectIntervalRef.current);
        reconnectIntervalRef.current = null;
      }
    };

    ws.onmessage = (event: MessageEvent) => {
      try {
        const data: Message = JSON.parse(event.data);
        console.log("📥 Mensaje recibido:", data);
        setMessages((prev) => [...prev, data]);
      } catch (err) {
        console.error("❌ Error al parsear mensaje:", err);
      }
    };

    ws.onclose = () => {
      console.log("❌ Connection closed");
      setStatus("disconnected");
      attemptReconnect();
    };

    ws.onerror = (err) => {
      console.error("🚨 Connection error:", err);
      setStatus("error");
      attemptReconnect();
    };

    wsRef.current = ws;
  };

  const disconnect = () => {
    if (wsRef.current) {
      console.log("📴 Desconectando...");
      wsRef.current.close();
      wsRef.current = null;
    }
    if (reconnectIntervalRef.current) {
      clearInterval(reconnectIntervalRef.current);
      reconnectIntervalRef.current = null;
    }
    setStatus("disconnected");
  };

  const attemptReconnect = () => {
    if (reconnectIntervalRef.current || status === "disconnected") return;

    console.log("🔄 Intentando reconectar cada 3s...");
    setStatus("reconnecting");

    reconnectIntervalRef.current = window.setInterval(() => {
      console.log("🔁 Intentando reconectar...");
      connect();
    }, 3000);
  };

  useEffect(() => {
    return () => {
      if (wsRef.current) wsRef.current.close();
      if (reconnectIntervalRef.current)
        clearInterval(reconnectIntervalRef.current);
    };
  }, []);

  return {
    wsUrl,
    setWsUrl,
    status,
    messages,
    connect,
    disconnect,
  };
}
