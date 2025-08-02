
import React from 'react';
import ReactDOM from 'react-dom/client';
import { BrowserRouter, Routes, Route } from 'react-router-dom';
import Home from '../app/routes/home';
import Auth from '../app/routes/auth';
import './index.css';

function injectPuterScript(callback: () => void) {
  const script = document.createElement('script');
  script.src = 'https://js.puter.com/v2/';
  script.async = true;
  script.onload = callback;
  document.head.appendChild(script);
}

injectPuterScript(() => {
  ReactDOM.createRoot(document.getElementById('root')!).render(
    <React.StrictMode>
      <BrowserRouter>
        <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/auth" element={<Auth />} />
        </Routes>
      </BrowserRouter>
    </React.StrictMode>
  );
});
