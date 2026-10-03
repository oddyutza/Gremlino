# Changelog

## 1.1.1 — 2026-10-03

USB identity bugfix.

- disabled Arduino's native-USB auto-start so descriptor configuration can run before `USB.begin()`
- firmware now owns the USB CDC interface explicitly
- custom manufacturer, product and chip-derived serial descriptors now apply at enumeration
- CDC + HID mouse + HID keyboard remain part of the same composite USB device


## 1.1.0 — 2026-10-02

Keyboard mischief and Mischief Deck release.

### Firmware

- added USB HID keyboard alongside HID mouse
- added persistent allowlisted prank mask
- added keyboard pranks: Space, Tab, Page Up, Page Down, Home, End, Left/Right/Up/Down Arrow and paired Caps Lock blink
- added Mild / Spicy / Chaos burst behavior
- added explicit keyboard release on STOP ALL, Gremlin disable and BOOT panic
- Gremlin Mode still always boots OFF
- `/api/action` remains allowlisted; no arbitrary text or keycode injection
- bumped firmware version to 1.1.0

### WebUI

- redesigned Gremlin Mode around a visual **Mischief Deck**
- individual prank tiles can be enabled/disabled
- every tile has a manual test control
- added All / Mouse / Keys quick deck presets
- added live intensity behavior summary
- HID status now reflects composite Mouse + Keyboard operation
- retained Wi-Fi, USB identity, telemetry, reboot and factory-reset controls

### Documentation

- README updated for 1.1 composite HID behavior and safe keyboard action set
- documented prank timing/burst behavior and API limits
- retained PlatformIO and standalone esptool flashing instructions

## 1.0.0 — 2026-10-02

Initial complete software release.

### Firmware

- ESP32-S2 native USB HID mouse support
- Away Killer with random interval and configurable movement amplitude
- mouse-only Gremlin Mode with Mild / Spicy / Chaos scheduling
- manual Nudge and Orbit actions
- persistent Preferences/NVS configuration
- Wi-Fi SoftAP and captive DNS portal
- editable AP SSID/password
- selectable USB identity profiles with stable chip-derived serial
- local status, config, action, network, USB identity and system APIs
- reboot and factory-reset controls
- BOOT tap panic stop
- BOOT 7-second factory-reset recovery
- Gremlin Mode forced OFF after every reboot

### WebUI

- responsive embedded dark UI
- live USB/HID/Wi-Fi/uptime status
- mode controls and timing configuration
- live last-action, next-action and session countdown
- device/network settings and USB identity selector
- firmware version, IP and free-heap telemetry
- reboot and factory-reset flows
- no external assets or cloud dependencies

### Build and documentation

- PlatformIO target pinned to `espressif32@7.1.3`
- CI pinned to Ubuntu 24.04, Python 3.13 and PlatformIO 6.2.0
- standalone flash artifact with esptool instructions
- hardware acceptance checklist
