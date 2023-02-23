# Laser usage counter
## Introduction
This project is a counter to monitor usage of a CO2 laser.
The principle is to count time when the laser is firing, but not when the head is just moving around.
Laser is controlled by a 5V TTL signal at 1 kHz PWM modulated.
We use a button library and its intrinsic debouncing algorithm to know if the laser is firing or not, without being bothered by low PWM duty cycles.
Callbacks linked to button pressed/released will resume/pause a timer.
The timer will call a callback every second to update the display.
When the timer reaches one full minute, its value (in minutes) is written to EEPROM. We don't write more ofter, otherwise it would wear the EEPROM.
## Board
Lilygo TTGO T-display
* Screen: 135x240
* http://www.lilygo.cn/prod_view.aspx?Id=1126
* https://f6czv.fr/en/documentation-on-esp32-and-the-ttgo-t-display

It is based on an Espressif 32 aka ESP32, integrating Wi-Fi and Bluetooth.

## ESP32 EEPROM

* https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
* https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/preferences.html

## T-display External flash

* W25Q32 (32M-bit)

## GPIO 5V tolerant?

We connect directly the 5V TTL of the [Openbuilds BlackBox toolhead](https://docs.openbuilds.com/doku.php?id=docs:blackbox-4x:connect-3-wire-laser) PWM pin to pin 36 of the TTGO.

This should be fine as per https://www.letscontrolit.com/forum/viewtopic.php?t=8845

**Do not use the 4th pin**, it's 0-10V, way too much, unless a resistor divisor is used!
## Arduino Board

There are a few TTGO boards in the ESP32 platforms but not the T-display, neither in Arduino IDE nor in Platform.io.
See
* https://github.com/Xinyuan-LilyGO/TTGO-T-Display#quick-start
* https://github.com/platformio/platform-espressif32/issues/209
* https://github.com/Xinyuan-LilyGO/TTGO-T-Display/issues/61

Current conclusions are

Board (4Mb flash version):
* Arduino-cli: `--fqbn esp32:esp32:esp32:PSRAM=disabled,FlashSize=4M`
* Platform.io: `platform = espressif32`, `board = esp32dev`

**note:** If you use Linux, don't forget to [give permission for communication with USB](https://nicolas.super.site/content/keep-your-system-organized#92f219ffa0bb4effa9111c1a3d8c89d2).


## Libraries

### TFT_eSPI

Lilygo provides a configured fork of TFT_eSPI in its [TTGO-T-Display repository](https://github.com/Xinyuan-LilyGO/TTGO-T-Display/tree/master/). It corresponds to the [following commit](https://github.com/Bodmer/TFT_eSPI/tree/0c49b71dd434b938628377f655acb5186e4c0ddc) of the official Bodmer/TFT_eSPI and the following configuration in `User_Setup_Select.h`:

```diff
-#include <User_Setup.h>           // Default setup is root library folder
+// #include <User_Setup.h>           // Default setup is root library folder
...
-//#include <User_Setups/Setup25_TTGO_T_Display.h>    // Setup file for ESP32 and TTGO T-Display ST7789V SPI bus TFT
+#include <User_Setups/Setup25_TTGO_T_Display.h>    // Setup file for ESP32 and TTGO T-Display ST7789V SPI bus TFT

```

We will use latest official [Bodmer/TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) with TTGO T-display settings from `User_Setups/Setup25_TTGO_T_Display.h` declared via compilation flags, so we don't need to touch the library files and we follow the same method as this [platformio.ini](https://github.com/JakubAndrysek/TTGO_T_Display/blob/master/TTGO_example/platformio.ini) example.

### Button2

Lilygo provides files of Button2 in `TFT_eSPI/examples/FactoryTest` in its [TTGO-T-Display repository](https://github.com/Xinyuan-LilyGO/TTGO-T-Display/tree/master/). It corresponds to [v1.0](https://github.com/LennartHennigs/Button2/releases/tag/v1.0) of the official LennartHennigs/Button2.

We will use latest official [LennartHennigs/Button2](https://github.com/LennartHennigs/Button2).

### TickTwo

We will use [sstaub/TickTwo](https://github.com/sstaub/TickTwo) to control the timer, as it provides convenient functions for ESP32 to suspend/resume the timer and call callbacks at predefined intervals.
## includes

TFT_eSPI includes the Adafruit_GFX but for easy use, we include the `Free_Fonts.h` which defines short names for the fonts.

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

## Platform.io

### Prerequisites

Everything needed is defined in `./platformio.ini`

### Compilation and flashing

```sh
source ~/.platformio/penv/bin/activate
pio run
pio run --target upload
deactivate
```

## Help wanted
It could be cool to make a nice case.
like this one:
![nice case](https://media.printables.com/media/prints/400057/images/3334143_59157810-2976-4578-a8b5-19a95bf47400/thumbs/cover/640x480/jpg/large_display_capture_400057.webp)

check this issue for more details: [https://github.com/openfab-lab/otamat2/issues/1](https://github.com/openfab-lab/otamat2/issues/1)

### Issues

- [Does not boot properly when powered via 5V pin,](https://github.com/openfab-lab/otamat2/issues/2) until reset by button.
- **TODO:** Platformio settings: make sure to disable PSRAM, and set proper flash size
