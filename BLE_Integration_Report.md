# BLE Integration Report: Merging VibrateBLE into HidogeddonReaderV1

**Date:** 2026-04-07  
**Target Hardware:** ESP32-C3 Super Mini  
**Author Analysis:** A hungry ghost trapped in a jar

---

## 1. Executive Summary

The current architecture uses two separate ESP32 boards: the main HidogeddonReaderV1 (ESP32-C3 Super Mini) and a secondary VibrateBLE board (FireBeetle 2 ESP32-E) that communicates with a FitPro smartwatch. The two boards communicate via a physical GPIO wire — EXT_PIN (GPIO 5) on the main board pulses high to trigger a BLE scan/vibration on the secondary board.

The ESP32-C3 has **built-in BLE 5.0 hardware**, meaning the secondary board is entirely redundant. This report describes how to consolidate both firmware images into a single binary that runs on the ESP32-C3 Super Mini, while keeping total flash and RAM usage within the hardware limits.

---

## 2. Hardware Constraints

| Resource | ESP32-C3 Super Mini | Notes |
|---|---|---|
| Flash | 4MB | Default partition scheme; sufficient |
| SRAM | 400KB | WiFi + BLE coexistence is tight but feasible |
| CPU | Single-core RISC-V @ 160MHz | No dual-core; task scheduling matters |
| BLE | BLE 5.0 (built-in) | Same RF as WiFi; time-multiplexed |
| WiFi | 2.4GHz 802.11 b/g/n | Already in use |

**Coexistence note:** The ESP32-C3 shares a single 2.4GHz RF for both WiFi and BLE. Espressif's SDK includes a hardware coexistence scheduler (`CONFIG_ESP_COEX_SW_COEXIST_ENABLE`) that time-multiplexes the two radios. This is enabled by default in Arduino ESP32 builds and is transparent to application code. Latency on both WiFi and BLE increases slightly but both remain functional.

---

## 3. Current Architecture

```
[Main ESP32-C3]                         [Secondary ESP32-E]
  Wiegand Reader                          BLE Client
  Web Server                              FitPro Watchband
  OLED Display                            
  GPIO 5 (EXT_PIN) ──── wire ──────────▶  GPIO 17 (trigger input)
```

The `/triggerBleScan` web route and special function cards pulse GPIO 5 high for 6 seconds. The secondary board polls GPIO 17 and, when it goes high, sends a BLE vibration command to the FitPro watch.

---

## 4. What the VibrateBLE Firmware Does

Reading `VibrateBLE/hr_ble.cpp` and `VibrateBLE/VibrateBLE.ino`, the BLE logic is compact and straightforward:

### 4.1 BLE Scan Phase
- Uses **NimBLE-Arduino** (not the heavier standard Arduino BLE stack)
- Scans for BLE advertisements containing manufacturer name `"716"` (FitPro identifier)
- On first discovery, saves the device MAC address to NVS preferences under namespace `"hrSettings"` key `"bleMac"`
- Exits scan mode and enters connection mode

### 4.2 BLE Connection Phase
- Connects to the saved MAC using `NimBLEClient`
- Accesses Nordic UART Service UUID `6e400001-b5a3-f393-e0a9-e50e24dcca9d`
- Writes to TX characteristic `6e400002-b5a3-f393-e0a9-e50e24dcca9d`

### 4.3 Vibration Commands
```cpp
// Vibrate command (9 bytes)
uint8_t vibrateCmd[9] = { 205, 0, 6, 18, 1, 11, 0, 1, 1 };

// Stop command (8 bytes)
uint8_t stopCmd[8] = { 220, 0, 5, 21, 1, 0, 20, 1 };
```

### 4.4 Trigger Input
- Polls GPIO 17 every 10ms
- Short press: Send vibrate → delay 500ms → send stop
- Hold >4 seconds (400 × 10ms): Clear saved MAC and reboot (re-scan)

### 4.5 Settings Stored
- `bleMac` (String): MAC address of paired FitPro
- `bleScanMode` (bool): Whether currently scanning

---

## 5. Integration Plan

### 5.1 Library Change

The secondary board uses **NimBLE-Arduino**. This is the correct library to use for the integrated build — it is significantly smaller than the ESP32 Arduino standard BLE library:

| Library | Flash overhead | RAM overhead |
|---|---|---|
| ESP32 Arduino BLE (standard) | ~500KB | ~60KB |
| NimBLE-Arduino | ~100KB | ~25KB |

NimBLE-Arduino must be added to the main project. It is available in the Arduino Library Manager. The ESP32-C3 is fully supported.

**Action:** Add `NimBLE-Arduino` to the project's `lib/` directory (alongside the existing Adafruit and AsyncTCP libraries).

---

### 5.2 Files to Add/Modify

#### New file: `hr_ble.h` / `hr_ble.cpp` (ported from `VibrateBLE/`)

The BLE logic from `VibrateBLE/hr_ble.cpp` can be ported directly into the main project. The key changes required are:

1. **Remove GPIO trigger polling** — the trigger will come from an internal function call, not a physical pin. The `hrBLE::InputTriggerHandler()` function that watches GPIO 17 is replaced by a direct public function `hrBLE::TriggerVibrate()`.

2. **Remove LED feedback tied to GPIO 17 logic** — the main project already has its own LED control via `hrUtil`.

3. **Namespace the BLE MAC setting into the existing preferences namespace** — the main project already uses `"hrSettings"` for NVS. The BLE MAC key `"bleMac"` can live in the same namespace without conflict.

4. **Run BLE in a FreeRTOS task** — the main project already pins the web server to Core 0 using `xTaskCreatePinnedToCore`. Since the ESP32-C3 is single-core, tasks are cooperative/preemptive via the FreeRTOS scheduler. The BLE task should be created similarly, running at a lower priority than the web server task.

**Suggested BLE task structure:**
```
Task: BLETask (stack: 4096 bytes, priority: 1)
  - Runs hrBLE::MaintainConnection() in a loop
  - Monitors a shared volatile flag: hrSettings::bleTriggerPending
  - When flag is set: calls vibrate sequence, clears flag
```

#### Modifications to `hr_settings.h` / `hr_settings.cpp`

Add the following fields to the settings struct/namespace:

```
bool    bleEnabled       — master on/off for BLE feature (default: false, saves RAM if unused)
bool    bleScanMode      — currently scanning for device
String  bleMac           — saved MAC of paired FitPro
volatile bool bleTriggerPending  — inter-task flag set when vibration is requested
```

The `bleEnabled` flag is important: if the user has no FitPro watch, BLE can be disabled entirely via the web UI settings page, preventing the NimBLE stack from initialising and saving ~25KB RAM.

#### Modifications to `hr_util.cpp`

The `CheckTriggerBLEScan()` function currently pulses GPIO 5 (EXT_PIN) high to trigger the external board. This function body should be replaced with:

```
if (hrSettings::bleTriggerPending == false) {
    hrSettings::bleTriggerPending = true;   // signal BLE task
}
```

GPIO 5 (EXT_PIN) is then freed. It can remain defined in hr_settings.h as unused (value 255, matching the existing convention) or repurposed.

#### Modifications to `hr_server.cpp`

The `/triggerBleScan` route handler currently pulses EXT_PIN via `hrUtil`. After integration, it should instead set `hrSettings::bleTriggerPending = true` directly, with no GPIO interaction.

#### Modifications to `HidogeddonReaderV1.ino`

In `setup()`, after the existing initialisations, add:

```
if (hrSettings::bleEnabled) {
    hrBLE::FitProM4Init();    // initialise NimBLE stack
    xTaskCreate(BLETask, "BLE", 4096, NULL, 1, NULL);
}
```

In `loop()`, the `CheckTriggerBLEScan()` call remains but its implementation changes as described above.

---

### 5.3 Settings Page Addition

The web UI settings page (`hr_html.cpp`, the `BuildSettingsPage()` function) should gain two new fields:

- **BLE Enabled** (checkbox): Enables/disables the entire BLE subsystem. When unchecked, `hrBLE::FitProM4Init()` is never called and NimBLE does not load.
- **BLE Re-Scan** (button or checkbox): Clears the saved MAC and sets `bleScanMode = true` on next boot, causing the device to scan for a new FitPro. This replaces the 4-second hold behaviour on the secondary board's trigger pin.

---

### 5.4 GPIO Changes Summary

| GPIO | Before | After |
|---|---|---|
| GPIO 5 (EXT_PIN) | Pulses high to signal secondary board | No longer needed; set to 255 (unused) |
| GPIO 17 (secondary board) | Trigger input from main board | Eliminated (secondary board removed) |

All other GPIO assignments are unchanged.

---

## 6. Memory Budget Estimate

| Component | Flash (approx.) | RAM (approx.) |
|---|---|---|
| Main firmware (current) | ~650KB | ~120KB |
| Adafruit OLED + GFX | ~70KB | ~15KB |
| ESPAsyncWebSrv + AsyncTCP | ~80KB | ~30KB |
| SPIFFS | ~20KB | ~5KB |
| NimBLE-Arduino (added) | ~100KB | ~25KB |
| **Total estimated** | **~920KB** | **~195KB** |
| ESP32-C3 available | 4096KB (flash) | 400KB (SRAM) |
| **Headroom** | **~3176KB flash** | **~205KB RAM** |

These are conservative estimates. The integrated binary should fit comfortably within flash. RAM is the tighter constraint but remains well within limits, especially if `bleEnabled = false` prevents NimBLE from loading when not needed.

---

## 7. BLE + WiFi Coexistence Considerations

- The ESP32-C3 RF coexistence is handled by the SDK and requires no application-level changes.
- BLE scanning is the most RF-intensive BLE operation. Once the FitPro MAC is saved (first boot), scanning is not performed on subsequent boots — the device connects directly. This minimises RF contention.
- The BLE connection is maintained at low duty cycle (the FitPro sets its own connection interval). WiFi throughput for the web UI is largely unaffected during an established BLE connection.
- If scan mode must be active at the same time as the web server, expect slightly increased HTTP response latency (~100ms). This is acceptable for the use case.

---

## 8. Task Architecture on Single-Core ESP32-C3

The ESP32-C3 is single-core. The existing code pins the web server task to Core 0 using `xTaskCreatePinnedToCore(..., 0)`. On a single-core device, this is equivalent to `xTaskCreate` — all tasks run on Core 0.

Recommended task priorities (higher number = higher priority):

| Task | Priority | Notes |
|---|---|---|
| Arduino `loop()` | 1 | Heartbeat, card parsing, trigger flag |
| Web server task | 2 | Already set this way |
| BLE task | 1 | Same priority as loop; non-blocking design |

The BLE task should use `vTaskDelay(pdMS_TO_TICKS(10))` within its loop to yield the CPU, preventing it from starving the web server or Wiegand interrupt handlers.

Wiegand interrupt handlers (`IRAM_ATTR`) are not affected by task priorities — they preempt everything as hardware interrupts.

---

## 9. Step-by-Step Implementation Order

1. **Add NimBLE-Arduino** to `lib/` directory.
2. **Create `hr_ble.h` / `hr_ble.cpp`** by porting from `VibrateBLE/hr_ble.cpp/h`, with GPIO trigger logic replaced by a public `TriggerVibrate()` function.
3. **Update `hr_settings.h`** to add `bleEnabled`, `bleScanMode`, `bleMac`, and `bleTriggerPending`.
4. **Update `hr_settings.cpp`** to load/save the new BLE settings from NVS.
5. **Update `hr_util.cpp`** — change `CheckTriggerBLEScan()` to set the `bleTriggerPending` flag instead of pulsing EXT_PIN.
6. **Update `hr_server.cpp`** — change `/triggerBleScan` handler to set the flag directly.
7. **Update `hr_html.cpp`** — add BLE Enabled toggle and BLE Re-Scan option to the settings page.
8. **Update `HidogeddonReaderV1.ino`** — add conditional BLE init and BLE task creation in `setup()`.
9. **Set EXT_PIN to 255** in `hr_settings.h` to mark it unused.
10. **Test**: Verify BLE scan finds FitPro, vibrate triggers from web UI, WiFi web interface remains responsive during BLE operation.

---

## 10. What Can Be Removed

Once integrated, the following are no longer needed:

- The entire `VibrateBLE/` directory (secondary board firmware)
- GPIO 5 EXT_PIN pulse logic in `hr_util.cpp`
- Physical wire between the two ESP32 boards
- The secondary ESP32-E board itself

The `WiegandSend/` directory is a test utility and is unrelated to this change.

---

## 11. Risk and Mitigations

| Risk | Likelihood | Mitigation |
|---|---|---|
| RAM pressure under heavy WiFi + BLE load | Low | `bleEnabled` flag prevents NimBLE init if unused |
| BLE task starves web server | Low | Use `vTaskDelay(10ms)` in BLE loop; web server priority is higher |
| FitPro compatibility (M4/M5/M6) | None | Identical UUIDs and commands; no hardware change |
| SPIFFS write contention from BLE task | Very Low | BLE task only writes BLE MAC to NVS (separate from SPIFFS card log) |
| Wiegand interrupt timing disruption from BLE | Very Low | Interrupts are `IRAM_ATTR` and hardware-level; not affected by BLE scheduling |

---

## 12. Conclusion

Integrating the VibrateBLE functionality into the main HidogeddonReaderV1 firmware is straightforward and low-risk. The ESP32-C3 Super Mini has all required hardware (BLE 5.0, sufficient flash and RAM), and the NimBLE-Arduino library keeps the BLE footprint small (~100KB flash, ~25KB RAM). The estimated combined binary is under 1MB flash and under 200KB RAM — well within the 4MB / 400KB limits of the hardware.

The key design decisions are:
- Use **NimBLE-Arduino** (not standard BLE) for minimal size
- Use a **volatile shared flag** (`bleTriggerPending`) instead of a GPIO wire for inter-task communication
- Gate NimBLE initialisation behind a **`bleEnabled` setting** so users without a FitPro watch pay no RAM penalty
- Run the BLE logic in a **dedicated low-priority FreeRTOS task** to avoid blocking the web server or card reader
