import Footer from "./Footer";
import Header from "./Header";

import Sidebar from "./Sidebar";

export default function Layout({ children }: { children: React.ReactNode }) {
  return (
    <div className="min-h-screen bg-gray-900 text-gray-100">
      <Header />
      <Sidebar />
      <main className="pt-10 pl-2 pr-4 pb-8">{children}</main>
      <Footer />
    </div>
  );
}
