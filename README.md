## Board
Lilygo TTGO T-display
* Ecran: 135x240
* http://www.lilygo.cn/prod_view.aspx?Id=1126
* https://f6czv.fr/en/documentation-on-esp32-and-the-ttgo-t-display

## EEPROM

* https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
* https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/preferences.html

## Prerequisites

ESP32
```sh
arduino-cli core update-index           --additional-urls https://dl.espressif.com/dl/package_esp32_index.json
arduino-cli core install esp32:esp32    --additional-urls https://dl.espressif.com/dl/package_esp32_index.json
```

Arduino libs
* https://github.com/Xinyuan-LilyGO/TTGO-T-Display (TFT_eSPI)
* https://github.com/sstaub/TickTwo

```sh
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
```

## compilation and flashing

```sh
arduino-cli compile --fqbn esp32:esp32:esp32:PSRAM=disabled,FlashSize=4M Otamat2 || exit 1
arduino-cli upload  --fqbn esp32:esp32:esp32 --port /dev/ttyUSB0 Otamat2
```

## box STL

https://github.com/Xinyuan-LilyGO/TTGO-T-Display/blob/master/3d_file/README.md
