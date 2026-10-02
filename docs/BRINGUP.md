# Gremlino hardware bring-up

Use this checklist for the first ESP32-S2 Mini board.

## 1. Identify the board

Confirm:

- ESP32-S2 MCU
- 4 MB flash
- native USB wired to the USB-C connector
- BOOT button on GPIO0
- PlatformIO profile `lolin_s2_mini` is compatible with the clone

## 2. First flash

Build and upload:

```bash
pio run
pio run -t upload
```

If the board does not enter the bootloader automatically:

1. hold **BOOT**;
2. tap **RESET** or reconnect USB;
3. release **BOOT**;
4. retry the upload.

## 3. Serial sanity

Open:

```bash
pio device monitor
```

Expected boot information includes:

- `Gremlino ready`
- an AP name like `Gremlino-1A2B`
- local IP `192.168.4.1`

## 4. Wi-Fi / UI

- connect to the Gremlino AP;
- default password is `gremlino!`;
- verify the captive portal opens;
- otherwise browse to `http://192.168.4.1/`;
- confirm the dashboard renders correctly on both desktop and phone.

## 5. USB HID

Keep both modes OFF initially.

Confirm the dashboard reaches:

- **USB: Active**
- **HID: Ready**

Then use **Test nudge** once and verify that the pointer moves and returns.

## 6. Away Killer

Start conservatively:

- minimum: 20 s
- maximum: 40 s
- amplitude: 2 px

Verify several cycles before increasing amplitude.

## 7. Panic button

While a mode is active, press BOOT once.

Expected result:

- Away Killer OFF
- Gremlin Mode OFF
- no further scheduled movement

## 8. Gremlin Mode

Test in order:

1. Mild, 15 minutes
2. Spicy, 15 minutes
3. Chaos, only after confirming the first two

Check that every orbit closes and returns the pointer to its starting area.

## 9. Reboot behavior

After a reboot:

- Gremlin Mode must always be OFF;
- Away Killer may restore its previous enabled state and saved interval settings.

## 10. Record the exact clone

Once validated, update the README with:

- board photo/model
- USB VID/PID observed by the host
- exact upload procedure
- any board-specific quirks
