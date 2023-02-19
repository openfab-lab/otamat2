#!/bin/bash

arduino-cli compile --fqbn esp32:esp32:esp32:PSRAM=disabled,FlashSize=4M Otamat2 || exit 1
arduino-cli upload  --fqbn esp32:esp32:esp32 --port /dev/ttyUSB0 Otamat2
