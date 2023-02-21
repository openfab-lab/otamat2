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

## Arduino Board

There are a few TTGO boards in the ESP32 platforms but not the T-display, neither in Arduino IDE nor in Platform.io.
See
* https://github.com/Xinyuan-LilyGO/TTGO-T-Display#quick-start
* https://github.com/platformio/platform-espressif32/issues/209
* https://github.com/Xinyuan-LilyGO/TTGO-T-Display/issues/61

Current conclusions are

Board (4Mb flash version):
* Arduino-cli: `--fqbn esp32:esp32:esp32:PSRAM=disabled,FlashSize=4M` and compilation flags to configure TFT_eSPI lib
* Platform.io: `platform = espressif32`, `board = esp32dev`
  * Possibly with compilation flags provided in this [platformio.ini](https://github.com/JakubAndrysek/TTGO_T_Display/blob/master/TTGO_example/platformio.ini)
  * TODO: make sure to disable PSRAM, set flash size

## Arduino-cli

### Prerequisites

```sh
$ ./arduino-cli-0setup
```
### Compilation and flashing

```sh
$ ./arduino-cli-compile
$ ./arduino-cli-upload
```
