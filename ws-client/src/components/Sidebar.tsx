import { Link, useLocation } from "react-router-dom";

const navLinks = [
  { to: "/", label: "Inicio" },
  { to: "/websocket", label: "WebSocket" },
  { to: "/chart", label: "Gráfica" },
  { to: "/edit-params", label: "Editar parámetros" },
  { to: "/control", label: "Control" },
  { to: "/params-view", label: "Vista de parámetros" },
  { to: "/results", label: "Resultados" },
  { to: "/about", label: "Acerca de" },
];

export default function Sidebar() {
  const location = useLocation();
  return (
    <aside
      className="fixed left-0 h-full w-48 bg-gray-800 shadow z-40 flex flex-col"
      style={{ top: "72px" }}
    >
      <nav className="flex flex-col gap-2 px-4 mt-6">
        {navLinks.map((link) => (
          <Link
            key={link.to}
            to={link.to}
            className={`py-2 px-3 rounded font-medium transition-colors text-gray-200 hover:bg-gray-700 hover:text-blue-400 ${
              location.pathname === link.to ? "bg-gray-700 text-blue-400" : ""
            }`}
          >
            {link.label}
          </Link>
        ))}
      </nav>
    </aside>
  );
}
