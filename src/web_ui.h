#pragma once

#include <Arduino.h>

const char GREMLINO_INDEX_HTML[] PROGMEM = R"GREMLINO(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
  <meta name="theme-color" content="#0b0d10">
  <title>Gremlino</title>
  <style>
    :root {
      color-scheme: dark;
      --bg:#090b0e;
      --panel:#111419;
      --panel-2:#161a20;
      --line:#252b33;
      --text:#f4f7f8;
      --muted:#929ba6;
      --lime:#b7ff42;
      --lime-2:#7fdc22;
      --purple:#a77bff;
      --danger:#ff5f6d;
      --shadow:0 24px 70px rgba(0,0,0,.38);
      --radius:22px;
    }

    * { box-sizing:border-box; }

    body {
      margin:0;
      min-height:100vh;
      font-family:Inter,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif;
      background:
        radial-gradient(900px 500px at 15% -10%, rgba(183,255,66,.12), transparent 60%),
        radial-gradient(700px 520px at 105% 15%, rgba(167,123,255,.12), transparent 55%),
        var(--bg);
      color:var(--text);
    }

    button,input,select { font:inherit; }

    .shell {
      width:min(1040px,100%);
      margin:0 auto;
      padding:28px 18px 44px;
    }

    header {
      display:flex;
      justify-content:space-between;
      align-items:center;
      gap:20px;
      margin-bottom:22px;
    }

    .brand { display:flex; align-items:center; gap:14px; }

    .mark {
      width:48px; height:48px;
      display:grid; place-items:center;
      border-radius:15px;
      background:linear-gradient(145deg,var(--lime),#74c913);
      color:#0b0d10;
      font-weight:950;
      font-size:25px;
      letter-spacing:-1px;
      box-shadow:0 10px 32px rgba(183,255,66,.2);
      transform:rotate(-2deg);
    }

    .brand h1 {
      margin:0;
      font-size:23px;
      line-height:1;
      letter-spacing:-.5px;
    }

    .brand p {
      margin:6px 0 0;
      color:var(--muted);
      font-size:13px;
    }

    .live {
      display:flex; align-items:center; gap:8px;
      padding:9px 12px;
      border:1px solid var(--line);
      border-radius:999px;
      background:rgba(17,20,25,.72);
      color:var(--muted);
      font-size:12px;
      font-weight:750;
    }

    .dot {
      width:8px; height:8px;
      border-radius:50%;
      background:var(--danger);
      box-shadow:0 0 0 5px rgba(255,95,109,.08);
    }

    .live.ok .dot {
      background:var(--lime);
      box-shadow:0 0 0 5px rgba(183,255,66,.08);
    }

    .status {
      display:grid;
      grid-template-columns:repeat(4,1fr);
      gap:10px;
      margin-bottom:12px;
    }

    .stat {
      background:rgba(17,20,25,.78);
      border:1px solid var(--line);
      border-radius:16px;
      padding:14px 15px;
      min-width:0;
    }

    .stat span {
      display:block;
      color:var(--muted);
      font-size:11px;
      text-transform:uppercase;
      letter-spacing:.09em;
      margin-bottom:6px;
    }

    .stat strong {
      display:block;
      font-size:15px;
      overflow:hidden;
      white-space:nowrap;
      text-overflow:ellipsis;
    }

    .grid {
      display:grid;
      grid-template-columns:1fr 1fr;
      gap:12px;
    }

    .card {
      background:linear-gradient(180deg,rgba(20,24,29,.95),rgba(15,18,22,.95));
      border:1px solid var(--line);
      border-radius:var(--radius);
      padding:20px;
      box-shadow:var(--shadow);
    }

    .card.wide { grid-column:1/-1; }

    .card-head {
      display:flex;
      align-items:flex-start;
      justify-content:space-between;
      gap:14px;
      margin-bottom:18px;
    }

    .eyebrow {
      display:block;
      color:var(--lime);
      font-size:10px;
      font-weight:850;
      letter-spacing:.15em;
      text-transform:uppercase;
      margin-bottom:7px;
    }

    h2 {
      margin:0;
      font-size:20px;
      letter-spacing:-.35px;
    }

    .sub {
      color:var(--muted);
      font-size:13px;
      line-height:1.5;
      margin:7px 0 0;
      max-width:48ch;
    }

    .switch {
      position:relative;
      display:inline-flex;
      width:52px; height:30px;
      flex:0 0 auto;
    }

    .switch input { opacity:0; width:0; height:0; }

    .slider {
      position:absolute; inset:0;
      cursor:pointer;
      border-radius:999px;
      background:#252b33;
      border:1px solid #343c46;
      transition:.2s ease;
    }

    .slider:before {
      content:"";
      position:absolute;
      width:22px; height:22px;
      left:3px; top:3px;
      border-radius:50%;
      background:#dce1e5;
      transition:.2s ease;
      box-shadow:0 2px 8px rgba(0,0,0,.32);
    }

    .switch input:checked + .slider {
      background:rgba(183,255,66,.17);
      border-color:rgba(183,255,66,.45);
    }

    .switch input:checked + .slider:before {
      transform:translateX(22px);
      background:var(--lime);
    }

    .fields {
      display:grid;
      grid-template-columns:1fr 1fr;
      gap:10px;
    }

    .field { min-width:0; }

    label.caption {
      display:block;
      color:var(--muted);
      font-size:11px;
      font-weight:700;
      margin:0 0 7px 2px;
    }

    input[type="number"], select {
      width:100%;
      border:1px solid var(--line);
      border-radius:12px;
      background:#0d1014;
      color:var(--text);
      padding:11px 12px;
      outline:none;
    }

    input[type="number"]:focus, select:focus {
      border-color:rgba(183,255,66,.5);
      box-shadow:0 0 0 3px rgba(183,255,66,.07);
    }

    .range-wrap { margin-top:14px; }

    .range-top {
      display:flex; justify-content:space-between;
      color:var(--muted);
      font-size:11px;
      margin-bottom:9px;
    }

    .range-top b { color:var(--text); }

    input[type="range"] {
      width:100%;
      accent-color:var(--lime);
    }

    .segments {
      display:grid;
      grid-template-columns:repeat(3,1fr);
      gap:7px;
      margin-bottom:14px;
    }

    .seg {
      border:1px solid var(--line);
      background:#0d1014;
      color:var(--muted);
      padding:10px 8px;
      border-radius:11px;
      cursor:pointer;
      font-weight:800;
      font-size:12px;
      transition:.16s ease;
    }

    .seg:hover { color:var(--text); border-color:#3a424c; }

    .seg.active {
      color:#0b0d10;
      background:var(--lime);
      border-color:var(--lime);
    }

    .toolbar {
      display:flex;
      gap:8px;
      flex-wrap:wrap;
      margin-top:16px;
    }

    .btn {
      border:1px solid var(--line);
      border-radius:12px;
      padding:10px 13px;
      background:#171b21;
      color:var(--text);
      font-weight:800;
      font-size:12px;
      cursor:pointer;
      transition:.16s ease;
    }

    .btn:hover { transform:translateY(-1px); border-color:#404955; }
    .btn:active { transform:translateY(0); }
    .btn.primary { background:var(--lime); color:#0b0d10; border-color:var(--lime); }
    .btn.ghost { background:#0d1014; }
    .btn.danger {
      background:rgba(255,95,109,.1);
      border-color:rgba(255,95,109,.28);
      color:#ff9da6;
    }

    .activity {
      display:flex;
      align-items:center;
      justify-content:space-between;
      gap:16px;
      padding:15px 16px;
      border-radius:15px;
      border:1px solid var(--line);
      background:#0d1014;
    }

    .activity small { display:block; color:var(--muted); margin-bottom:5px; }
    .activity strong { font-size:14px; }
    .activity .next { text-align:right; flex:0 0 auto; }

    .panic {
      margin-top:12px;
      width:100%;
      min-height:54px;
      border-radius:15px;
      border:1px solid rgba(255,95,109,.35);
      background:linear-gradient(180deg,rgba(255,95,109,.16),rgba(255,95,109,.09));
      color:#ffb4bb;
      font-weight:900;
      letter-spacing:.03em;
      cursor:pointer;
    }

    .panic:hover { border-color:rgba(255,95,109,.65); }

    .foot {
      color:#707984;
      font-size:11px;
      line-height:1.6;
      text-align:center;
      margin-top:18px;
    }

    .toast {
      position:fixed;
      left:50%; bottom:22px;
      transform:translate(-50%,18px);
      opacity:0;
      pointer-events:none;
      background:#e9edf0;
      color:#0b0d10;
      border-radius:999px;
      padding:10px 14px;
      font-size:12px;
      font-weight:800;
      transition:.2s ease;
      box-shadow:0 12px 36px rgba(0,0,0,.35);
      z-index:20;
    }

    .toast.show { opacity:1; transform:translate(-50%,0); }

    @media (max-width:760px) {
      .shell { padding:20px 14px 34px; }
      .status { grid-template-columns:1fr 1fr; }
      .grid { grid-template-columns:1fr; }
      .card.wide { grid-column:auto; }
      .brand p { display:none; }
      .mark { width:44px; height:44px; }
    }

    @media (max-width:420px) {
      header { align-items:flex-start; }
      .live { padding:8px 10px; }
      .card { padding:17px; border-radius:18px; }
      .fields { grid-template-columns:1fr; }
    }
  </style>
</head>
<body>
  <main class="shell">
    <header>
      <div class="brand">
        <div class="mark">G</div>
        <div>
          <h1>Gremlino</h1>
          <p>USB HID mouse control · local only</p>
        </div>
      </div>
      <div id="live" class="live"><span class="dot"></span><span id="liveText">Connecting</span></div>
    </header>

    <section class="status">
      <div class="stat"><span>HID</span><strong id="hid">—</strong></div>
      <div class="stat"><span>USB</span><strong id="usb">—</strong></div>
      <div class="stat"><span>Wi-Fi clients</span><strong id="clients">—</strong></div>
      <div class="stat"><span>Uptime</span><strong id="uptime">—</strong></div>
    </section>

    <section class="grid">
      <article class="card">
        <div class="card-head">
          <div>
            <span class="eyebrow">Stealth</span>
            <h2>Away Killer</h2>
            <p class="sub">Tiny random nudges, immediately returned to origin. Quiet and deliberately boring.</p>
          </div>
          <label class="switch" aria-label="Toggle Away Killer">
            <input id="idleToggle" type="checkbox">
            <span class="slider"></span>
          </label>
        </div>

        <div class="fields">
          <div class="field">
            <label class="caption" for="minSec">Minimum interval</label>
            <input id="minSec" type="number" min="5" max="300" value="20">
          </div>
          <div class="field">
            <label class="caption" for="maxSec">Maximum interval</label>
            <input id="maxSec" type="number" min="5" max="300" value="40">
          </div>
        </div>

        <div class="range-wrap">
          <div class="range-top"><span>Movement</span><b><span id="ampValue">2</span> px</b></div>
          <input id="amplitude" type="range" min="1" max="8" value="2">
        </div>

        <div class="toolbar">
          <button class="btn primary" id="saveConfig">Apply settings</button>
          <button class="btn ghost" data-action="nudge">Test nudge</button>
        </div>
      </article>

      <article class="card">
        <div class="card-head">
          <div>
            <span class="eyebrow">Mischief</span>
            <h2>Gremlin Mode</h2>
            <p class="sub">Randomized mouse-only patterns with long quiet gaps. Always starts OFF after reboot.</p>
          </div>
          <label class="switch" aria-label="Toggle Gremlin Mode">
            <input id="gremlinToggle" type="checkbox">
            <span class="slider"></span>
          </label>
        </div>

        <label class="caption">Intensity</label>
        <div class="segments">
          <button class="seg active" data-intensity="1">Mild</button>
          <button class="seg" data-intensity="2">Spicy</button>
          <button class="seg" data-intensity="3">Chaos</button>
        </div>

        <div class="field">
          <label class="caption" for="session">Session</label>
          <select id="session">
            <option value="15">15 minutes</option>
            <option value="60" selected>1 hour</option>
            <option value="240">4 hours</option>
            <option value="0">Until reboot</option>
          </select>
        </div>

        <div class="toolbar">
          <button class="btn ghost" data-action="nudge">Nudge</button>
          <button class="btn ghost" data-action="orbit">Orbit</button>
        </div>
      </article>

      <article class="card wide">
        <div class="card-head">
          <div>
            <span class="eyebrow">Control</span>
            <h2>Current activity</h2>
          </div>
        </div>

        <div class="activity">
          <div>
            <small>Last action</small>
            <strong id="lastAction">Boot</strong>
          </div>
          <div class="next">
            <small>Next action</small>
            <strong id="nextAction">—</strong>
          </div>
        </div>

        <button id="stopAll" class="panic">STOP ALL ACTIVITY</button>
      </article>
    </section>

    <p class="foot">BOOT/GPIO0 is the physical panic button. Dashboard traffic stays on the Gremlino access point.</p>
  </main>

  <div id="toast" class="toast">Saved</div>

  <script>
    const byId = (id) => document.getElementById(id);
    const els = {
      live: byId("live"), liveText: byId("liveText"), hid: byId("hid"), usb: byId("usb"),
      clients: byId("clients"), uptime: byId("uptime"), idle: byId("idleToggle"),
      gremlin: byId("gremlinToggle"), min: byId("minSec"), max: byId("maxSec"),
      amp: byId("amplitude"), ampValue: byId("ampValue"), session: byId("session"),
      last: byId("lastAction"), next: byId("nextAction"), toast: byId("toast")
    };

    let intensity = 1;
    let hydrated = false;
    let toastTimer;

    function showToast(message) {
      els.toast.textContent = message;
      els.toast.classList.add("show");
      clearTimeout(toastTimer);
      toastTimer = setTimeout(() => els.toast.classList.remove("show"), 1600);
    }

    function formatUptime(seconds) {
      seconds = Number(seconds || 0);
      const d = Math.floor(seconds / 86400);
      const h = Math.floor((seconds % 86400) / 3600);
      const m = Math.floor((seconds % 3600) / 60);
      if (d) return d + "d " + h + "h";
      if (h) return h + "h " + m + "m";
      return m + "m";
    }

    async function post(path, data) {
      const response = await fetch(path, {
        method: "POST",
        headers: {"Content-Type":"application/x-www-form-urlencoded"},
        body: new URLSearchParams(data)
      });
      if (!response.ok) throw new Error(await response.text());
      return response.json();
    }

    function setIntensity(value) {
      intensity = Number(value);
      document.querySelectorAll("[data-intensity]").forEach((button) => {
        button.classList.toggle("active", Number(button.dataset.intensity) === intensity);
      });
    }

    async function saveConfig(quiet) {
      let min = Math.max(5, Math.min(300, Number(els.min.value || 20)));
      let max = Math.max(5, Math.min(300, Number(els.max.value || 40)));
      if (max < min) { const t = min; min = max; max = t; }
      els.min.value = min;
      els.max.value = max;

      await post("/api/config", {
        min: min,
        max: max,
        amp: els.amp.value,
        intensity: intensity,
        session: els.session.value
      });
      if (!quiet) showToast("Settings applied");
    }

    async function runAction(name, extra) {
      try {
        const data = Object.assign({name:name}, extra || {});
        await post("/api/action", data);
        await refresh();
      } catch (error) {
        showToast(error.message || "Action failed");
      }
    }

    async function refresh() {
      try {
        const response = await fetch("/api/status", {cache:"no-store"});
        if (!response.ok) throw new Error("offline");
        const s = await response.json();

        els.live.classList.add("ok");
        els.liveText.textContent = "Online";
        els.hid.textContent = s.hid_ready ? "Ready" : "Waiting";
        els.usb.textContent = s.usb_suspended ? "Suspended" : (s.usb_started ? "Active" : "Starting");
        els.clients.textContent = s.wifi_clients;
        els.uptime.textContent = formatUptime(s.uptime);
        els.last.textContent = s.last_action || "—";
        els.next.textContent = s.next_in < 0 ? "—" : s.next_in + "s";

        els.idle.checked = !!s.idle_enabled;
        els.gremlin.checked = !!s.gremlin_enabled;

        if (!hydrated) {
          els.min.value = s.min_sec;
          els.max.value = s.max_sec;
          els.amp.value = s.amplitude;
          els.ampValue.textContent = s.amplitude;
          els.session.value = String(s.session_min);
          setIntensity(s.intensity);
          hydrated = true;
        }
      } catch (error) {
        els.live.classList.remove("ok");
        els.liveText.textContent = "Offline";
      }
    }

    els.amp.addEventListener("input", () => els.ampValue.textContent = els.amp.value);

    document.querySelectorAll("[data-intensity]").forEach((button) => {
      button.addEventListener("click", async () => {
        setIntensity(button.dataset.intensity);
        try { await saveConfig(true); } catch (_) {}
      });
    });

    document.querySelectorAll("[data-action]").forEach((button) => {
      button.addEventListener("click", () => runAction(button.dataset.action));
    });

    byId("saveConfig").addEventListener("click", async () => {
      try { await saveConfig(false); } catch (error) { showToast("Could not save"); }
    });

    els.idle.addEventListener("change", async () => {
      await runAction("idle", {enabled: els.idle.checked ? 1 : 0});
    });

    els.gremlin.addEventListener("change", async () => {
      try {
        await saveConfig(true);
        await runAction("gremlin", {enabled: els.gremlin.checked ? 1 : 0});
      } catch (error) {
        els.gremlin.checked = false;
        showToast("Could not start Gremlin Mode");
      }
    });

    byId("stopAll").addEventListener("click", () => runAction("stop"));

    refresh();
    setInterval(refresh, 1000);
  </script>
</body>
</html>
)GREMLINO";
