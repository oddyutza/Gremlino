#pragma once

#include <Arduino.h>

const char GREMLINO_INDEX_HTML[] PROGMEM = R"GREMLINO(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
  <meta name="theme-color" content="#080a0d">
  <title>Gremlino</title>
  <style>
    :root {
      color-scheme: dark;
      --bg:#07090c;
      --panel:#111419;
      --panel2:#151920;
      --field:#0b0e12;
      --line:#252b33;
      --line2:#39424d;
      --text:#f4f7f8;
      --muted:#929ba6;
      --lime:#b7ff42;
      --lime2:#7fd51f;
      --purple:#a77bff;
      --cyan:#6ee7ff;
      --danger:#ff6572;
      --warn:#ffc857;
      --shadow:0 24px 70px rgba(0,0,0,.42);
      --radius:22px;
    }

    * { box-sizing:border-box; }
    html { min-height:100%; }

    body {
      margin:0;
      min-height:100vh;
      font-family:Inter,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif;
      color:var(--text);
      background:
        radial-gradient(900px 520px at 12% -12%,rgba(183,255,66,.12),transparent 60%),
        radial-gradient(760px 560px at 106% 11%,rgba(167,123,255,.12),transparent 58%),
        radial-gradient(620px 440px at 55% 108%,rgba(110,231,255,.045),transparent 64%),
        var(--bg);
    }

    button,input,select { font:inherit; }
    button { -webkit-tap-highlight-color:transparent; }
    button:focus-visible,input:focus-visible,select:focus-visible,.trick:focus-visible {
      outline:2px solid var(--lime);
      outline-offset:2px;
    }

    .shell {
      width:min(1120px,100%);
      margin:0 auto;
      padding:28px 18px 44px;
    }

    header {
      display:flex;
      justify-content:space-between;
      align-items:center;
      gap:18px;
      margin-bottom:22px;
    }

    .brand { display:flex; align-items:center; gap:14px; }
    .mark {
      width:50px;
      height:50px;
      display:grid;
      place-items:center;
      border-radius:15px;
      background:linear-gradient(145deg,var(--lime),#73c913);
      color:#0a0c0e;
      font-size:25px;
      font-weight:950;
      letter-spacing:-1px;
      transform:rotate(-2deg);
      box-shadow:0 10px 32px rgba(183,255,66,.20);
      user-select:none;
    }

    .brand h1 { margin:0; font-size:23px; line-height:1; letter-spacing:-.5px; }
    .brand p { margin:6px 0 0; color:var(--muted); font-size:13px; }

    .live {
      display:flex;
      align-items:center;
      gap:8px;
      padding:9px 12px;
      border:1px solid var(--line);
      border-radius:999px;
      background:rgba(17,20,25,.76);
      color:var(--muted);
      font-size:12px;
      font-weight:800;
      backdrop-filter:blur(10px);
    }

    .dot {
      width:8px;
      height:8px;
      border-radius:50%;
      background:var(--danger);
      box-shadow:0 0 0 5px rgba(255,101,114,.08);
    }
    .live.ok .dot { background:var(--lime); box-shadow:0 0 0 5px rgba(183,255,66,.08); }

    .status {
      display:grid;
      grid-template-columns:repeat(4,1fr);
      gap:10px;
      margin-bottom:12px;
    }

    .stat {
      min-width:0;
      padding:14px 15px;
      border:1px solid var(--line);
      border-radius:16px;
      background:rgba(17,20,25,.80);
      backdrop-filter:blur(10px);
    }
    .stat span {
      display:block;
      margin-bottom:6px;
      color:var(--muted);
      font-size:10px;
      font-weight:800;
      letter-spacing:.10em;
      text-transform:uppercase;
    }
    .stat strong { display:block; font-size:15px; overflow:hidden; white-space:nowrap; text-overflow:ellipsis; }

    .grid {
      display:grid;
      grid-template-columns:minmax(0,.78fr) minmax(0,1.22fr);
      gap:12px;
    }

    .card {
      min-width:0;
      padding:20px;
      border:1px solid var(--line);
      border-radius:var(--radius);
      background:linear-gradient(180deg,rgba(20,24,29,.97),rgba(14,17,21,.97));
      box-shadow:var(--shadow);
    }
    .wide { grid-column:1/-1; }

    .card-head {
      display:flex;
      justify-content:space-between;
      align-items:flex-start;
      gap:14px;
      margin-bottom:18px;
    }

    .eyebrow {
      display:block;
      margin-bottom:7px;
      color:var(--lime);
      font-size:10px;
      font-weight:900;
      letter-spacing:.15em;
      text-transform:uppercase;
    }
    h2 { margin:0; font-size:20px; letter-spacing:-.35px; }
    h3 { margin:0; }
    .sub { margin:7px 0 0; max-width:58ch; color:var(--muted); font-size:13px; line-height:1.5; }

    .switch { position:relative; display:inline-flex; width:52px; height:30px; flex:0 0 auto; }
    .switch input { opacity:0; width:0; height:0; }
    .slider {
      position:absolute;
      inset:0;
      cursor:pointer;
      border:1px solid #343c46;
      border-radius:999px;
      background:#252b33;
      transition:.2s ease;
    }
    .slider:before {
      content:"";
      position:absolute;
      width:22px;
      height:22px;
      top:3px;
      left:3px;
      border-radius:50%;
      background:#dce1e5;
      box-shadow:0 2px 8px rgba(0,0,0,.32);
      transition:.2s ease;
    }
    .switch input:checked + .slider { background:rgba(183,255,66,.17); border-color:rgba(183,255,66,.45); }
    .switch input:checked + .slider:before { transform:translateX(22px); background:var(--lime); }

    .fields { display:grid; grid-template-columns:1fr 1fr; gap:10px; }
    .field { min-width:0; }
    label.caption {
      display:block;
      margin:0 0 7px 2px;
      color:var(--muted);
      font-size:11px;
      font-weight:750;
    }

    input[type="number"],input[type="text"],input[type="password"],select {
      width:100%;
      padding:11px 12px;
      border:1px solid var(--line);
      border-radius:12px;
      outline:none;
      background:var(--field);
      color:var(--text);
      transition:border-color .16s ease,box-shadow .16s ease;
    }
    input::placeholder { color:#5f6872; }
    input:focus,select:focus { border-color:rgba(183,255,66,.52); box-shadow:0 0 0 3px rgba(183,255,66,.07); }

    .range-wrap { margin-top:14px; }
    .range-top { display:flex; justify-content:space-between; margin-bottom:9px; color:var(--muted); font-size:11px; }
    .range-top b { color:var(--text); }
    input[type="range"] { width:100%; accent-color:var(--lime); }

    .segments { display:grid; grid-template-columns:repeat(3,1fr); gap:7px; }
    .seg,.mini {
      border:1px solid var(--line);
      border-radius:11px;
      background:var(--field);
      color:var(--muted);
      cursor:pointer;
      font-weight:850;
      transition:.16s ease;
    }
    .seg { padding:10px 8px; font-size:12px; }
    .mini { padding:7px 9px; font-size:10px; }
    .seg:hover,.mini:hover { color:var(--text); border-color:var(--line2); }
    .seg.active { color:#0b0d10; background:var(--lime); border-color:var(--lime); }

    .mode-summary {
      display:flex;
      align-items:center;
      justify-content:space-between;
      gap:10px;
      margin:9px 0 14px;
      padding:10px 11px;
      border:1px solid var(--line);
      border-radius:12px;
      background:var(--field);
      color:var(--muted);
      font-size:11px;
    }
    .mode-summary strong { color:var(--text); }

    .deck-head {
      display:flex;
      justify-content:space-between;
      align-items:end;
      gap:12px;
      margin:17px 0 9px;
    }
    .deck-title { font-size:12px; font-weight:900; }
    .deck-count { color:var(--muted); font-size:10px; }
    .deck-presets { display:flex; gap:5px; flex-wrap:wrap; justify-content:flex-end; }

    .deck {
      display:grid;
      grid-template-columns:repeat(4,minmax(0,1fr));
      gap:7px;
    }

    .trick {
      position:relative;
      min-height:64px;
      padding:10px 34px 9px 10px;
      border:1px solid var(--line);
      border-radius:13px;
      background:#0c0f13;
      color:var(--muted);
      cursor:pointer;
      text-align:left;
      transition:.16s ease;
      overflow:hidden;
      user-select:none;
    }
    .trick:hover { border-color:var(--line2); transform:translateY(-1px); }
    .trick.active {
      border-color:rgba(183,255,66,.48);
      background:linear-gradient(145deg,rgba(183,255,66,.12),rgba(183,255,66,.035));
      color:var(--text);
    }
    .trick.mouse.active { border-color:rgba(167,123,255,.52); background:linear-gradient(145deg,rgba(167,123,255,.13),rgba(167,123,255,.035)); }
    .trick .icon { display:block; margin-bottom:5px; font-size:16px; line-height:1; }
    .trick .name { display:block; font-size:11px; font-weight:900; white-space:nowrap; overflow:hidden; text-overflow:ellipsis; }
    .trick .kind { display:block; margin-top:2px; color:#68727d; font-size:8px; font-weight:850; letter-spacing:.08em; text-transform:uppercase; }
    .test {
      position:absolute;
      top:8px;
      right:8px;
      width:23px;
      height:23px;
      display:grid;
      place-items:center;
      padding:0;
      border:1px solid var(--line);
      border-radius:8px;
      background:#151920;
      color:#9ba5b0;
      cursor:pointer;
      font-size:9px;
      font-weight:900;
    }
    .test:hover { color:var(--lime); border-color:rgba(183,255,66,.35); }

    .toolbar { display:flex; gap:8px; flex-wrap:wrap; margin-top:16px; }
    .btn {
      padding:10px 13px;
      border:1px solid var(--line);
      border-radius:12px;
      background:#171b21;
      color:var(--text);
      cursor:pointer;
      font-size:12px;
      font-weight:850;
      transition:.16s ease;
    }
    .btn:hover { transform:translateY(-1px); border-color:#404955; }
    .btn:active { transform:translateY(0); }
    .btn.primary { background:var(--lime); color:#0b0d10; border-color:var(--lime); }
    .btn.ghost { background:var(--field); }
    .btn.danger { background:rgba(255,101,114,.10); border-color:rgba(255,101,114,.28); color:#ffabb3; }

    .activity {
      display:grid;
      grid-template-columns:1fr auto auto;
      align-items:center;
      gap:22px;
      padding:15px 16px;
      border:1px solid var(--line);
      border-radius:15px;
      background:var(--field);
    }
    .activity small,.meta small { display:block; margin-bottom:5px; color:var(--muted); font-size:11px; }
    .activity strong,.meta strong { font-size:14px; }
    .activity .right { text-align:right; }

    .panic {
      width:100%;
      min-height:54px;
      margin-top:12px;
      border:1px solid rgba(255,101,114,.35);
      border-radius:15px;
      background:linear-gradient(180deg,rgba(255,101,114,.16),rgba(255,101,114,.09));
      color:#ffb4bb;
      cursor:pointer;
      font-weight:950;
      letter-spacing:.03em;
      transition:.16s ease;
    }
    .panic:hover { border-color:rgba(255,101,114,.65); background:linear-gradient(180deg,rgba(255,101,114,.20),rgba(255,101,114,.11)); }

    .device-grid { display:grid; grid-template-columns:1.15fr 1fr .85fr; gap:18px; }
    .device-pane + .device-pane { border-left:1px solid var(--line); padding-left:18px; }
    .pane-title { margin:0 0 14px; font-size:13px; font-weight:900; }
    .helper { margin:7px 0 0; color:var(--muted); font-size:11px; line-height:1.5; }
    .meta-list { display:grid; grid-template-columns:1fr 1fr; gap:8px; margin-bottom:14px; }
    .meta { min-width:0; padding:11px 12px; border:1px solid var(--line); border-radius:12px; background:var(--field); }
    .meta strong { display:block; white-space:nowrap; overflow:hidden; text-overflow:ellipsis; }

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
    .danger-zone { margin-top:14px; padding-top:14px; border-top:1px solid var(--line); }
    .system-actions { display:grid; gap:8px; }
    .system-actions .btn { width:100%; text-align:left; padding:12px 13px; }

    .foot {
      display:flex;
      justify-content:space-between;
      gap:12px;
      margin-top:18px;
      padding:0 3px;
      color:#707984;
      font-size:11px;
      line-height:1.6;
    }
    .foot span:last-child { text-align:right; }

    .toast {
      position:fixed;
      left:50%;
      bottom:22px;
      z-index:20;
      max-width:calc(100vw - 28px);
      padding:10px 14px;
      border-radius:999px;
      background:#e9edf0;
      color:#0b0d10;
      box-shadow:0 12px 36px rgba(0,0,0,.35);
      opacity:0;
      pointer-events:none;
      transform:translate(-50%,18px);
      transition:.2s ease;
      font-size:12px;
      font-weight:850;
      text-align:center;
    }
    .toast.error { background:#ffccd1; color:#3d090e; }
    .toast.show { opacity:1; transform:translate(-50%,0); }

    @media (max-width:900px) {
      .grid { grid-template-columns:1fr; }
      .wide { grid-column:auto; }
      .device-grid { grid-template-columns:1fr; }
      .device-pane + .device-pane { border-left:0; border-top:1px solid var(--line); padding-left:0; padding-top:18px; }
    }

    @media (max-width:680px) {
      .shell { padding:20px 14px 34px; }
      .status { grid-template-columns:1fr 1fr; }
      .brand p { display:none; }
      .mark { width:44px; height:44px; }
      .deck { grid-template-columns:repeat(3,minmax(0,1fr)); }
      .activity { grid-template-columns:1fr 1fr; }
      .activity > div:first-child { grid-column:1/-1; }
      .activity .right { text-align:left; }
    }

    @media (max-width:440px) {
      header { align-items:flex-start; }
      .live { padding:8px 10px; }
      .card { padding:17px; border-radius:18px; }
      .fields,.meta-list { grid-template-columns:1fr; }
      .deck { grid-template-columns:repeat(2,minmax(0,1fr)); }
      .deck-head { align-items:flex-start; flex-direction:column; }
      .deck-presets { justify-content:flex-start; }
      .foot { display:block; text-align:center; }
      .foot span { display:block; }
      .foot span:last-child { margin-top:3px; text-align:center; }
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
          <p>USB HID mischief appliance · local only</p>
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
            <p class="sub">A tiny reversible mouse nudge at a random interval. Nothing else.</p>
          </div>
          <label class="switch" aria-label="Toggle Away Killer"><input id="idleToggle" type="checkbox"><span class="slider"></span></label>
        </div>

        <div class="fields">
          <div class="field"><label class="caption" for="minSec">Minimum interval</label><input id="minSec" type="number" min="5" max="300" value="20"></div>
          <div class="field"><label class="caption" for="maxSec">Maximum interval</label><input id="maxSec" type="number" min="5" max="300" value="40"></div>
        </div>

        <div class="range-wrap">
          <div class="range-top"><span>Movement</span><b><span id="ampValue">16</span> px</b></div>
          <input id="amplitude" type="range" min="1" max="127" value="16">\n          <div class="range-scale"><span>1 · subtle</span><span>32</span><span>64</span><span>127 · maximum</span></div>
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
            <p class="sub">Random mouse and keyboard annoyances, picked only from your selected deck. No command typing, modifiers or destructive keys.</p>
          </div>
          <label class="switch" aria-label="Toggle Gremlin Mode"><input id="gremlinToggle" type="checkbox"><span class="slider"></span></label>
        </div>

        <div class="fields">
          <div class="field">
            <label class="caption">Intensity</label>
            <div class="segments">
              <button class="seg active" data-intensity="1">Mild</button>
              <button class="seg" data-intensity="2">Spicy</button>
              <button class="seg" data-intensity="3">Chaos</button>
            </div>
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
        </div>

        <div class="mode-summary"><span>Pattern</span><strong id="modeSummary">1 action · every 45–120 sec</strong></div>

        <div class="deck-head">
          <div><div class="deck-title">Mischief Deck</div><div id="deckCount" class="deck-count">13 selected</div></div>
          <div class="deck-presets">
            <button class="mini" data-deck-preset="all">All</button>
            <button class="mini" data-deck-preset="mouse">Mouse</button>
            <button class="mini" data-deck-preset="keys">Keys</button>
          </div>
        </div>
        <div id="deck" class="deck"></div>
      </article>

      <article class="card wide">
        <div class="card-head"><div><span class="eyebrow">Control</span><h2>Current activity</h2></div></div>
        <div class="activity">
          <div><small>Last action</small><strong id="lastAction">Boot</strong></div>
          <div class="right"><small>Next action</small><strong id="nextAction">—</strong></div>
          <div class="right"><small>Gremlin session</small><strong id="sessionLeft">—</strong></div>
        </div>
        <button id="stopAll" class="panic">STOP ALL ACTIVITY</button>
      </article>

      <article class="card wide">
        <div class="card-head">
          <div><span class="eyebrow">Device</span><h2>Gremlino settings</h2><p class="sub">Everything is stored on the board. No cloud, account or external service.</p></div>
        </div>

        <div class="device-grid">
          <section class="device-pane">
            <h3 class="pane-title">Wi-Fi access point</h3>
            <div class="field"><label class="caption" for="ssid">Network name</label><input id="ssid" type="text" maxlength="32" autocomplete="off"></div>
            <div class="field" style="margin-top:10px"><label class="caption" for="apPassword">New password</label><input id="apPassword" type="password" minlength="8" maxlength="63" placeholder="Leave blank to keep current" autocomplete="new-password"></div>
            <p class="helper">Password must contain 8–63 characters. Network changes apply after restart.</p>
            <div class="toolbar"><button id="saveNetwork" class="btn primary">Save network</button></div>
          </section>

          <section class="device-pane">
            <h3 class="pane-title">USB identity</h3>
            <div class="field">
              <label class="caption" for="usbIdentity">Host-visible profile</label>
              <select id="usbIdentity">
                <option value="gremlino">Gremlino</option>
                <option value="receiver">USB Receiver</option>
                <option value="office_mouse">Office Mouse</option>
                <option value="desktop_input">Desktop Input</option>
              </select>
            </div>
            <p class="helper">Changes manufacturer/product strings after restart. VID/PID stay with the board profile.</p>
            <div class="toolbar"><button id="saveUsbIdentity" class="btn primary">Save USB identity</button></div>

            <div class="danger-zone">
              <div class="meta-list">
                <div class="meta"><small>VID:PID</small><strong id="usbVidPid">—</strong></div>
                <div class="meta"><small>Serial</small><strong id="usbSerial">—</strong></div>
                <div class="meta"><small>Manufacturer</small><strong id="usbManufacturer">—</strong></div>
                <div class="meta"><small>Product</small><strong id="usbProduct">—</strong></div>
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
            <div class="system-actions"><button id="reboot" class="btn ghost">Restart Gremlino</button></div>
            <div class="danger-zone">
              <button id="factoryReset" class="btn danger">Factory reset</button>
              <p class="helper">Tap BOOT = panic stop. Hold BOOT 7 sec = factory reset.</p>
            </div>
          </section>
        </div>

        <div id="restartNotice" class="notice">
          Device settings saved. Restart Gremlino to apply network or USB identity changes.
          <div class="toolbar"><button id="restartFromNotice" class="btn ghost">Restart now</button></div>
        </div>
      </article>
    </section>

    <p class="foot"><span>Gremlin Mode always boots OFF · keyboard actions are allowlisted</span><span id="footerVersion">Gremlino</span></p>
  </main>

  <div id="toast" class="toast">Saved</div>

  <script>
    const $ = (id) => document.getElementById(id);

    const PRANKS = [
      {bit:1,    name:"Nudge",      icon:"↗", kind:"mouse", action:"nudge"},
      {bit:2,    name:"Orbit",      icon:"◌", kind:"mouse", action:"orbit"},
      {bit:4,    name:"Space",      icon:"␠", kind:"key",   action:"key_space"},
      {bit:8,    name:"Tab",        icon:"⇥", kind:"key",   action:"key_tab"},
      {bit:16,   name:"Page Up",    icon:"⇞", kind:"key",   action:"key_page_up"},
      {bit:32,   name:"Page Down",  icon:"⇟", kind:"key",   action:"key_page_down"},
      {bit:64,   name:"Home",       icon:"↖", kind:"key",   action:"key_home"},
      {bit:128,  name:"End",        icon:"↘", kind:"key",   action:"key_end"},
      {bit:256,  name:"Left",       icon:"←", kind:"key",   action:"key_left"},
      {bit:512,  name:"Right",      icon:"→", kind:"key",   action:"key_right"},
      {bit:1024, name:"Up",         icon:"↑", kind:"key",   action:"key_up"},
      {bit:2048, name:"Down",       icon:"↓", kind:"key",   action:"key_down"},
      {bit:4096, name:"Caps blink", icon:"A⇧",kind:"key",   action:"caps_blink"}
    ];

    const ALL_MASK = PRANKS.reduce((mask,p) => mask | p.bit, 0);
    const MOUSE_MASK = PRANKS.filter(p => p.kind === "mouse").reduce((mask,p) => mask | p.bit, 0);
    const KEY_MASK = ALL_MASK & ~MOUSE_MASK;

    const els = {
      live:$("live"), liveText:$("liveText"), hid:$("hid"), usb:$("usb"), clients:$("clients"), uptime:$("uptime"),
      idle:$("idleToggle"), gremlin:$("gremlinToggle"), min:$("minSec"), max:$("maxSec"), amp:$("amplitude"), ampValue:$("ampValue"),
      session:$("session"), last:$("lastAction"), next:$("nextAction"), sessionLeft:$("sessionLeft"), modeSummary:$("modeSummary"),
      deck:$("deck"), deckCount:$("deckCount"), ssid:$("ssid"), password:$("apPassword"), usbIdentity:$("usbIdentity"),
      usbVidPid:$("usbVidPid"), usbSerial:$("usbSerial"), usbManufacturer:$("usbManufacturer"), usbProduct:$("usbProduct"),
      version:$("version"), ip:$("ip"), heap:$("heap"), apName:$("apName"), footerVersion:$("footerVersion"),
      restartNotice:$("restartNotice"), toast:$("toast")
    };

    let intensity = 1;
    let prankMask = ALL_MASK;
    let hydrated = false;
    let toastTimer;

    function showToast(message,error) {
      els.toast.textContent = message;
      els.toast.classList.toggle("error",!!error);
      els.toast.classList.add("show");
      clearTimeout(toastTimer);
      toastTimer = setTimeout(() => els.toast.classList.remove("show"),1900);
    }

    function formatDuration(seconds) {
      seconds = Number(seconds);
      if (!Number.isFinite(seconds) || seconds < 0) return "—";
      const d=Math.floor(seconds/86400), h=Math.floor((seconds%86400)/3600), m=Math.floor((seconds%3600)/60), s=Math.floor(seconds%60);
      if (d) return d+"d "+h+"h";
      if (h) return h+"h "+m+"m";
      if (m) return m+"m "+s+"s";
      return s+"s";
    }

    function formatBytes(bytes) {
      const value=Number(bytes||0);
      if (value>=1048576) return (value/1048576).toFixed(1)+" MB";
      if (value>=1024) return Math.round(value/1024)+" KB";
      return value+" B";
    }

    async function request(path,options) {
      const response=await fetch(path,options||{});
      const text=await response.text();
      let data={};
      if (text) { try { data=JSON.parse(text); } catch (_) { data={error:text}; } }
      if (!response.ok) throw new Error(data.error||"Request failed");
      return data;
    }

    async function post(path,data) {
      return request(path,{method:"POST",headers:{"Content-Type":"application/x-www-form-urlencoded"},body:new URLSearchParams(data)});
    }

    function summaryForIntensity() {
      if (intensity===1) return "1 action · every 45–120 sec";
      if (intensity===2) return "1–2 actions · every 20–75 sec";
      return "1–3 actions · every 8–35 sec";
    }

    function setIntensity(value) {
      intensity=Number(value);
      document.querySelectorAll("[data-intensity]").forEach(button => button.classList.toggle("active",Number(button.dataset.intensity)===intensity));
      els.modeSummary.textContent=summaryForIntensity();
    }

    async function togglePrank(prank) {
      prankMask ^= prank.bit;
      renderDeck();
      try { await saveConfig(true); } catch (error) { showToast(error.message||"Could not save deck",true); }
    }

    function renderDeck() {
      els.deck.innerHTML="";
      PRANKS.forEach(prank => {
        const tile=document.createElement("div");
        tile.className="trick "+prank.kind+((prankMask&prank.bit)?" active":"");
        tile.setAttribute("role","button");
        tile.setAttribute("aria-pressed",(prankMask&prank.bit)?"true":"false");
        tile.tabIndex=0;
        tile.innerHTML='<span class="icon">'+prank.icon+'</span><span class="name">'+prank.name+'</span><span class="kind">'+prank.kind+'</span><button type="button" class="test" title="Test '+prank.name+'" aria-label="Test '+prank.name+'">▶</button>';

        tile.addEventListener("click",async event => {
          if (event.target.closest(".test")) return;
          await togglePrank(prank);
        });

        tile.addEventListener("keydown",async event => {
          if (event.target.closest(".test")) return;
          if (event.key!=="Enter" && event.key!==" ") return;
          event.preventDefault();
          await togglePrank(prank);
        });

        tile.querySelector(".test").addEventListener("click",async event => {
          event.stopPropagation();
          try { await runAction(prank.action); } catch (_) {}
        });

        els.deck.appendChild(tile);
      });
      const count=PRANKS.filter(p => prankMask&p.bit).length;
      els.deckCount.textContent=count+" selected";
    }

    async function saveConfig(quiet) {
      let min=Math.max(5,Math.min(300,Number(els.min.value||20)));
      let max=Math.max(5,Math.min(300,Number(els.max.value||40)));
      if (max<min) { const t=min; min=max; max=t; }
      els.min.value=min; els.max.value=max;
      await post("/api/config",{min:min,max:max,amp:els.amp.value,intensity:intensity,session:els.session.value,prankmask:prankMask});
      if (!quiet) showToast("Settings applied");
    }

    async function runAction(name,extra) {
      try {
        await post("/api/action",Object.assign({name:name},extra||{}));
        await refresh();
      } catch (error) {
        showToast(error.message||"Action failed",true);
        throw error;
      }
    }

    async function systemAction(name) { await post("/api/system",{name:name}); }

    async function refresh() {
      try {
        const s=await request("/api/status",{cache:"no-store"});
        els.live.classList.add("ok");
        els.liveText.textContent=s.restart_pending?"Restarting":"Online";
        els.hid.textContent=s.hid_ready?"Mouse + Keyboard":"Waiting";
        els.usb.textContent=s.usb_suspended?"Suspended":(s.usb_started?"Active":"Starting");
        els.clients.textContent=s.wifi_clients;
        els.uptime.textContent=formatDuration(s.uptime);
        els.last.textContent=s.last_action||"—";
        els.next.textContent=formatDuration(s.next_in);
        els.sessionLeft.textContent=formatDuration(s.gremlin_remaining);
        els.idle.checked=!!s.idle_enabled;
        els.gremlin.checked=!!s.gremlin_enabled;
        els.version.textContent="v"+s.version;
        els.footerVersion.textContent="Gremlino v"+s.version;
        els.ip.textContent=s.ip||"—";
        els.heap.textContent=formatBytes(s.free_heap);
        els.apName.textContent=s.ap_ssid||"—";
        els.usbVidPid.textContent=s.usb_vid_pid||"—";
        els.usbSerial.textContent=s.usb_serial||"—";
        els.usbManufacturer.textContent=s.usb_manufacturer||"—";
        els.usbProduct.textContent=s.usb_product||"—";

        if (!hydrated) {
          els.min.value=s.min_sec;
          els.max.value=s.max_sec;
          els.amp.value=s.amplitude;
          els.ampValue.textContent=s.amplitude;
          els.session.value=String(s.session_min);
          els.ssid.value=s.ap_ssid||"";
          els.usbIdentity.value=s.usb_identity||"gremlino";
          prankMask=Number(s.prank_mask)||0;
          setIntensity(s.intensity);
          renderDeck();
          hydrated=true;
        }
      } catch (error) {
        els.live.classList.remove("ok");
        els.liveText.textContent="Offline";
      }
    }

    els.amp.addEventListener("input",() => els.ampValue.textContent=els.amp.value);

    document.querySelectorAll("[data-intensity]").forEach(button => {
      button.addEventListener("click",async () => {
        setIntensity(button.dataset.intensity);
        try { await saveConfig(true); } catch (error) { showToast(error.message||"Could not save",true); }
      });
    });

    document.querySelectorAll("[data-deck-preset]").forEach(button => {
      button.addEventListener("click",async () => {
        const preset=button.dataset.deckPreset;
        prankMask=preset==="mouse"?MOUSE_MASK:(preset==="keys"?KEY_MASK:ALL_MASK);
        renderDeck();
        try { await saveConfig(true); showToast("Deck updated"); } catch (error) { showToast(error.message||"Could not save deck",true); }
      });
    });

    document.querySelectorAll("[data-action]").forEach(button => button.addEventListener("click",() => runAction(button.dataset.action)));

    $("saveConfig").addEventListener("click",async () => {
      try { await saveConfig(false); } catch (error) { showToast(error.message||"Could not save",true); }
    });

    els.idle.addEventListener("change",async () => {
      const wanted=els.idle.checked;
      try { await runAction("idle",{enabled:wanted?1:0}); } catch (_) { els.idle.checked=!wanted; }
    });

    els.gremlin.addEventListener("change",async () => {
      const wanted=els.gremlin.checked;
      try {
        await saveConfig(true);
        await runAction("gremlin",{enabled:wanted?1:0});
      } catch (_) { els.gremlin.checked=!wanted; }
    });

    $("stopAll").addEventListener("click",async () => {
      try { await runAction("stop"); showToast("All activity stopped"); } catch (_) {}
    });

    $("saveNetwork").addEventListener("click",async () => {
      const ssid=els.ssid.value.trim();
      if (!ssid||ssid.length>32) return showToast("SSID must be 1–32 characters",true);
      if (els.password.value&&(els.password.value.length<8||els.password.value.length>63)) return showToast("Password must be 8–63 characters",true);
      try {
        await post("/api/network",{ssid:ssid,password:els.password.value});
        els.password.value="";
        els.restartNotice.classList.add("show");
        showToast("Network settings saved");
      } catch (error) { showToast(error.message||"Could not save network",true); }
    });

    $("saveUsbIdentity").addEventListener("click",async () => {
      try {
        await post("/api/usb",{profile:els.usbIdentity.value});
        els.restartNotice.classList.add("show");
        showToast("USB identity saved");
      } catch (error) { showToast(error.message||"Could not save USB identity",true); }
    });

    async function rebootNow() {
      try {
        showToast("Restarting Gremlino…");
        await systemAction("reboot");
        setTimeout(() => { els.live.classList.remove("ok"); els.liveText.textContent="Restarting"; },250);
      } catch (error) { showToast(error.message||"Restart failed",true); }
    }

    $("reboot").addEventListener("click",rebootNow);
    $("restartFromNotice").addEventListener("click",rebootNow);

    $("factoryReset").addEventListener("click",async () => {
      if (!window.confirm("Factory reset Gremlino? Saved Wi-Fi, USB identity and mode settings will be erased.")) return;
      try {
        showToast("Factory reset started");
        await systemAction("factory_reset");
        setTimeout(() => { els.live.classList.remove("ok"); els.liveText.textContent="Resetting"; },250);
      } catch (error) { showToast(error.message||"Factory reset failed",true); }
    });

    renderDeck();
    refresh();
    setInterval(refresh,1000);
  </script>
</body>
</html>
)GREMLINO";
