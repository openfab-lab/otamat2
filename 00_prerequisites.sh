#!/bin/bash

arduino-cli core update-index           --additional-urls https://dl.espressif.com/dl/package_esp32_index.json
arduino-cli core install esp32:esp32    --additional-urls https://dl.espressif.com/dl/package_esp32_index.json

arduino-cli config set library.enable_unsafe_install true
arduino-cli lib install --git-url https://github.com/sstaub/TickTwo

# TFT_eSPI must be installed manually:
#arduino-cli lib install --git-url https://github.com/Xinyuan-LilyGO/TTGO-T-Display
wget https://github.com/Xinyuan-LilyGO/TTGO-T-Display/archive/refs/heads/master.zip
unzip master.zip
cd TTGO-T-Display-master/; zip -r ../TFT_eSPI.zip TFT_eSPI; cd -
rm -rf master.zip TTGO-T-Display-master
arduino-cli lib install --zip-path TFT_eSPI.zip
rm TFT_eSPI.zip
