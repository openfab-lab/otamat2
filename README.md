## Board
Lilygo TTGO T-display
* Ecran: 135x240
* http://www.lilygo.cn/prod_view.aspx?Id=1126
* https://f6czv.fr/en/documentation-on-esp32-and-the-ttgo-t-display

## EEPROM

* https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
* https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/preferences.html

## External flash

* W25Q32 (32M-bit)

## GPIO 5V tolerant?

I've connected directly the 5V TTL of the BlackBox to pin 36 of the TTGO.

Should be fine as per https://www.letscontrolit.com/forum/viewtopic.php?t=8845

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

## Issues

Does not boot properly when powered via 5V pin, until reset by button
See https://github.com/Xinyuan-LilyGO/TTGO-T-Display/issues/9 but lot of crap in this issue...
* remove capacitor ? (but it's for debouncing the reset button and providing clean reset at power-up... and with capa on button1, it will consider button1 pressed during boot = DOWNLOAD_BOOT?)
* try powering via V_Bus ? (to get serial chip powered)
* maybe board boots but not screen ?

=> cleanest probably is powering via Vbus, to be found...
