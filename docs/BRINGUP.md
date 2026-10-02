# Gremlino 1.0 hardware acceptance

Use this checklist when the ESP32-S2 Mini arrives.

## 1. Identify the board

Confirm:

- ESP32-S2 MCU
- 4 MB flash
- native USB wired to USB-C
- BOOT button on GPIO0
- PlatformIO profile `lolin_s2_mini` is compatible with the clone

## 2. Build and flash

```bash
pio run
pio run -t upload
```

If automatic bootloader entry fails:

1. hold **BOOT**;
2. tap **RESET** or reconnect USB;
3. release **BOOT**;
4. retry upload.

## 3. Serial sanity

```bash
pio device monitor
```

Expected output includes:

- `Gremlino 1.0.0 ready`
- AP name such as `Gremlino-1A2B`
- local IP `192.168.4.1`

## 4. Wi-Fi and captive portal

- join the Gremlino AP using `gremlino!`;
- confirm the captive portal opens;
- otherwise open `http://192.168.4.1/`;
- confirm the UI renders correctly on desktop and phone;
- change SSID/password in the Device section;
- reboot and confirm the new credentials apply.

## 5. USB HID

Keep both automatic modes OFF.

Confirm:

- **USB: Active**
- **HID: Ready**

Run **Test nudge** and verify the pointer moves then returns.

Run **Orbit** and verify the pattern closes near its starting point.

## 6. Away Killer

Start with:

- minimum: 20 s
- maximum: 40 s
- amplitude: 2 px

Verify multiple cycles and confirm the next-action countdown updates.

## 7. Gremlin Mode

Test in order:

1. Mild, 15 minutes
2. Spicy, 15 minutes
3. Chaos, 15 minutes

Confirm the session countdown expires and the mode disables itself.

## 8. Panic and recovery

While a mode is active:

- tap BOOT: both modes must stop;
- hold BOOT for 7 seconds: saved settings must clear and the board must reboot.

After factory reset, confirm the default `Gremlino-XXXX` AP and `gremlino!` password return.

## 9. Reboot behavior

After a normal reboot:

- Gremlin Mode must be OFF;
- Away Killer may restore its saved enabled state;
- timing and network settings must persist.

## 10. Record exact hardware behavior

Update the README if the clone differs from the LOLIN profile, including:

- board/model marking;
- observed USB VID/PID;
- upload procedure;
- BOOT/RESET behavior;
- any native-USB quirk;
- captive-portal observations by OS.
