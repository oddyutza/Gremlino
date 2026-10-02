# Gremlino 1.1 hardware acceptance

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

- `Gremlino 1.1.0 ready`
- AP name such as `Gremlino-1A2B`
- local IP `192.168.4.1`
- active USB identity strings

## 4. Wi-Fi and captive portal

- join the Gremlino AP using `gremlino!`;
- confirm the captive portal opens;
- otherwise open `http://192.168.4.1/`;
- confirm the UI renders correctly on desktop and phone;
- change SSID/password in the Device section;
- reboot and confirm the new credentials apply.

## 5. Composite USB HID

Keep both automatic modes OFF.

Confirm the host sees a composite device providing:

- USB CDC serial
- HID mouse
- HID keyboard

Confirm the dashboard reaches:

- **USB: Active**
- **HID: Mouse + Keyboard**

Record the actual host-visible VID:PID, manufacturer, product and serial.

## 6. Manual mouse actions

Run:

- **Nudge** — pointer must move and return;
- **Orbit** — pattern must close near its starting position.

## 7. Manual keyboard actions

Use each Mischief Deck test button once and confirm exactly one intended event occurs:

- Space
- Tab
- Page Up
- Page Down
- Home
- End
- Left Arrow
- Right Arrow
- Up Arrow
- Down Arrow
- Caps Lock blink

For Caps Lock blink, verify the LED/state returns to its original state after the paired toggle.

Do not test inside unsaved or sensitive work; use a blank text/editor/browser test page.

## 8. Mischief Deck persistence

- disable several tiles;
- save/reload the page;
- confirm the deck selection persists;
- reboot Gremlino;
- confirm the selection still persists;
- confirm Gremlin Mode itself is OFF after reboot.

## 9. Away Killer

Start with:

- minimum: 20 s
- maximum: 40 s
- amplitude: 2 px

Verify multiple cycles and confirm the next-action countdown updates.

## 10. Gremlin Mode

Test in order:

1. Mild, 15 minutes
2. Spicy, 15 minutes
3. Chaos, 15 minutes

Verify:

- only selected Mischief Deck actions occur;
- Mild emits single actions;
- Spicy occasionally emits a short 2-action burst;
- Chaos can emit 2–3 action bursts;
- the session countdown expires and the mode disables itself.

## 11. Panic and keyboard release

While Gremlin Mode is active:

- tap BOOT;
- both automatic modes must stop;
- no key may remain logically held;
- no further scheduled mouse or keyboard action may occur.

Repeat using **STOP ALL ACTIVITY** in the WebUI.

## 12. Factory reset

Hold BOOT for 7 seconds.

Expected result:

- saved Wi-Fi settings cleared;
- USB identity returns to Gremlino;
- Mischief Deck returns to defaults;
- timing/mode settings return to defaults;
- board reboots;
- default `Gremlino-XXXX` AP and `gremlino!` password return.

## 13. USB identity profiles

For each preset:

- Gremlino
- USB Receiver
- Office Mouse
- Desktop Input

Save, reboot and confirm manufacturer/product strings change while the board VID/PID remains unchanged.

## 14. Record exact hardware behavior

Update the README if the clone differs from the LOLIN profile, including:

- board/model marking;
- observed USB VID/PID;
- actual composite interfaces shown by the host;
- upload procedure;
- BOOT/RESET behavior;
- any native-USB quirk;
- captive-portal observations by OS.
