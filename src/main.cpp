#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Button2.h>
#include <TickTwo.h>
#include <Preferences.h>
#include "Free_Fonts.h"               // Include the header file attached to this sketch
//#include <SPI.h>

#if USER_SETUP_ID != 25
  #error "TFT_eSPI must be configured for TTGO T-display"
#endif

void printCounter();
TickTwo timer1(printCounter, 1000, 0, MILLIS);

#define ADC_EN              14  //ADC_EN is the ADC detection enable port
#define ADC_PIN             34
#define BUTTON_1            35
#define BUTTON_2            0
#define BUTTON_ADC          36

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library
//Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);
Button2 btnadc(BUTTON_ADC);

char buff[512];
int vref = 1100;

void button_init()
{
    btn2.setPressedHandler([](Button2 & b) {
        timer1.resume();
    });
    btn2.setReleasedHandler([](Button2 & b) {
        timer1.pause();
    });
    // Logic is inverted !
    btnadc.setPressedHandler([](Button2 & b) {
        timer1.pause();
    });
    btnadc.setReleasedHandler([](Button2 & b) {
        timer1.resume();
    });
}
/////////////////////////////////////////////////////////////////

void button_loop()
{
//    btn1.loop();
    btn2.loop();
    btnadc.loop();
}

Preferences preferences;
unsigned long min_counter = 0;
unsigned long last_stored_min_counter = 0;

void setup()
{
    preferences.begin("otamat", false); 
    Serial.begin(115200);
    Serial.println("Start");
    timer1.start();
    timer1.pause();
    /*
    ADC_EN is the ADC detection enable port
    If the USB port is used for power supply, it is turned on by default.
    If it is powered by battery, it needs to be set to high level
    */
    pinMode(ADC_EN, OUTPUT);
    digitalWrite(ADC_EN, HIGH);

    tft.init();
    tft.fillScreen(TFT_NAVY); // Clear screen to navy background
    tft.setTextColor(TFT_YELLOW, TFT_NAVY);
    tft.setTextPadding(120);
    tft.setCursor(0, 0);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);
    tft.setRotation(1);
    button_init();
    min_counter = preferences.getULong("min", 15600);
    last_stored_min_counter = min_counter;
    printCounter();
    if (! btnadc.isPressed()) { // Logic is inverted!
        timer1.resume();
    };
}

void printCounter() {
  Serial.print("Counter ");
  unsigned long sec_counter = timer1.counter() + (min_counter * 60);
  int xpos = tft.width() / 2; // Half the screen width
  int ypos = 0;
  tft.setFreeFont(GLCD);
  tft.setTextDatum(TC_DATUM); // Centre text on x,y position
  bool committed = false;
  if (sec_counter/60 > last_stored_min_counter) {
    preferences.putULong("min", sec_counter/60);
    last_stored_min_counter = preferences.getULong("min");
    committed = true;
  }

  String sec_counter_string = String(sec_counter) + " s";
  String min_counter_string = String(sec_counter/60) + " min";
  Serial.println(sec_counter_string);

  ypos = 10;
  tft.drawString(sec_counter_string, xpos, ypos, GFXFF);
  ypos += tft.fontHeight(GFXFF) + 20;
  tft.setFreeFont(FSB12);
  tft.drawString(min_counter_string, xpos, ypos, GFXFF);
  ypos += tft.fontHeight(GFXFF) + 15;
//  String sv = String(analogRead(36));
//  String debug_string = "DBG: " + (committed ? " EEPROM":"*****");
//  tft.setFreeFont(GLCD);
//  tft.drawString(debug_string, xpos, ypos, GFXFF);
}

void loop()
{
    button_loop();
    timer1.update();
}
