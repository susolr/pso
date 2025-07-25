import React, { createContext, useContext, useRef, useState } from "react";

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

interface WebSocketContextType {
  wsUrl: string;
  setWsUrl: (url: string) => void;
  status: ConnectionStatus;
  messages: Message[];
  connect: () => void;
  disconnect: () => void;
}

const WebSocketContext = createContext<WebSocketContextType | undefined>(
  undefined
);

export const useWebSocketContext = () => {
  const ctx = useContext(WebSocketContext);
  if (!ctx)
    throw new Error(
      "useWebSocketContext debe usarse dentro de WebSocketProvider"
    );
  return ctx;
};

export const WebSocketProvider: React.FC<{ children: React.ReactNode }> = ({
  children,
}) => {
  const [wsUrl, setWsUrl] = useState("ws://localhost:9999");
  const [status, setStatus] = useState<ConnectionStatus>("disconnected");
  const [messages, setMessages] = useState<Message[]>([]);
  const wsRef = useRef<WebSocket | null>(null);
  const reconnectIntervalRef = useRef<number | null>(null);

  const connect = () => {
    if (wsRef.current && (status === "connected" || status === "connecting"))
      return;
    setStatus("connecting");
    const ws = new WebSocket(wsUrl);
    ws.onopen = () => {
      setStatus("connected");
      if (reconnectIntervalRef.current)
        clearInterval(reconnectIntervalRef.current);
      reconnectIntervalRef.current = null;
    };
    ws.onmessage = (event: MessageEvent) => {
      try {
        const data: Message = JSON.parse(event.data);
        setMessages((prev) => [...prev, data]);
      } catch {}
    };
    ws.onclose = () => {
      setStatus("disconnected");
      attemptReconnect();
    };
    ws.onerror = () => {
      setStatus("error");
      attemptReconnect();
    };
    wsRef.current = ws;
  };

  const attemptReconnect = () => {
    if (reconnectIntervalRef.current || status === "disconnected") return;
    setStatus("reconnecting");
    reconnectIntervalRef.current = window.setInterval(() => {
      connect();
    }, 3000);
  };

  const disconnect = () => {
    if (wsRef.current) {
      wsRef.current.close();
      wsRef.current = null;
    }
    if (reconnectIntervalRef.current) {
      clearInterval(reconnectIntervalRef.current);
      reconnectIntervalRef.current = null;
    }
    setStatus("disconnected");
  };

  // Elimina el cierre automático del socket al desmontar el provider para mantener la conexión global
  // useEffect(() => {
  //   return () => {
  //     if (wsRef.current) wsRef.current.close();
  //     if (reconnectIntervalRef.current) clearInterval(reconnectIntervalRef.current);
  //   };
  // }, []);

  return (
    <WebSocketContext.Provider
      value={{ wsUrl, setWsUrl, status, messages, connect, disconnect }}
    >
      {children}
    </WebSocketContext.Provider>
  );
};
