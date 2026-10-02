#include <Arduino.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>

#include "USB.h"
#include "USBHIDMouse.h"

#include "gremlino_config.h"
#include "web_ui.h"

USBHID HID;
USBHIDMouse Mouse;

DNSServer dnsServer;
WebServer server(GREMLINO_HTTP_PORT);
Preferences prefs;

struct Settings {
  bool idleEnabled = false;
  bool gremlinEnabled = false;
  uint16_t minSec = 20;
  uint16_t maxSec = 40;
  uint8_t amplitude = 2;
  uint8_t intensity = 1;
  uint16_t sessionMinutes = 60;
};

Settings settings;

volatile bool usbStarted = false;
volatile bool usbSuspended = false;

uint32_t nextIdleAt = 0;
uint32_t nextGremlinAt = 0;
uint32_t gremlinUntil = 0;

bool previousBootButton = HIGH;
String lastAction = "Boot";
char apSsid[32];

static bool reached(uint32_t now, uint32_t target) {
  return target != 0 && static_cast<int32_t>(now - target) >= 0;
}

static uint32_t secondsFromNow(uint16_t seconds) {
  return millis() + static_cast<uint32_t>(seconds) * 1000UL;
}

static uint32_t randomSecondsFromNow(uint16_t minSec, uint16_t maxSec) {
  if (maxSec < minSec) {
    const uint16_t temp = minSec;
    minSec = maxSec;
    maxSec = temp;
  }

  const uint32_t minMs = static_cast<uint32_t>(minSec) * 1000UL;
  const uint32_t maxMs = static_cast<uint32_t>(maxSec) * 1000UL;

  if (minMs == maxMs) {
    return millis() + minMs;
  }

  return millis() + static_cast<uint32_t>(random(static_cast<long>(minMs), static_cast<long>(maxMs + 1UL)));
}

static void saveSettings() {
  prefs.putBool("idle", settings.idleEnabled);
  prefs.putUShort("minsec", settings.minSec);
  prefs.putUShort("maxsec", settings.maxSec);
  prefs.putUChar("amp", settings.amplitude);
  prefs.putUChar("level", settings.intensity);
  prefs.putUShort("session", settings.sessionMinutes);
}

static void loadSettings() {
  settings.idleEnabled = prefs.getBool("idle", false);
  settings.minSec = constrain(prefs.getUShort("minsec", 20), 5, 300);
  settings.maxSec = constrain(prefs.getUShort("maxsec", 40), 5, 300);
  settings.amplitude = constrain(prefs.getUChar("amp", 2), 1, 8);
  settings.intensity = constrain(prefs.getUChar("level", 1), 1, 3);

  const uint16_t storedSession = prefs.getUShort("session", 60);
  settings.sessionMinutes =
      (storedSession == 0 || storedSession == 15 || storedSession == 60 || storedSession == 240)
          ? storedSession
          : 60;

  if (settings.maxSec < settings.minSec) {
    settings.maxSec = settings.minSec;
  }

  // Deliberately never restore Gremlin Mode after a reboot.
  settings.gremlinEnabled = false;
}

static void scheduleIdle() {
  nextIdleAt = settings.idleEnabled
                   ? randomSecondsFromNow(settings.minSec, settings.maxSec)
                   : 0;
}

static void scheduleGremlin() {
  if (!settings.gremlinEnabled) {
    nextGremlinAt = 0;
    return;
  }

  switch (settings.intensity) {
    case 1:
      nextGremlinAt = randomSecondsFromNow(45, 120);
      break;
    case 2:
      nextGremlinAt = randomSecondsFromNow(20, 75);
      break;
    default:
      nextGremlinAt = randomSecondsFromNow(8, 35);
      break;
  }
}

static bool hidReady() {
  return HID.ready();
}

static bool nudge(uint8_t maxAmplitude) {
  if (!hidReady()) {
    return false;
  }

  const int8_t magnitudeX = static_cast<int8_t>(random(1, maxAmplitude + 1));
  const int8_t magnitudeY = static_cast<int8_t>(random(1, maxAmplitude + 1));
  const int8_t dx = random(0, 2) ? magnitudeX : -magnitudeX;
  const int8_t dy = random(0, 2) ? magnitudeY : -magnitudeY;

  Mouse.move(dx, dy, 0);
  delay(55);
  Mouse.move(-dx, -dy, 0);

  lastAction = "Mouse nudge";
  return true;
}

static bool orbit(uint8_t size) {
  if (!hidReady()) {
    return false;
  }

  const int8_t s = static_cast<int8_t>(constrain(size, 2, 20));

  Mouse.move(s, 0, 0);
  delay(60);
  Mouse.move(0, s, 0);
  delay(60);
  Mouse.move(-s, 0, 0);
  delay(60);
  Mouse.move(0, -s, 0);

  lastAction = "Mouse orbit";
  return true;
}

static void stopAll(const char *reason = "Stopped") {
  settings.idleEnabled = false;
  settings.gremlinEnabled = false;
  nextIdleAt = 0;
  nextGremlinAt = 0;
  gremlinUntil = 0;
  lastAction = reason;
  saveSettings();
}

static void setIdleEnabled(bool enabled) {
  settings.idleEnabled = enabled;
  scheduleIdle();
  lastAction = enabled ? "Away Killer enabled" : "Away Killer disabled";
  saveSettings();
}

static void setGremlinEnabled(bool enabled) {
  settings.gremlinEnabled = enabled;

  if (enabled) {
    if (settings.sessionMinutes > 0) {
      gremlinUntil = millis() + static_cast<uint32_t>(settings.sessionMinutes) * 60UL * 1000UL;
    } else {
      gremlinUntil = 0;
    }
    scheduleGremlin();
    lastAction = "Gremlin Mode enabled";
  } else {
    nextGremlinAt = 0;
    gremlinUntil = 0;
    lastAction = "Gremlin Mode disabled";
  }
}

static void runGremlinAction() {
  const int roll = random(0, 100);

  uint8_t orbitThreshold = 88;
  if (settings.intensity == 2) {
    orbitThreshold = 78;
  } else if (settings.intensity == 3) {
    orbitThreshold = 65;
  }

  if (roll < orbitThreshold) {
    const uint8_t maxAmp = static_cast<uint8_t>(3 + (settings.intensity * 2));
    nudge(maxAmp);
  } else {
    const uint8_t size = static_cast<uint8_t>(5 + (settings.intensity * 3));
    orbit(size);
  }
}

static int32_t nextActionSeconds() {
  const uint32_t now = millis();
  uint32_t next = 0;

  if (settings.idleEnabled && nextIdleAt != 0) {
    next = nextIdleAt;
  }

  if (settings.gremlinEnabled && nextGremlinAt != 0) {
    if (next == 0 || static_cast<int32_t>(nextGremlinAt - next) < 0) {
      next = nextGremlinAt;
    }
  }

  if (next == 0) {
    return -1;
  }

  if (reached(now, next)) {
    return 0;
  }

  return static_cast<int32_t>((next - now + 999UL) / 1000UL);
}

static void usbEventCallback(void *arg, esp_event_base_t eventBase, int32_t eventId, void *eventData) {
  (void)arg;
  (void)eventData;

  if (eventBase != ARDUINO_USB_EVENTS) {
    return;
  }

  switch (eventId) {
    case ARDUINO_USB_STARTED_EVENT:
      usbStarted = true;
      usbSuspended = false;
      break;

    case ARDUINO_USB_STOPPED_EVENT:
      usbStarted = false;
      usbSuspended = false;
      break;

    case ARDUINO_USB_SUSPEND_EVENT:
      usbSuspended = true;
      break;

    case ARDUINO_USB_RESUME_EVENT:
      usbSuspended = false;
      break;

    default:
      break;
  }
}

static void sendNoCache() {
  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
  server.sendHeader("Pragma", "no-cache");
}

static void serveUi() {
  server.send_P(200, "text/html; charset=utf-8", GREMLINO_INDEX_HTML);
}

static void redirectToUi() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

static void sendStatus() {
  String json;
  json.reserve(420);

  json += "{";
  json += "\"hid_ready\":";
  json += hidReady() ? "true" : "false";
  json += ",\"usb_started\":";
  json += usbStarted ? "true" : "false";
  json += ",\"usb_suspended\":";
  json += usbSuspended ? "true" : "false";
  json += ",\"wifi_clients\":";
  json += String(WiFi.softAPgetStationNum());
  json += ",\"uptime\":";
  json += String(millis() / 1000UL);
  json += ",\"idle_enabled\":";
  json += settings.idleEnabled ? "true" : "false";
  json += ",\"gremlin_enabled\":";
  json += settings.gremlinEnabled ? "true" : "false";
  json += ",\"min_sec\":";
  json += String(settings.minSec);
  json += ",\"max_sec\":";
  json += String(settings.maxSec);
  json += ",\"amplitude\":";
  json += String(settings.amplitude);
  json += ",\"intensity\":";
  json += String(settings.intensity);
  json += ",\"session_min\":";
  json += String(settings.sessionMinutes);
  json += ",\"next_in\":";
  json += String(nextActionSeconds());
  json += ",\"last_action\":\"";
  json += lastAction;
  json += "\"}";

  sendNoCache();
  server.send(200, "application/json", json);
}

static uint16_t readClampedU16(const char *name, uint16_t current, uint16_t low, uint16_t high) {
  if (!server.hasArg(name)) {
    return current;
  }

  const long value = server.arg(name).toInt();
  return static_cast<uint16_t>(constrain(value, static_cast<long>(low), static_cast<long>(high)));
}

static uint8_t readClampedU8(const char *name, uint8_t current, uint8_t low, uint8_t high) {
  if (!server.hasArg(name)) {
    return current;
  }

  const long value = server.arg(name).toInt();
  return static_cast<uint8_t>(constrain(value, static_cast<long>(low), static_cast<long>(high)));
}

static void handleConfig() {
  settings.minSec = readClampedU16("min", settings.minSec, 5, 300);
  settings.maxSec = readClampedU16("max", settings.maxSec, 5, 300);

  if (settings.maxSec < settings.minSec) {
    const uint16_t temp = settings.minSec;
    settings.minSec = settings.maxSec;
    settings.maxSec = temp;
  }

  settings.amplitude = readClampedU8("amp", settings.amplitude, 1, 8);
  settings.intensity = readClampedU8("intensity", settings.intensity, 1, 3);

  if (server.hasArg("session")) {
    const int requested = server.arg("session").toInt();
    if (requested == 0 || requested == 15 || requested == 60 || requested == 240) {
      settings.sessionMinutes = static_cast<uint16_t>(requested);
    }
  }

  if (settings.idleEnabled) {
    scheduleIdle();
  }

  if (settings.gremlinEnabled) {
    setGremlinEnabled(true);
  }

  saveSettings();
  sendNoCache();
  server.send(200, "application/json", "{\"ok\":true}");
}

static bool parseEnabled() {
  return server.hasArg("enabled") && server.arg("enabled") == "1";
}

static void handleAction() {
  if (!server.hasArg("name")) {
    server.send(400, "text/plain", "Missing action");
    return;
  }

  const String name = server.arg("name");

  if (name == "idle") {
    setIdleEnabled(parseEnabled());
  } else if (name == "gremlin") {
    setGremlinEnabled(parseEnabled());
  } else if (name == "stop") {
    stopAll("STOP ALL");
  } else if (name == "nudge") {
    if (!nudge(settings.amplitude)) {
      server.send(409, "text/plain", "USB HID is not ready");
      return;
    }
  } else if (name == "orbit") {
    if (!orbit(static_cast<uint8_t>(5 + settings.intensity * 3))) {
      server.send(409, "text/plain", "USB HID is not ready");
      return;
    }
  } else {
    server.send(400, "text/plain", "Unknown action");
    return;
  }

  sendNoCache();
  server.send(200, "application/json", "{\"ok\":true}");
}

static void setupWebServer() {
  server.on("/", HTTP_GET, serveUi);
  server.on("/api/status", HTTP_GET, sendStatus);
  server.on("/api/config", HTTP_POST, handleConfig);
  server.on("/api/action", HTTP_POST, handleAction);

  // Common captive-portal probes.
  server.on("/generate_204", HTTP_GET, serveUi);
  server.on("/hotspot-detect.html", HTTP_GET, serveUi);
  server.on("/canonical.html", HTTP_GET, serveUi);
  server.on("/success.txt", HTTP_GET, serveUi);
  server.on("/ncsi.txt", HTTP_GET, serveUi);
  server.on("/connecttest.txt", HTTP_GET, serveUi);
  server.on("/redirect", HTTP_GET, serveUi);
  server.on("/fwlink", HTTP_GET, serveUi);

  server.onNotFound(redirectToUi);
  server.begin();
}

static void setupAccessPoint() {
  const uint64_t chipId = ESP.getEfuseMac();
  snprintf(apSsid, sizeof(apSsid), "Gremlino-%04X", static_cast<uint16_t>(chipId & 0xFFFFU));

  WiFi.mode(WIFI_AP);
  WiFi.setSleep(false);
  WiFi.softAP(apSsid, GREMLINO_AP_PASSWORD);

  dnsServer.start(GREMLINO_DNS_PORT, "*", WiFi.softAPIP());
}

static void setupUsb() {
  USB.onEvent(usbEventCallback);
  Mouse.begin();
  USB.begin();
}

static void updatePanicButton() {
  const bool current = digitalRead(GREMLINO_BOOT_PIN);

  if (previousBootButton == HIGH && current == LOW) {
    stopAll("Physical panic button");
  }

  previousBootButton = current;
}

static void updateSchedulers() {
  const uint32_t now = millis();

  if (settings.gremlinEnabled && gremlinUntil != 0 && reached(now, gremlinUntil)) {
    setGremlinEnabled(false);
    lastAction = "Gremlin session ended";
  }

  if (settings.idleEnabled && reached(now, nextIdleAt)) {
    if (!nudge(settings.amplitude)) {
      lastAction = "Away Killer waiting for HID";
    }
    scheduleIdle();
  }

  if (settings.gremlinEnabled && reached(now, nextGremlinAt)) {
    if (hidReady()) {
      runGremlinAction();
    } else {
      lastAction = "Gremlin waiting for HID";
    }
    scheduleGremlin();
  }
}

void setup() {
  pinMode(GREMLINO_BOOT_PIN, INPUT_PULLUP);
  previousBootButton = digitalRead(GREMLINO_BOOT_PIN);

  Serial.begin(115200);
  delay(50);

  randomSeed(static_cast<uint32_t>(ESP.getEfuseMac()));

  prefs.begin("gremlino", false);
  loadSettings();

  setupAccessPoint();
  setupWebServer();
  setupUsb();

  if (settings.idleEnabled) {
    scheduleIdle();
  }

  Serial.println();
  Serial.println("Gremlino ready");
  Serial.printf("AP: %s\n", apSsid);
  Serial.printf("IP: %s\n", WiFi.softAPIP().toString().c_str());
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();

  updatePanicButton();
  updateSchedulers();

  delay(2);
}
