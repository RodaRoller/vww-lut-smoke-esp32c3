# vww-lut-smoke-esp32c3

Minimal C++ program (Arduino framework) for the ESP32-C3, built and run in the
Wokwi simulator. It is a deterministic smoke test of the toolchain and of the
simulation workflow: fixed input, hand-computed expected result, PASS/FAIL on
the serial monitor and on two LEDs.

Checkpoint **C3-01** of the VWW-LUT thesis work (person detection on ESP32-C3).

**Wokwi project:** https://wokwi.com/projects/480136526006967297

## What it does

The program computes `y = w·x + b` with integer arithmetic and compares it with
a value computed by hand:

| Quantity | Value |
|---|---|
| Input `X` | 3, -1, 4, 2 |
| Weights `W` | 2, 5, -1, 3 |
| Bias `B` | 2 |
| Expected `y` | 3·2 + (-1)·5 + 4·(-1) + 2·3 + 2 = **5** (`Y_EXPECTED`) |
| Expected class (`y > 0`) | **1** (`CLASS_EXPECTED`) |

The test runs once in `setup()`. `loop()` only blinks the status LED.

## Circuit

| GPIO | Component | Meaning |
|---|---|---|
| 4 | green LED + 220 Ω resistor to GND | test passed: slow blink (700 ms on / 700 ms off) |
| 5 | red LED + 220 Ω resistor to GND | test failed: fast blink (150 ms on / 150 ms off) |

Board: ESP32-C3-DevKitM-1 (Wokwi part `board-esp32-c3-devkitm-1`). Serial
monitor connected to TX/RX (UART0) at 115200 baud. GPIO2/8/9 (boot strapping
pins) and GPIO20/21 (serial) are not used on purpose.

## Build and run in Wokwi

No local toolchain is needed: Wokwi compiles the sketch on its servers when
the simulation starts.

### Option A: saved project

1. Open https://wokwi.com/projects/480136526006967297
2. Click the green **Start the simulation** button (Wokwi builds, then runs).
3. Read the serial monitor below the board and compare it with
   [Expected output](#expected-output). The green LED blinks slowly.

### Option B: rebuild from this repository

1. On wokwi.com create a new **ESP32-C3** project with the **Arduino** template.
2. Replace the content of `sketch.ino` with [`sketch.ino`](sketch.ino) from this repository.
3. Replace the content of `diagram.json` with [`diagram.json`](diagram.json) from this repository.
4. Click **Start the simulation**. No extra libraries are required.

## Expected output

Program output (from [`logs/SmokePassed_Output.txt`](logs/SmokePassed_Output.txt)):

```
[env] project=vww-lut-smoke v0.1
[env] arduino-esp32=3.3.12
[env] esp-idf=v5.5.5
[env] chip=ESP32-C3
[env] cplusplus=202002
[test] x=3,-1,4,2
[test] w=2,5,-1,3
[test] b=2
[test] y=5 expected=5 OK
[test] class=1 expected=1 OK
SMOKE TEST PASSED
```

The lines before `[env]` (`ESP-ROM`, `rst`, `load`, `entry` …) are printed by
the chip's boot ROM, not by this program, and may differ between environments.
The version numbers in the `[env]` lines may change if Wokwi updates its online
build service (see [Limitations](#limitations)). The `[test]` lines and the
final line must match exactly: they are identical on every run.

## Negative test

Purpose: show that the check can actually fail.

### Procedure

1. In `sketch.ino` change
   `const int Y_EXPECTED = 5;` to `const int Y_EXPECTED = 6;`
2. Click **Start the simulation**.
3. Expected result (from [`logs/SmokeNOTPassed_Output.txt`](logs/SmokeNOTPassed_Output.txt)):
   ```
   [test] y=5 expected=6 MISMATCH
   [test] class=1 expected=1 OK
   SMOKE TEST FAILED
   ```
   and the red LED blinks fast.

### Restore

1. Set the value back: `const int Y_EXPECTED = 5;`
2. Click **Start the simulation** again and check that the output ends with
   `SMOKE TEST PASSED` and the green LED blinks slowly.
3. Do not save or commit the sketch with `Y_EXPECTED = 6`: the version in this
   repository and in the saved Wokwi project must always contain `5`.

## Versions and commits

| Item | Value |
|---|---|
| Tested code | commit `641cc24` (`sketch.ino` with `Y_EXPECTED = 5`) |
| Logs | commit `2ea6244`, produced by the same sketch as in `641cc24` |
| Circuit | `diagram.json`, unchanged since commit `3e28532` |
| Test date | 2026-11-10 (date of the log commit) |
| Host | Windows 11, Google Chrome |
| Simulator | Wokwi web, online build |
| Framework | Arduino-ESP32 3.3.12 (printed by the firmware) |
| ESP-IDF | v5.5.5 (printed by the firmware) |
| C++ standard | C++20 (`__cplusplus` = 202002) |
| Compiler (GCC) version | not recorded by this version of the sketch |

## Limitations

- **Integer environment and I/O test only.** It verifies build, start-up,
  serial output and GPIO in the simulator. It is not yet a KAN/LUT network,
  it does not perform VWW (Visual Wake Words) person detection, and it is not
  a measurement on real hardware.
- **No hardware measurement.** Nothing was run on a physical board. Wokwi
  timing is not cycle-accurate, so no latency, memory or power figures are
  claimed.
- **Minimal test coverage.** One fixed input vector, integer arithmetic only.
  Floating point (software-emulated on the ESP32-C3, which has no FPU) is not
  exercised.
- **Toolchain not pinned.** The Arduino core is chosen by Wokwi's online build
  service; the versions above are the ones printed on the test date and may
  change. They also differ from the reference firmware of `lut-kan-ids`
  (PlatformIO, official `espressif32` platform: Arduino-ESP32 2.0.17 /
  ESP-IDF 4.4), so code that builds here is not guaranteed to build there.
- **Serial port.** Output was verified only on UART0 in Wokwi. On boards
  without a USB-UART bridge (e.g. ESP32-C3 SuperMini) the build option
  "USB CDC On Boot" must be enabled to read it over USB.

## Files

| File | Content |
|---|---|
| `sketch.ino` | program (test + LED) |
| `diagram.json` | Wokwi circuit |
| `wokwi-project.txt` | link to the Wokwi project (generated by Wokwi) |
| `logs/SmokePassed_Output.txt` | normal run |
| `logs/SmokeNOTPassed_Output.txt` | negative test (`Y_EXPECTED = 6`) |

## Own vs reused

- **Reused:** Wokwi ESP32-C3 Arduino template (`setup()`/`loop()` skeleton,
  board definition); Arduino-ESP32 core library (`Serial`, GPIO, timing).
- **Own:** LED circuit and behaviour (two LEDs, blink timing), output format,
  test runs, logs and documentation.
- **Assistance:** basic project built as a guided exercise with an AI
  assistant. I followed the code step by step and customized the behaviour.
  I also used it to learn the Git workflow.
