#pragma once

#include <Arduino.h>

const char GREMLINO_INDEX_HTML[] PROGMEM = R"GREMLINO(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
  <meta name="theme-color" content="#090b0e">
  <title>Gremlino</title>
  <style>
    :root {
      color-scheme: dark;
      --bg:#080a0d;
      --panel:#111419;
      --panel-2:#161a20;
      --field:#0c0f13;
      --line:#252b33;
      --line-hi:#3b444f;
      --text:#f4f7f8;
      --muted:#929ba6;
      --lime:#b7ff42;
      --lime-2:#82dc29;
      --purple:#a77bff;
      --danger:#ff6572;
      --warn:#ffc857;
      --shadow:0 24px 70px rgba(0,0,0,.40);
      --radius:22px;
    }

    * { box-sizing:border-box; }

    html { min-height:100%; }

    body {
      margin:0;
      min-height:100vh;
      font-family:Inter,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif;
      background:
        radial-gradient(900px 520px at 13% -12%,rgba(183,255,66,.12),transparent 60%),
        radial-gradient(720px 520px at 106% 13%,rgba(167,123,255,.11),transparent 56%),
        var(--bg);
      color:var(--text);
    }

    button,input,select { font:inherit; }

    button:focus-visible,input:focus-visible,select:focus-visible {
      outline:2px solid var(--lime);
      outline-offset:2px;
    }

    .shell {
      width:min(1080px,100%);
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
      width:49px;
      height:49px;
      display:grid;
      place-items:center;
      border-radius:15px;
      background:linear-gradient(145deg,var(--lime),#74c913);
      color:#0b0d10;
      font-weight:950;
      font-size:25px;
      letter-spacing:-1px;
      box-shadow:0 10px 32px rgba(183,255,66,.20);
      transform:rotate(-2deg);
      user-select:none;
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
      display:flex;
      align-items:center;
      gap:8px;
      padding:9px 12px;
      border:1px solid var(--line);
      border-radius:999px;
      background:rgba(17,20,25,.74);
      color:var(--muted);
      font-size:12px;
      font-weight:750;
      backdrop-filter:blur(10px);
    }

    .dot {
      width:8px;
      height:8px;
      border-radius:50%;
      background:var(--danger);
      box-shadow:0 0 0 5px rgba(255,101,114,.08);
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
      background:rgba(17,20,25,.80);
      border:1px solid var(--line);
      border-radius:16px;
      padding:14px 15px;
      min-width:0;
      backdrop-filter:blur(10px);
    }

    .stat span {
      display:block;
      color:var(--muted);
      font-size:10px;
      font-weight:760;
      text-transform:uppercase;
      letter-spacing:.10em;
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
      background:linear-gradient(180deg,rgba(20,24,29,.96),rgba(14,17,21,.96));
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
      max-width:52ch;
    }

    .switch {
      position:relative;
      display:inline-flex;
      width:52px;
      height:30px;
      flex:0 0 auto;
    }

    .switch input {
      opacity:0;
      width:0;
      height:0;
    }

    .slider {
      position:absolute;
      inset:0;
      cursor:pointer;
      border-radius:999px;
      background:#252b33;
      border:1px solid #343c46;
      transition:.2s ease;
    }

    .slider:before {
      content:"";
      position:absolute;
      width:22px;
      height:22px;
      left:3px;
      top:3px;
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

    input[type="number"],
    input[type="text"],
    input[type="password"],
    select {
      width:100%;
      border:1px solid var(--line);
      border-radius:12px;
      background:var(--field);
      color:var(--text);
      padding:11px 12px;
      outline:none;
      transition:border-color .16s ease,box-shadow .16s ease;
    }

    input::placeholder { color:#5f6872; }

    input:focus,
    select:focus {
      border-color:rgba(183,255,66,.52);
      box-shadow:0 0 0 3px rgba(183,255,66,.07);
    }

    .range-wrap { margin-top:14px; }

    .range-top {
      display:flex;
      justify-content:space-between;
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
      background:var(--field);
      color:var(--muted);
      padding:10px 8px;
      border-radius:11px;
      cursor:pointer;
      font-weight:800;
      font-size:12px;
      transition:.16s ease;
    }

    .seg:hover {
      color:var(--text);
      border-color:var(--line-hi);
    }

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

    .btn:hover {
      transform:translateY(-1px);
      border-color:#404955;
    }

    .btn:active { transform:translateY(0); }

    .btn.primary {
      background:var(--lime);
      color:#0b0d10;
      border-color:var(--lime);
    }

    .btn.ghost { background:var(--field); }

    .btn.danger {
      background:rgba(255,101,114,.10);
      border-color:rgba(255,101,114,.28);
      color:#ffabb3;
    }

    .activity {
      display:grid;
      grid-template-columns:1fr auto auto;
      align-items:center;
      gap:22px;
      padding:15px 16px;
      border-radius:15px;
      border:1px solid var(--line);
      background:var(--field);
    }

    .activity small,
    .meta small {
      display:block;
      color:var(--muted);
      margin-bottom:5px;
      font-size:11px;
    }

    .activity strong,
    .meta strong {
      font-size:14px;
    }

    .activity .right { text-align:right; }

    .panic {
      margin-top:12px;
      width:100%;
      min-height:54px;
      border-radius:15px;
      border:1px solid rgba(255,101,114,.35);
      background:linear-gradient(180deg,rgba(255,101,114,.16),rgba(255,101,114,.09));
      color:#ffb4bb;
      font-weight:900;
      letter-spacing:.03em;
      cursor:pointer;
      transition:.16s ease;
    }

    .panic:hover {
      border-color:rgba(255,101,114,.65);
      background:linear-gradient(180deg,rgba(255,101,114,.20),rgba(255,101,114,.11));
    }

    .device-grid {
      display:grid;
      grid-template-columns:1.25fr .75fr;
      gap:18px;
    }

    .device-pane + .device-pane {
      border-left:1px solid var(--line);
      padding-left:18px;
    }

    .pane-title {
      margin:0 0 14px;
      font-size:13px;
      font-weight:850;
    }

    .helper {
      color:var(--muted);
      font-size:11px;
      line-height:1.5;
      margin:7px 0 0;
    }

    .meta-list {
      display:grid;
      grid-template-columns:1fr 1fr;
      gap:8px;
      margin-bottom:14px;
    }

    .meta {
      padding:11px 12px;
      border:1px solid var(--line);
      border-radius:12px;
      background:var(--field);
      min-width:0;
    }

    .meta strong {
      display:block;
      white-space:nowrap;
      overflow:hidden;
      text-overflow:ellipsis;
    }

    .notice {
      display:none;
      margin-top:12px;
      padding:11px 12px;
      border:1px solid rgba(255,200,87,.28);
      border-radius:12px;
      background:rgba(255,200,87,.08);
      color:#ffe09b;
      font-size:11px;
      line-height:1.5;
    }

    .notice.show { display:block; }

    .system-actions {
      display:grid;
      gap:8px;
    }

    .system-actions .btn {
      width:100%;
      text-align:left;
      padding:12px 13px;
    }

    .danger-zone {
      margin-top:12px;
      padding-top:12px;
      border-top:1px solid var(--line);
    }

    .foot {
      display:flex;
      justify-content:space-between;
      gap:12px;
      color:#707984;
      font-size:11px;
      line-height:1.6;
      margin-top:18px;
      padding:0 3px;
    }

    .foot span:last-child { text-align:right; }

    .toast {
      position:fixed;
      left:50%;
      bottom:22px;
      transform:translate(-50%,18px);
      opacity:0;
      pointer-events:none;
      max-width:calc(100vw - 28px);
      background:#e9edf0;
      color:#0b0d10;
      border-radius:999px;
      padding:10px 14px;
      font-size:12px;
      font-weight:800;
      text-align:center;
      transition:.2s ease;
      box-shadow:0 12px 36px rgba(0,0,0,.35);
      z-index:20;
    }

    .toast.error {
      background:#ffccd1;
      color:#3d090e;
    }

    .toast.show {
      opacity:1;
      transform:translate(-50%,0);
    }

    @media (max-width:780px) {
      .shell { padding:20px 14px 34px; }
      .status { grid-template-columns:1fr 1fr; }
      .grid { grid-template-columns:1fr; }
      .card.wide { grid-column:auto; }
      .device-grid { grid-template-columns:1fr; }
      .device-pane + .device-pane {
        border-left:0;
        border-top:1px solid var(--line);
        padding-left:0;
        padding-top:18px;
      }
      .brand p { display:none; }
      .mark { width:44px; height:44px; }
      .activity { grid-template-columns:1fr 1fr; }
      .activity > div:first-child { grid-column:1/-1; }
      .activity .right { text-align:left; }
    }

    @media (max-width:430px) {
      header { align-items:flex-start; }
      .live { padding:8px 10px; }
      .card { padding:17px; border-radius:18px; }
      .fields,.meta-list { grid-template-columns:1fr; }
      .foot { display:block; text-align:center; }
      .foot span { display:block; }
      .foot span:last-child { text-align:center; margin-top:3px; }
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
      <div id="live" class="live">
        <span class="dot"></span>
        <span id="liveText">Connecting</span>
      </div>
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
          <div class="range-top">
            <span>Movement</span>
            <b><span id="ampValue">2</span> px</b>
          </div>
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
          <div class="right">
            <small>Next action</small>
            <strong id="nextAction">—</strong>
          </div>
          <div class="right">
            <small>Gremlin session</small>
            <strong id="sessionLeft">—</strong>
          </div>
        </div>

        <button id="stopAll" class="panic">STOP ALL ACTIVITY</button>
      </article>

      <article class="card wide">
        <div class="card-head">
          <div>
            <span class="eyebrow">Device</span>
            <h2>Gremlino settings</h2>
            <p class="sub">Network settings are stored locally on the board. No cloud, account or external service is involved.</p>
          </div>
        </div>

        <div class="device-grid">
          <section class="device-pane">
            <h3 class="pane-title">Wi-Fi access point</h3>

            <div class="fields">
              <div class="field">
                <label class="caption" for="ssid">Network name</label>
                <input id="ssid" type="text" maxlength="32" autocomplete="off">
              </div>
              <div class="field">
                <label class="caption" for="apPassword">New password</label>
                <input id="apPassword" type="password" minlength="8" maxlength="63" placeholder="Leave blank to keep current" autocomplete="new-password">
              </div>
            </div>

            <p class="helper">Password must contain 8–63 characters. Changing the network name or password requires a restart.</p>

            <div class="toolbar">
              <button id="saveNetwork" class="btn primary">Save network</button>
            </div>

            <div id="restartNotice" class="notice">
              Network settings saved. Restart Gremlino to apply them.
              <div class="toolbar">
                <button id="restartFromNotice" class="btn ghost">Restart now</button>
              </div>
            </div>
          </section>

          <section class="device-pane">
            <h3 class="pane-title">System</h3>

            <div class="meta-list">
              <div class="meta"><small>Firmware</small><strong id="version">—</strong></div>
              <div class="meta"><small>Local IP</small><strong id="ip">—</strong></div>
              <div class="meta"><small>Free heap</small><strong id="heap">—</strong></div>
              <div class="meta"><small>AP</small><strong id="apName">—</strong></div>
            </div>

            <div class="system-actions">
              <button id="reboot" class="btn ghost">Restart Gremlino</button>
            </div>

            <div class="danger-zone">
              <button id="factoryReset" class="btn danger">Factory reset</button>
              <p class="helper">Factory reset clears saved modes, timing and Wi-Fi settings. Holding BOOT for 7 seconds does the same.</p>
            </div>
          </section>
        </div>
      </article>
    </section>

    <p class="foot">
      <span>BOOT: tap = panic stop · hold 7 s = factory reset</span>
      <span id="footerVersion">Gremlino</span>
    </p>
  </main>

  <div id="toast" class="toast">Saved</div>

  <script>
    const byId = (id) => document.getElementById(id);

    const els = {
      live: byId("live"),
      liveText: byId("liveText"),
      hid: byId("hid"),
      usb: byId("usb"),
      clients: byId("clients"),
      uptime: byId("uptime"),
      idle: byId("idleToggle"),
      gremlin: byId("gremlinToggle"),
      min: byId("minSec"),
      max: byId("maxSec"),
      amp: byId("amplitude"),
      ampValue: byId("ampValue"),
      session: byId("session"),
      last: byId("lastAction"),
      next: byId("nextAction"),
      sessionLeft: byId("sessionLeft"),
      ssid: byId("ssid"),
      password: byId("apPassword"),
      version: byId("version"),
      ip: byId("ip"),
      heap: byId("heap"),
      apName: byId("apName"),
      footerVersion: byId("footerVersion"),
      restartNotice: byId("restartNotice"),
      toast: byId("toast")
    };

    let intensity = 1;
    let hydrated = false;
    let toastTimer;

    function showToast(message, error) {
      els.toast.textContent = message;
      els.toast.classList.toggle("error", !!error);
      els.toast.classList.add("show");
      clearTimeout(toastTimer);
      toastTimer = setTimeout(() => els.toast.classList.remove("show"), 1900);
    }

    function formatDuration(seconds) {
      seconds = Number(seconds);
      if (!Number.isFinite(seconds) || seconds < 0) return "—";

      const d = Math.floor(seconds / 86400);
      const h = Math.floor((seconds % 86400) / 3600);
      const m = Math.floor((seconds % 3600) / 60);
      const s = Math.floor(seconds % 60);

      if (d) return d + "d " + h + "h";
      if (h) return h + "h " + m + "m";
      if (m) return m + "m " + s + "s";
      return s + "s";
    }

    function formatBytes(bytes) {
      const value = Number(bytes || 0);
      if (value >= 1048576) return (value / 1048576).toFixed(1) + " MB";
      if (value >= 1024) return Math.round(value / 1024) + " KB";
      return value + " B";
    }

    async function request(path, options) {
      const response = await fetch(path, options || {});
      const text = await response.text();
      let data = {};

      if (text) {
        try { data = JSON.parse(text); }
        catch (_) { data = {error:text}; }
      }

      if (!response.ok) {
        throw new Error(data.error || "Request failed");
      }

      return data;
    }

    async function post(path, data) {
      return request(path, {
        method:"POST",
        headers:{"Content-Type":"application/x-www-form-urlencoded"},
        body:new URLSearchParams(data)
      });
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

      if (max < min) {
        const temp = min;
        min = max;
        max = temp;
      }

      els.min.value = min;
      els.max.value = max;

      await post("/api/config", {
        min:min,
        max:max,
        amp:els.amp.value,
        intensity:intensity,
        session:els.session.value
      });

      if (!quiet) showToast("Settings applied");
    }

    async function runAction(name, extra) {
      try {
        const data = Object.assign({name:name}, extra || {});
        await post("/api/action", data);
        await refresh();
      } catch (error) {
        showToast(error.message || "Action failed", true);
        throw error;
      }
    }

    async function systemAction(name) {
      await post("/api/system", {name:name});
    }

    async function refresh() {
      try {
        const s = await request("/api/status", {cache:"no-store"});

        els.live.classList.add("ok");
        els.liveText.textContent = s.restart_pending ? "Restarting" : "Online";
        els.hid.textContent = s.hid_ready ? "Ready" : "Waiting";
        els.usb.textContent = s.usb_suspended ? "Suspended" : (s.usb_started ? "Active" : "Starting");
        els.clients.textContent = s.wifi_clients;
        els.uptime.textContent = formatDuration(s.uptime);
        els.last.textContent = s.last_action || "—";
        els.next.textContent = formatDuration(s.next_in);
        els.sessionLeft.textContent = formatDuration(s.gremlin_remaining);

        els.idle.checked = !!s.idle_enabled;
        els.gremlin.checked = !!s.gremlin_enabled;

        els.version.textContent = "v" + s.version;
        els.footerVersion.textContent = "Gremlino v" + s.version;
        els.ip.textContent = s.ip || "—";
        els.heap.textContent = formatBytes(s.free_heap);
        els.apName.textContent = s.ap_ssid || "—";

        if (!hydrated) {
          els.min.value = s.min_sec;
          els.max.value = s.max_sec;
          els.amp.value = s.amplitude;
          els.ampValue.textContent = s.amplitude;
          els.session.value = String(s.session_min);
          els.ssid.value = s.ap_ssid || "";
          setIntensity(s.intensity);
          hydrated = true;
        }
      } catch (error) {
        els.live.classList.remove("ok");
        els.liveText.textContent = "Offline";
      }
    }

    els.amp.addEventListener("input", () => {
      els.ampValue.textContent = els.amp.value;
    });

    document.querySelectorAll("[data-intensity]").forEach((button) => {
      button.addEventListener("click", async () => {
        setIntensity(button.dataset.intensity);
        try { await saveConfig(true); }
        catch (error) { showToast(error.message || "Could not save", true); }
      });
    });

    document.querySelectorAll("[data-action]").forEach((button) => {
      button.addEventListener("click", () => runAction(button.dataset.action));
    });

    byId("saveConfig").addEventListener("click", async () => {
      try { await saveConfig(false); }
      catch (error) { showToast(error.message || "Could not save", true); }
    });

    els.idle.addEventListener("change", async () => {
      try {
        await runAction("idle", {enabled:els.idle.checked ? 1 : 0});
      } catch (_) {
        els.idle.checked = !els.idle.checked;
      }
    });

    els.gremlin.addEventListener("change", async () => {
      const wanted = els.gremlin.checked;

      try {
        await saveConfig(true);
        await runAction("gremlin", {enabled:wanted ? 1 : 0});
      } catch (_) {
        els.gremlin.checked = !wanted;
      }
    });

    byId("stopAll").addEventListener("click", async () => {
      try {
        await runAction("stop");
        showToast("All activity stopped");
      } catch (_) {}
    });

    byId("saveNetwork").addEventListener("click", async () => {
      const ssid = els.ssid.value.trim();

      if (!ssid || ssid.length > 32) {
        showToast("SSID must be 1–32 characters", true);
        return;
      }

      if (els.password.value && (els.password.value.length < 8 || els.password.value.length > 63)) {
        showToast("Password must be 8–63 characters", true);
        return;
      }

      try {
        await post("/api/network", {
          ssid:ssid,
          password:els.password.value
        });

        els.password.value = "";
        els.restartNotice.classList.add("show");
        showToast("Network settings saved");
      } catch (error) {
        showToast(error.message || "Could not save network", true);
      }
    });

    async function rebootNow() {
      try {
        showToast("Restarting Gremlino…");
        await systemAction("reboot");
        setTimeout(() => {
          els.live.classList.remove("ok");
          els.liveText.textContent = "Restarting";
        }, 250);
      } catch (error) {
        showToast(error.message || "Restart failed", true);
      }
    }

    byId("reboot").addEventListener("click", rebootNow);
    byId("restartFromNotice").addEventListener("click", rebootNow);

    byId("factoryReset").addEventListener("click", async () => {
      const confirmed = window.confirm(
        "Factory reset Gremlino? Saved Wi-Fi and mode settings will be erased."
      );

      if (!confirmed) return;

      try {
        showToast("Factory reset started");
        await systemAction("factory_reset");
        setTimeout(() => {
          els.live.classList.remove("ok");
          els.liveText.textContent = "Resetting";
        }, 250);
      } catch (error) {
        showToast(error.message || "Factory reset failed", true);
      }
    });

    refresh();
    setInterval(refresh, 1000);
  </script>
</body>
</html>
)GREMLINO";
