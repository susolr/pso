import WebSocketStatusBar from "./WebSocketStatusBar";

export default function Header() {
  return (
    <header className="fixed top-0 left-0 right-0 bg-gray-800 shadow z-50">
      <div className="flex items-center gap-6 px-4 py-2">
        <h1 className="header-title text-white" style={{ marginLeft: 0 }}>
          <span style={{ fontSize: "1.2em", marginRight: "0.2em" }}>🚀</span>
          <span>My WebSocket App</span>
        </h1>
        <div className="flex-1 flex justify-center">
          <WebSocketStatusBar />
        </div>
      </div>
    </header>
  );
}
