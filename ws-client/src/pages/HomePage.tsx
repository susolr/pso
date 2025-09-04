import { Link } from "react-router-dom";

export default function HomePage() {
  return (
    <div className="flex flex-col items-center text-center px-6 pt-24 pb-12">
      <div className="max-w-3xl w-full">
        <div className="bg-gradient-to-r from-indigo-500 via-sky-500 to-emerald-500 rounded-2xl p-0.5 shadow-lg mb-8">
          <div className="bg-white dark:bg-neutral-900 rounded-2xl p-8">
            <h1 className="text-4xl md:text-5xl font-extrabold tracking-tight mb-4">
              Welcome to your BPSO Dashboard
            </h1>
            <p className="text-lg md:text-xl text-gray-600 dark:text-gray-300 mb-6">
              Launch runs, explore results, and manage your optimization
              workflow.
            </p>
            <div className="flex flex-col sm:flex-row gap-3 justify-center">
              <Link
                to="/control"
                className="inline-flex items-center justify-center px-6 py-3 rounded-lg bg-indigo-100 hover:bg-indigo-200 text-indigo-800 font-bold shadow-md"
              >
                Go to Control
              </Link>
              <Link
                to="/results"
                className="inline-flex items-center justify-center px-6 py-3 rounded-lg bg-emerald-100 hover:bg-emerald-200 text-emerald-800 font-bold shadow-md"
              >
                View Results
              </Link>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
