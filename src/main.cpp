#include <Arduino.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>

#include "USB.h"
#include "USBHID.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"

#include "gremlino_config.h"
#include "web_ui.h"

USBHID Hid;
USBHIDMouse Mouse;
USBHIDKeyboard Keyboard;

DNSServer dnsServer;
WebServer server(GREMLINO_HTTP_PORT);
Preferences prefs;

enum PrankAction : uint16_t {
  PRANK_MOUSE_NUDGE = 1u << 0,
  PRANK_MOUSE_ORBIT = 1u << 1,
  PRANK_SPACE = 1u << 2,
  PRANK_TAB = 1u << 3,
  PRANK_PAGE_UP = 1u << 4,
  PRANK_PAGE_DOWN = 1u << 5,
  PRANK_HOME = 1u << 6,
  PRANK_END = 1u << 7,
  PRANK_LEFT = 1u << 8,
  PRANK_RIGHT = 1u << 9,
  PRANK_UP = 1u << 10,
  PRANK_DOWN = 1u << 11,
  PRANK_CAPS_BLINK = 1u << 12,
};

constexpr uint16_t PRANK_MASK_ALL =
    PRANK_MOUSE_NUDGE | PRANK_MOUSE_ORBIT | PRANK_SPACE | PRANK_TAB |
    PRANK_PAGE_UP | PRANK_PAGE_DOWN | PRANK_HOME | PRANK_END | PRANK_LEFT |
    PRANK_RIGHT | PRANK_UP | PRANK_DOWN | PRANK_CAPS_BLINK;

static const uint16_t PRANK_BITS[] = {
    PRANK_MOUSE_NUDGE, PRANK_MOUSE_ORBIT, PRANK_SPACE,     PRANK_TAB,
    PRANK_PAGE_UP,     PRANK_PAGE_DOWN,   PRANK_HOME,      PRANK_END,
    PRANK_LEFT,        PRANK_RIGHT,       PRANK_UP,        PRANK_DOWN,
    PRANK_CAPS_BLINK,
};

struct Settings {
  bool idleEnabled = false;
  bool gremlinEnabled = false;
  uint16_t minSec = 20;
  uint16_t maxSec = 40;
  uint8_t amplitude = 2;
  uint8_t intensity = 1;
  uint16_t sessionMinutes = 60;
  uint16_t prankMask = PRANK_MASK_ALL;
  String apSsid;
  String apPassword;
  String usbIdentity = "gremlino";
};

Settings settings;

volatile bool usbStarted = false;
volatile bool usbSuspended = false;

uint32_t nextIdleAt = 0;
uint32_t nextGremlinAt = 0;
uint32_t gremlinUntil = 0;
uint32_t restartAt = 0;

bool bootPressed = false;
bool factoryResetTriggered = false;
uint32_t bootPressedAt = 0;

String lastAction = "Boot";

struct UsbIdentityProfile {
  const char *key;
  const char *label;
  const char *manufacturer;
  const char *product;
};

static const UsbIdentityProfile USB_IDENTITIES[] = {
    {"gremlino", "Gremlino", "Gremlino", "Gremlino"},
    {"receiver", "USB Receiver", "Generic", "USB Receiver"},
    {"office_mouse", "Office Mouse", "Generic", "Office Mouse"},
    {"desktop_input", "Desktop Input", "Generic", "Desktop Input Device"},
};

static const UsbIdentityProfile &usbIdentityProfile(const String &key) {
  for (const auto &profile : USB_IDENTITIES) {
    if (key == profile.key) {
      return profile;
    }
  }
  return USB_IDENTITIES[0];
}

static bool validUsbIdentity(const String &key) {
  for (const auto &profile : USB_IDENTITIES) {
    if (key == profile.key) {
      return true;
    }
  }
  return false;
}

static String usbSerialNumber() {
  char serial[13];
  const uint64_t chipId = ESP.getEfuseMac();
  snprintf(serial, sizeof(serial), "%012llX",
           static_cast<unsigned long long>(chipId & 0xFFFFFFFFFFFFULL));
  return String(serial);
}

static bool reached(uint32_t now, uint32_t target) {
  return target != 0 && static_cast<int32_t>(now - target) >= 0;
}

static String defaultApSsid() {
  char suffix[5];
  const uint64_t chipId = ESP.getEfuseMac();
  snprintf(suffix, sizeof(suffix), "%04X",
           static_cast<uint16_t>(chipId & 0xFFFFU));

  String ssid = GREMLINO_AP_PREFIX;
  ssid += "-";
  ssid += suffix;
  return ssid;
}

static bool validSsid(const String &ssid) {
  return ssid.length() >= 1 && ssid.length() <= 32;
}

static bool validPassword(const String &password) {
  return password.length() >= 8 && password.length() <= 63;
}

static String jsonEscape(const String &input) {
  String out;
  out.reserve(input.length() + 8);

  for (size_t i = 0; i < input.length(); ++i) {
    const char c = input.charAt(i);
    switch (c) {
      case '\\':
        out += "\\\\";
        break;
      case '"':
        out += "\\\"";
        break;
      case '\n':
        out += "\\n";
        break;
      case '\r':
        out += "\\r";
        break;
      case '\t':
        out += "\\t";
        break;
      default:
        if (static_cast<uint8_t>(c) >= 0x20) {
          out += c;
        }
        break;
    }
  }

  return out;
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

  return millis() +
         static_cast<uint32_t>(random(static_cast<long>(minMs),
                                      static_cast<long>(maxMs + 1UL)));
}

static void saveSettings() {
  prefs.putBool("idle", settings.idleEnabled);
  prefs.putUShort("minsec", settings.minSec);
  prefs.putUShort("maxsec", settings.maxSec);
  prefs.putUChar("amp", settings.amplitude);
  prefs.putUChar("level", settings.intensity);
  prefs.putUShort("session", settings.sessionMinutes);
  prefs.putUShort("pranks", settings.prankMask);
  prefs.putString("ssid", settings.apSsid);
  prefs.putString("pass", settings.apPassword);
  prefs.putString("usbprof", settings.usbIdentity);
}

static void loadSettings() {
  settings.idleEnabled = prefs.getBool("idle", false);
  settings.gremlinEnabled = false;  // Never restore Gremlin Mode after reboot.
  settings.minSec = constrain(prefs.getUShort("minsec", 20), 5, 300);
  settings.maxSec = constrain(prefs.getUShort("maxsec", 40), 5, 300);
  settings.amplitude = constrain(prefs.getUChar("amp", 2), 1, 8);
  settings.intensity = constrain(prefs.getUChar("level", 1), 1, 3);
  settings.prankMask = prefs.getUShort("pranks", PRANK_MASK_ALL) & PRANK_MASK_ALL;

  const uint16_t storedSession = prefs.getUShort("session", 60);
  settings.sessionMinutes =
      (storedSession == 0 || storedSession == 15 || storedSession == 60 ||
       storedSession == 240)
          ? storedSession
          : 60;

  if (settings.maxSec < settings.minSec) {
    settings.maxSec = settings.minSec;
  }

  settings.apSsid = prefs.getString("ssid", defaultApSsid());
  settings.apPassword = prefs.getString("pass", GREMLINO_AP_PASSWORD);
  settings.usbIdentity = prefs.getString("usbprof", "gremlino");

  if (!validUsbIdentity(settings.usbIdentity)) {
    settings.usbIdentity = "gremlino";
  }
  if (!validSsid(settings.apSsid)) {
    settings.apSsid = defaultApSsid();
  }
  if (!validPassword(settings.apPassword)) {
    settings.apPassword = GREMLINO_AP_PASSWORD;
  }
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
  return Hid.ready();
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
  delay(55);
  Mouse.move(0, s, 0);
  delay(55);
  Mouse.move(-s, 0, 0);
  delay(55);
  Mouse.move(0, -s, 0);
  lastAction = "Mouse orbit";
  return true;
}

static bool tapKey(uint8_t key, const char *label) {
  if (!hidReady()) {
    return false;
  }

  Keyboard.press(key);
  delay(static_cast<uint32_t>(random(35, 90)));
  Keyboard.releaseAll();
  lastAction = label;
  return true;
}

static bool capsBlink() {
  if (!hidReady()) {
    return false;
  }

  Keyboard.press(KEY_CAPS_LOCK);
  delay(45);
  Keyboard.releaseAll();
  delay(static_cast<uint32_t>(random(180, 520)));
  Keyboard.press(KEY_CAPS_LOCK);
  delay(45);
  Keyboard.releaseAll();
  lastAction = "Caps Lock blink";
  return true;
}

static bool runPrankBit(uint16_t bit) {
  switch (bit) {
    case PRANK_MOUSE_NUDGE:
      return nudge(static_cast<uint8_t>(3 + settings.intensity * 2));
    case PRANK_MOUSE_ORBIT:
      return orbit(static_cast<uint8_t>(5 + settings.intensity * 3));
    case PRANK_SPACE:
      return tapKey(' ', "Space");
    case PRANK_TAB:
      return tapKey(KEY_TAB, "Tab");
    case PRANK_PAGE_UP:
      return tapKey(KEY_PAGE_UP, "Page Up");
    case PRANK_PAGE_DOWN:
      return tapKey(KEY_PAGE_DOWN, "Page Down");
    case PRANK_HOME:
      return tapKey(KEY_HOME, "Home");
    case PRANK_END:
      return tapKey(KEY_END, "End");
    case PRANK_LEFT:
      return tapKey(KEY_LEFT_ARROW, "Left Arrow");
    case PRANK_RIGHT:
      return tapKey(KEY_RIGHT_ARROW, "Right Arrow");
    case PRANK_UP:
      return tapKey(KEY_UP_ARROW, "Up Arrow");
    case PRANK_DOWN:
      return tapKey(KEY_DOWN_ARROW, "Down Arrow");
    case PRANK_CAPS_BLINK:
      return capsBlink();
    default:
      return false;
  }
}

static uint16_t randomEnabledPrank() {
  uint8_t enabledCount = 0;
  for (const uint16_t bit : PRANK_BITS) {
    if (settings.prankMask & bit) {
      ++enabledCount;
    }
  }

  if (enabledCount == 0) {
    return 0;
  }

  uint8_t pick = static_cast<uint8_t>(random(0, enabledCount));
  for (const uint16_t bit : PRANK_BITS) {
    if (!(settings.prankMask & bit)) {
      continue;
    }
    if (pick == 0) {
      return bit;
    }
    --pick;
  }

  return 0;
}

static void stopAll(const char *reason = "Stopped", bool persist = true) {
  settings.idleEnabled = false;
  settings.gremlinEnabled = false;
  nextIdleAt = 0;
  nextGremlinAt = 0;
  gremlinUntil = 0;
  Keyboard.releaseAll();
  lastAction = reason;

  if (persist) {
    saveSettings();
  }
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
      gremlinUntil =
          millis() +
          static_cast<uint32_t>(settings.sessionMinutes) * 60UL * 1000UL;
    } else {
      gremlinUntil = 0;
    }
    scheduleGremlin();
    lastAction = "Gremlin Mode enabled";
  } else {
    nextGremlinAt = 0;
    gremlinUntil = 0;
    Keyboard.releaseAll();
    lastAction = "Gremlin Mode disabled";
  }
}

static void runGremlinAction() {
  uint8_t burst = 1;

  if (settings.intensity == 2 && random(0, 100) < 24) {
    burst = 2;
  } else if (settings.intensity == 3) {
    const int roll = random(0, 100);
    if (roll < 18) {
      burst = 3;
    } else if (roll < 58) {
      burst = 2;
    }
  }

  for (uint8_t i = 0; i < burst; ++i) {
    const uint16_t bit = randomEnabledPrank();
    if (bit == 0) {
      setGremlinEnabled(false);
      lastAction = "Gremlin deck empty";
      return;
    }

    runPrankBit(bit);

    if (i + 1 < burst) {
      delay(static_cast<uint32_t>(random(180, 650)));
    }
  }
}

static int32_t secondsUntil(uint32_t target) {
  if (target == 0) {
    return -1;
  }

  const uint32_t now = millis();
  if (reached(now, target)) {
    return 0;
  }

  return static_cast<int32_t>((target - now + 999UL) / 1000UL);
}

static int32_t nextActionSeconds() {
  uint32_t next = 0;

  if (settings.idleEnabled && nextIdleAt != 0) {
    next = nextIdleAt;
  }
  if (settings.gremlinEnabled && nextGremlinAt != 0) {
    if (next == 0 || static_cast<int32_t>(nextGremlinAt - next) < 0) {
      next = nextGremlinAt;
    }
  }

  return secondsUntil(next);
}

static int32_t gremlinRemainingSeconds() {
  if (!settings.gremlinEnabled || gremlinUntil == 0) {
    return -1;
  }
  return secondsUntil(gremlinUntil);
}

static void scheduleRestart(const char *reason) {
  stopAll(reason, false);
  restartAt = millis() + GREMLINO_RESTART_DELAY_MS;
}

static void factoryReset() {
  stopAll("Factory reset", false);
  prefs.clear();
  restartAt = millis() + GREMLINO_RESTART_DELAY_MS;
}

static void usbEventCallback(void *arg, esp_event_base_t eventBase,
                             int32_t eventId, void *eventData) {
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
  server.sendHeader("Cache-Control",
                    "no-store, no-cache, must-revalidate, max-age=0");
  server.sendHeader("Pragma", "no-cache");
}

static void serveUi() {
  sendNoCache();
  server.send_P(200, "text/html; charset=utf-8", GREMLINO_INDEX_HTML);
}

static void redirectToUi() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

static void sendStatus() {
  String json;
  json.reserve(1100);

  json += "{";
  json += "\"version\":\"";
  json += GREMLINO_VERSION;
  json += "\"";

  json += ",\"hid_ready\":";
  json += hidReady() ? "true" : "false";
  json += ",\"usb_started\":";
  json += usbStarted ? "true" : "false";
  json += ",\"usb_suspended\":";
  json += usbSuspended ? "true" : "false";
  json += ",\"wifi_clients\":";
  json += String(WiFi.softAPgetStationNum());
  json += ",\"uptime\":";
  json += String(millis() / 1000UL);
  json += ",\"free_heap\":";
  json += String(ESP.getFreeHeap());

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
  json += ",\"prank_mask\":";
  json += String(settings.prankMask);
  json += ",\"next_in\":";
  json += String(nextActionSeconds());
  json += ",\"gremlin_remaining\":";
  json += String(gremlinRemainingSeconds());

  json += ",\"ap_ssid\":\"";
  json += jsonEscape(settings.apSsid);
  json += "\"";

  const UsbIdentityProfile &identity = usbIdentityProfile(settings.usbIdentity);
  json += ",\"usb_identity\":\"";
  json += identity.key;
  json += "\"";
  json += ",\"usb_identity_label\":\"";
  json += identity.label;
  json += "\"";
  json += ",\"usb_manufacturer\":\"";
  json += jsonEscape(USB.manufacturerName());
  json += "\"";
  json += ",\"usb_product\":\"";
  json += jsonEscape(USB.productName());
  json += "\"";
  json += ",\"usb_serial\":\"";
  json += jsonEscape(USB.serialNumber());
  json += "\"";

  char usbId[10];
  snprintf(usbId, sizeof(usbId), "%04X:%04X", USB.VID(), USB.PID());
  json += ",\"usb_vid_pid\":\"";
  json += usbId;
  json += "\"";

  json += ",\"ip\":\"";
  json += WiFi.softAPIP().toString();
  json += "\"";
  json += ",\"restart_pending\":";
  json += restartAt != 0 ? "true" : "false";
  json += ",\"last_action\":\"";
  json += jsonEscape(lastAction);
  json += "\"";
  json += "}";

  sendNoCache();
  server.send(200, "application/json", json);
}

static uint16_t readClampedU16(const char *name, uint16_t current,
                               uint16_t low, uint16_t high) {
  if (!server.hasArg(name)) {
    return current;
  }
  const long value = server.arg(name).toInt();
  return static_cast<uint16_t>(
      constrain(value, static_cast<long>(low), static_cast<long>(high)));
}

static uint8_t readClampedU8(const char *name, uint8_t current, uint8_t low,
                             uint8_t high) {
  if (!server.hasArg(name)) {
    return current;
  }
  const long value = server.arg(name).toInt();
  return static_cast<uint8_t>(
      constrain(value, static_cast<long>(low), static_cast<long>(high)));
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
    if (requested == 0 || requested == 15 || requested == 60 ||
        requested == 240) {
      settings.sessionMinutes = static_cast<uint16_t>(requested);
    }
  }

  if (server.hasArg("prankmask")) {
    settings.prankMask =
        static_cast<uint16_t>(server.arg("prankmask").toInt()) & PRANK_MASK_ALL;
  }

  if (settings.idleEnabled) {
    scheduleIdle();
  }
  if (settings.gremlinEnabled && settings.prankMask == 0) {
    setGremlinEnabled(false);
  } else if (settings.gremlinEnabled) {
    setGremlinEnabled(true);
  }

  saveSettings();
  sendNoCache();
  server.send(200, "application/json", "{\"ok\":true}");
}

static void handleNetwork() {
  if (!server.hasArg("ssid")) {
    server.send(400, "application/json",
                "{\"ok\":false,\"error\":\"Missing SSID\"}");
    return;
  }

  String ssid = server.arg("ssid");
  ssid.trim();

  if (!validSsid(ssid)) {
    server.send(400, "application/json",
                "{\"ok\":false,\"error\":\"SSID must be 1-32 characters\"}");
    return;
  }

  String password = settings.apPassword;
  if (server.hasArg("password") && server.arg("password").length() > 0) {
    password = server.arg("password");
    if (!validPassword(password)) {
      server.send(
          400, "application/json",
          "{\"ok\":false,\"error\":\"Password must be 8-63 characters\"}");
      return;
    }
  }

  settings.apSsid = ssid;
  settings.apPassword = password;
  saveSettings();
  lastAction = "Network settings saved";

  sendNoCache();
  server.send(200, "application/json",
              "{\"ok\":true,\"reboot_required\":true}");
}

static void handleUsbIdentity() {
  if (!server.hasArg("profile")) {
    server.send(400, "application/json",
                "{\"ok\":false,\"error\":\"Missing USB identity profile\"}");
    return;
  }

  const String profile = server.arg("profile");
  if (!validUsbIdentity(profile)) {
    server.send(400, "application/json",
                "{\"ok\":false,\"error\":\"Unknown USB identity profile\"}");
    return;
  }

  settings.usbIdentity = profile;
  saveSettings();
  lastAction = "USB identity saved";

  sendNoCache();
  server.send(200, "application/json",
              "{\"ok\":true,\"reboot_required\":true}");
}

static bool parseEnabled() {
  return server.hasArg("enabled") && server.arg("enabled") == "1";
}

static bool runNamedAction(const String &name) {
  if (name == "nudge") return nudge(settings.amplitude);
  if (name == "orbit") return orbit(static_cast<uint8_t>(5 + settings.intensity * 3));
  if (name == "key_space") return runPrankBit(PRANK_SPACE);
  if (name == "key_tab") return runPrankBit(PRANK_TAB);
  if (name == "key_page_up") return runPrankBit(PRANK_PAGE_UP);
  if (name == "key_page_down") return runPrankBit(PRANK_PAGE_DOWN);
  if (name == "key_home") return runPrankBit(PRANK_HOME);
  if (name == "key_end") return runPrankBit(PRANK_END);
  if (name == "key_left") return runPrankBit(PRANK_LEFT);
  if (name == "key_right") return runPrankBit(PRANK_RIGHT);
  if (name == "key_up") return runPrankBit(PRANK_UP);
  if (name == "key_down") return runPrankBit(PRANK_DOWN);
  if (name == "caps_blink") return runPrankBit(PRANK_CAPS_BLINK);
  return false;
}

static void handleAction() {
  if (!server.hasArg("name")) {
    server.send(400, "application/json",
                "{\"ok\":false,\"error\":\"Missing action\"}");
    return;
  }

  const String name = server.arg("name");

  if (name == "idle") {
    setIdleEnabled(parseEnabled());
  } else if (name == "gremlin") {
    const bool enabled = parseEnabled();
    if (enabled && settings.prankMask == 0) {
      server.send(409, "application/json",
                  "{\"ok\":false,\"error\":\"Select at least one prank\"}");
      return;
    }
    setGremlinEnabled(enabled);
  } else if (name == "stop") {
    stopAll("STOP ALL");
  } else if (runNamedAction(name)) {
    // Manual allowlisted action completed.
  } else {
    if (!hidReady()) {
      server.send(409, "application/json",
                  "{\"ok\":false,\"error\":\"USB HID is not ready\"}");
    } else {
      server.send(400, "application/json",
                  "{\"ok\":false,\"error\":\"Unknown action\"}");
    }
    return;
  }

  sendNoCache();
  server.send(200, "application/json", "{\"ok\":true}");
}

static void handleSystem() {
  if (!server.hasArg("name")) {
    server.send(400, "application/json",
                "{\"ok\":false,\"error\":\"Missing system action\"}");
    return;
  }

  const String name = server.arg("name");
  if (name == "reboot") {
    sendNoCache();
    server.send(200, "application/json", "{\"ok\":true}");
    scheduleRestart("Restart requested");
    return;
  }
  if (name == "factory_reset") {
    sendNoCache();
    server.send(200, "application/json", "{\"ok\":true}");
    factoryReset();
    return;
  }

  server.send(400, "application/json",
              "{\"ok\":false,\"error\":\"Unknown system action\"}");
}

static void setupWebServer() {
  server.on("/", HTTP_GET, serveUi);
  server.on("/api/status", HTTP_GET, sendStatus);
  server.on("/api/config", HTTP_POST, handleConfig);
  server.on("/api/network", HTTP_POST, handleNetwork);
  server.on("/api/usb", HTTP_POST, handleUsbIdentity);
  server.on("/api/action", HTTP_POST, handleAction);
  server.on("/api/system", HTTP_POST, handleSystem);
  server.on("/favicon.ico", HTTP_GET, []() { server.send(204); });

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
  WiFi.mode(WIFI_AP);
  WiFi.setSleep(false);
  WiFi.softAPsetHostname(GREMLINO_HOSTNAME);

  if (!WiFi.softAP(settings.apSsid, settings.apPassword)) {
    settings.apSsid = defaultApSsid();
    settings.apPassword = GREMLINO_AP_PASSWORD;
    saveSettings();
    WiFi.softAP(settings.apSsid, settings.apPassword);
  }

  dnsServer.start(GREMLINO_DNS_PORT, "*", WiFi.softAPIP());
}

static void setupUsb() {
  const UsbIdentityProfile &identity = usbIdentityProfile(settings.usbIdentity);
  const String serial = usbSerialNumber();

  USB.manufacturerName(identity.manufacturer);
  USB.productName(identity.product);
  USB.serialNumber(serial.c_str());
  USB.onEvent(usbEventCallback);

  Mouse.begin();
  Keyboard.begin();
  USB.begin();
}

static void updatePanicButton() {
  const bool pressed = digitalRead(GREMLINO_BOOT_PIN) == LOW;
  const uint32_t now = millis();

  if (pressed && !bootPressed) {
    bootPressed = true;
    factoryResetTriggered = false;
    bootPressedAt = now;
    stopAll("Physical panic button");
  }

  if (pressed && bootPressed && !factoryResetTriggered &&
      static_cast<uint32_t>(now - bootPressedAt) >=
          GREMLINO_FACTORY_RESET_HOLD_MS) {
    factoryResetTriggered = true;
    factoryReset();
  }

  if (!pressed && bootPressed) {
    bootPressed = false;
    factoryResetTriggered = false;
    bootPressedAt = 0;
  }
}

static void updateSchedulers() {
  if (restartAt != 0) {
    return;
  }

  const uint32_t now = millis();

  if (settings.gremlinEnabled && gremlinUntil != 0 &&
      reached(now, gremlinUntil)) {
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

static void updateRestart() {
  if (restartAt != 0 && reached(millis(), restartAt)) {
    delay(25);
    ESP.restart();
  }
}

void setup() {
  pinMode(GREMLINO_BOOT_PIN, INPUT_PULLUP);

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
  Serial.printf("Gremlino %s ready\n", GREMLINO_VERSION);
  Serial.printf("AP: %s\n", settings.apSsid.c_str());
  Serial.printf("IP: %s\n", WiFi.softAPIP().toString().c_str());
  Serial.printf("USB identity: %s / %s / %s\n", USB.manufacturerName(),
                USB.productName(), USB.serialNumber());
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();

  updatePanicButton();
  updateSchedulers();
  updateRestart();

  delay(2);
}
