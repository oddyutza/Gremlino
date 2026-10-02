# Changelog

## 1.0.0 — 2026-10-02

Initial complete software release.

### Firmware

- ESP32-S2 native USB HID mouse support
- Away Killer with random interval and configurable movement amplitude
- Gremlin Mode with Mild, Spicy and Chaos scheduling profiles
- manual Nudge and Orbit actions
- persistent Preferences/NVS configuration
- Wi-Fi SoftAP and captive DNS portal
- editable AP SSID/password
- local status, config, action, network and system APIs
- reboot and factory-reset controls
- BOOT tap panic stop
- BOOT 7-second factory-reset recovery
- Gremlin Mode forced OFF after every reboot

### WebUI

- responsive embedded dark UI
- live USB/HID/Wi-Fi/uptime status
- mode controls and timing configuration
- live last-action, next-action and session countdown
- device/network settings
- firmware version, IP and free-heap telemetry
- reboot and factory-reset flows
- no external assets or cloud dependencies

### Build and documentation

- PlatformIO target pinned to `espressif32@7.1.3`
- CI pinned to Ubuntu 24.04, Python 3.13 and PlatformIO 6.2.0
- hardware acceptance checklist
- project README and API overview
