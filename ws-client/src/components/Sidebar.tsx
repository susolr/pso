import { Link, useLocation } from "react-router-dom";

const navLinks = [
  { to: "/", label: "Home" },
  { to: "/websocket", label: "WebSocket" },
  { to: "/chart", label: "Charts" },
  { to: "/edit-params", label: "Edit Parameters" },
  { to: "/control", label: "Control" },
  { to: "/params-view", label: "Parameters View" },
  { to: "/results", label: "Results" },
  { to: "/about", label: "About" },
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
