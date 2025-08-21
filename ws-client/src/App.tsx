import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import Layout from "./components/Layout";
import HomePage from "./pages/HomePage";
import WebSocketPage from "./pages/WebSocketPage";
import ChartPage from "./pages/ChartPage";
import EditParamsPage from "./pages/EditParamsPage";
import ControlPage from "./pages/ControlPage";
import ParamsViewPage from "./pages/ParamsViewPage";
import ResultsPage from "./pages/ResultsPage";
import ResultDetailPage from "./pages/ResultDetailPage";
import { WebSocketProvider } from "./context/WebSocketContext";

export default function App() {
  return (
    <WebSocketProvider>
      <Router>
        <Layout>
          <Routes>
            <Route path="/" element={<HomePage />} />
            <Route path="/websocket" element={<WebSocketPage />} />
            <Route path="/chart" element={<ChartPage />} />
            <Route path="/edit-params" element={<EditParamsPage />} />
            <Route path="/control" element={<ControlPage />} />
            <Route path="/params-view" element={<ParamsViewPage />} />
            <Route path="/results" element={<ResultsPage />} />
            <Route path="/results/:id" element={<ResultDetailPage />} />
            {/* 👆 Aquí podrás añadir más páginas: */}
            {/* <Route path="/dashboard" element={<DashboardPage />} /> */}
            {/* <Route path="/settings" element={<SettingsPage />} /> */}
          </Routes>
        </Layout>
      </Router>
    </WebSocketProvider>
  );
}
