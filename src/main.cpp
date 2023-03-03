#include <TFT_eSPI.h>
//#include <SPI.h>
#include <Button2.h>
#include <TickTwo.h>
#include <Preferences.h>
#include "Free_Fonts.h"  // Include the header file attached to this sketch

#if USER_SETUP_ID != 25
#error "TFT_eSPI must be configured for TTGO T-display"
#endif

void printCounter();
TickTwo timer1(printCounter, 1000, 0, MILLIS);

#define ADC_EN              14  //ADC_EN is the ADC detection enable port
#define ADC_PIN             34
#define BUTTON_1            35
#define BUTTON_2            0
#define BUTTON_ADC          27

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library
TFT_eSprite img = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object
                                      // the pointer is used by pushSprite() to push it onto the TFT

// For Jpeg Boot logo:
#define USE_DMA
#include "bootlogo.h"
#include <TJpg_Decoder.h>
#ifdef USE_DMA
  uint16_t  dmaBuffer1[16*16]; // Toggle buffer for 16*16 MCU block, 512bytes
  uint16_t  dmaBuffer2[16*16]; // Toggle buffer for 16*16 MCU block, 512bytes
  uint16_t* dmaBufferPtr = dmaBuffer1;
  bool dmaBufferSel = 0;
#endif
// This next function will be called during decoding of the jpeg file to render each
// 16x16 or 8x8 image tile (Minimum Coding Unit) to the TFT.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  if ( y >= tft.height() ) return 0;
#ifdef USE_DMA
  // Double buffering is used, the bitmap is copied to the buffer by pushImageDMA() the
  // bitmap can then be updated by the jpeg decoder while DMA is in progress
  if (dmaBufferSel) dmaBufferPtr = dmaBuffer2;
  else dmaBufferPtr = dmaBuffer1;
  dmaBufferSel = !dmaBufferSel; // Toggle buffer selection
  //  pushImageDMA() will clip the image block at screen boundaries before initiating DMA
  tft.pushImageDMA(x, y, w, h, bitmap, dmaBufferPtr); // Initiate DMA - blocking only if last DMA is not complete
  // The DMA transfer of image block to the TFT is now in progress...
#else
  // Non-DMA blocking alternative
  tft.pushImage(x, y, w, h, bitmap);  // Blocking, so only returns when image block is drawn
#endif
  // Return 1 to decode next block.
  return 1;
}



//Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);
Button2 btnadc(BUTTON_ADC);
char buff[512];
int vref = 1100;

void drawLaser(bool on) {
  // Size of sprite
  #define IWIDTH  20
  #define IHEIGHT 20
  img.setColorDepth(8);
  img.createSprite(IWIDTH, IHEIGHT);
  img.fillSprite(TFT_NAVY);
  img.fillSmoothCircle( IWIDTH/2, IHEIGHT/2, IWIDTH/2-2, on ? TFT_RED : TFT_BLACK);
  // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
  // All navy pixels will not be drawn hence will show as "transparent"
  img.pushSprite(5, 5, TFT_NAVY);
  // Delete sprite to free up the RAM
  img.deleteSprite();
}

void button_init() {
    btn2.setPressedHandler([](Button2 &b) { // *NOPAD*
        timer1.resume();
        drawLaser(1);
    });
    btn2.setReleasedHandler([](Button2 &b) { // *NOPAD*
        timer1.pause();
        drawLaser(0);
    });
    btnadc.setReleasedHandler([](Button2 &b) { // *NOPAD*
        timer1.resume(); // logic is inverted!
        drawLaser(1);
    });
    btnadc.setPressedHandler([](Button2 &b) { // *NOPAD*
        timer1.pause();  // logic is inverted!
        drawLaser(0);
    });
}
/////////////////////////////////////////////////////////////////

void button_loop() {
//    btn1.loop();
    btn2.loop();
    btnadc.loop();
}

Preferences preferences;
unsigned long min_counter = 0;
unsigned long last_stored_min_counter = 0;

void setup() {
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
    digitalWrite(TFT_BL, LOW);

// For Jpeg Boot logo:
#ifdef USE_DMA
    tft.initDMA(); // To use SPI DMA you must call initDMA() to setup the DMA engine
#endif
    TJpgDec.setJpgScale(1);
    tft.setSwapBytes(true);
    TJpgDec.setCallback(tft_output);
    // Get the width and height in pixels of the jpeg if you wish:
    uint16_t w = 0, h = 0;
    TJpgDec.getJpgSize(&w, &h, bootlogo, sizeof(bootlogo));
    Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);
    tft.startWrite();
    TJpgDec.drawJpg(0, 0, bootlogo, sizeof(bootlogo));
    tft.endWrite();
    digitalWrite(TFT_BL, HIGH);
    delay(2000);


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
    if (! btnadc.isPressed()) { // logic is inverted!
        timer1.resume();
        drawLaser(1);
    } else {
        drawLaser(0);
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
    if (sec_counter / 60 > last_stored_min_counter) {
        preferences.putULong("min", sec_counter / 60);
        last_stored_min_counter = preferences.getULong("min");
        committed = true;
    }
// Seconds since tube installation
//    String sec_counter_string = String(sec_counter) + " s";
// Seconds since reset
    String sec_counter_string = String(timer1.counter()) + " s";
// Minutes since tube installation
    String min_counter_string = String(sec_counter / 60) + " min";
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

void loop() {
    button_loop();
    timer1.update();
}
