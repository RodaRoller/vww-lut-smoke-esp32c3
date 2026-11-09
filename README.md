# vww-lut-smoke-esp32c3

Description: Minimal ESP32-C3 smoke test running in the Wokwi simulator



Link Project:

https://wokwi.com/projects/480136526006967297



What it Does:

Hard-coded vector as a linear combination. The system makes a check between the scalar product and an expected reference value "Y\_EXPECTED=5". If the result is really 5 the test is passed and a green LED flashes slowly, if it has failed a red LED lights up and flashes quickly.



Environment:

Windows 11, Google Chrome, Wokwi web (online build).

Versions printed by the firmware at startup (see `logs/`):

Arduino-ESP32 3.3.12, ESP-IDF v5.5.5, C++20 (`\_\_cplusplus` = 202002)



Notes:

Basic project built as a guided exercise with an AI assistant. I followed the code step by step and customized the behaviour (two LEDs, output format). I also used it to learn the Git workflow.

