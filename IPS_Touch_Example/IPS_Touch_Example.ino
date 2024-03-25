// Example code using Adafruit LCD libraries and Arduino-FT6336U touch library

/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

/* #if defined(ARDUINO_FEATHER_ESP32) // Feather Huzzah32
  #define TFT_CS         14
  #define TFT_RST        15
  #define TFT_DC         32

#elif defined(ESP8266)
  #define TFT_CS         4
  #define TFT_RST        16                                            
  #define TFT_DC         5

#else
  // For the breakout board, you can use any 2 or 3 pins.
  // These pins will also work for the 1.8" TFT shield.
  #define TFT_CS        38
  #define TFT_RST        47 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         1
#endif */
  #define TFT_CS        38
  #define TFT_RST        47 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         1
 
// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique
// to each board and not reassignable. For Arduino Uno: MOSI = pin 11 and
// SCLK = pin 13. This is the fastest mode of operation and is required if
// using the breakout board's microSD card.
/* constexpr const static int8_t pin_mosi = 11;
constexpr const static int8_t pin_sclk = 13; */

// For 1.44" and 1.8" TFT with ST7735 use:
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// For 1.14", 1.3", 1.54", 1.69", and 2.0" TFT with ST7789:
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);


// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
#define TFT_MOSI 11  // Data out
#define TFT_SCLK 13  // Clock out

// For ST7735-based displays, we will use this call
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// OR for the ST7789-based displays, we will use this call
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//Touch includes
#include "FT6336U.h"

#define I2C_SDA 8
#define I2C_SCL 9
#define RST_N_PIN 10
#define INT_N_PIN 2

// Touch library stuff
FT6336U ft6336u(I2C_SDA, I2C_SCL, RST_N_PIN, INT_N_PIN);

bool coordinateMode = false;
uint32_t touchLastMillis = 0;
uint16_t debounceDelay = 100;

uint8_t lastColorDisplay;

uint32_t monitorLastMillis = 0;
uint16_t monitorRefreshRate = 1000;

void setup(void) {
  Serial.begin(115200);
  Serial.print(F("Hello! ST77xx TFT Test"));

  //OR use this initializer (uncomment) if using a 2.0" 320x240 TFT:
  tft.init(240, 320);           // Init ST7789 320x240

  // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
  // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
  // may end up with a black screen some times, or all the time.
  //tft.setSPISpeed(40000000);

  ft6336u.begin();

/*   Serial.print("FT6336U Firmware Version: ");
  Serial.println(ft6336u.read_firmware_id());
  Serial.print("FT6336U Device Mode: ");
  Serial.println(ft6336u.read_device_mode()); */
  
  tft.setRotation(2);
  Serial.println(F("Initialized"));
  tft.fillScreen(ST77XX_BLACK); // Looks like this display has a memory register that holds what was last written to it. Even between uploads! And after being powered off, it displays garbage. So best to clear everything.
  tft.fillRect(0, 80, 120, 80, ST77XX_GREEN);
  tft.fillRect(120, 80, 120, 80, ST77XX_RED);
  tft.fillRect(0, 160, 120, 80, ST77XX_BLUE);
  tft.fillRect(120, 160, 120, 80, ST77XX_YELLOW);
  tft.fillRect(0, 240, 120, 80, ST77XX_ORANGE);
  tft.fillRect(120, 240, 120, 80, ST77XX_MAGENTA);

}

void loop() {
  tft.invertDisplay(true);
/*   delay(500);
  tft.invertDisplay(false);
  delay(500); */


  //Checking for a touch event that triggers an interrupt
  if(digitalRead(INT_N_PIN) != 1) {
      
      // Lots of the original code from the lib example below. Apparently the chip can handle 2 touch points but haven't gotten that running. You can play around with the other features

      //Serial.print("FT6336U TD Status: ");
      //Serial.println(ft6336u.read_td_status());
      //Serial.print("FT6336U Touch Event/ID 1: (");
      //Serial.print(ft6336u.read_touch1_event()); Serial.print(" / "); Serial.print(ft6336u.read_touch1_id()); Serial.println(")");
      Serial.print("FT6336U Touch Position 1: (");
      uint16_t xPos = ft6336u.read_touch1_x();
      uint16_t yPos = ft6336u.read_touch1_y();
      Serial.print(xPos); Serial.print(" , "); Serial.print(yPos); Serial.println(")");
      //Serial.print("FT6336U Touch Weight/MISC 1: (");
      //Serial.print(ft6336u.read_touch1_weight()); Serial.print(" / "); Serial.print(ft6336u.read_touch1_misc()); Serial.println(")");
      //Serial.print("FT6336U Touch Event/ID 2: (");
      //Serial.print(ft6336u.read_touch2_event()); Serial.print(" / "); Serial.print(ft6336u.read_touch2_id()); Serial.println(")");
      Serial.print("FT6336U Touch Position 2: (");
      Serial.print(ft6336u.read_touch2_x()); Serial.print(" , "); Serial.print(ft6336u.read_touch2_y()); Serial.println(")");
      //Serial.print("FT6336U Touch Weight/MISC 2: (");
      //Serial.print(ft6336u.read_touch2_weight()); Serial.print(" / "); Serial.print(ft6336u.read_touch2_misc()); Serial.println(")");
      

        if (xPos < 240 && yPos < 80) {
          if (millis() - touchLastMillis > debounceDelay) {
          coordinateMode = !coordinateMode;
          Serial.println(coordinateMode);
          touchLastMillis = millis();
          displayColor(0);
          lastColorDisplay = 86;
          }
        }
      
      if (coordinateMode == true) {
        displayXY(xPos, yPos);   
      } else {

        if (xPos >= 0 && xPos < 120) {
          if (yPos >= 80 && yPos < 160) {
            displayColor(1);
          } else if (yPos >= 160 && yPos < 240) {
            displayColor(3);
          } else if (yPos >= 240 && yPos < 320) {
            displayColor(5);
          }
        }

        if (xPos >= 120 && xPos < 240) {
          if (yPos >= 80 && yPos < 160) {
            displayColor(2);
          } else if (yPos >= 160 && yPos < 240) {
            displayColor(4);
          } else if (yPos >= 240 && yPos < 320) {
            displayColor(6);
          }
        }
    
      }

      if (coordinateMode == false) {
        if (ft6336u.read_td_status() == 0) {
          if (lastColorDisplay != 0) {
            displayColor(0);
          }
        }
      }
  }

/*   if (millis() - monitorLastMillis > monitorRefreshRate) {
    Serial.print("FT6336U TD Status: ");
    Serial.println(ft6336u.read_td_status());
    monitorLastMillis = millis();
  } */
}

void displayColor(uint8_t code) {
  lastColorDisplay = code;
  tft.setTextColor(0xFFFF, 0x0000); 
  tft.setTextSize(3);
  tft.setCursor(15, 30);
  char buf[13];

  if (code == 0) {
    sprintf(buf, "%-12s", "");
  } else if (code == 1) {
    sprintf(buf, "%-12s", "Green");
  } else if (code == 2) {
    sprintf(buf, "%-12s", "Red");
  } else if (code == 3) {
    sprintf(buf, "%-12s", "Blue");
  } else if (code == 4) {
    sprintf(buf, "%-12s", "Yellow");
  } else if (code == 5) {
    sprintf(buf, "%-12s", "Orange");
  } else if (code == 6) {
    sprintf(buf, "%-12s", "Magenta");
  } 
  tft.print(buf);  
}

void displayXY(uint16_t xPos, uint16_t yPos) {
  //tft.fillRect(0, 0, 240, 80, ST77XX_BLACK);
  tft.setTextColor(0xFFFF, 0x0000); // To minimize flickering of erasing screen, we're using this hack of setting setTextColor(foreground, background). This only works if you have the same length of text. But you can pad things if need be with uneven text
  tft.setTextSize(3);
  tft.setCursor(15, 30);
  tft.print("X=");
  char xBuf[4];
  char yBuf[4];
  sprintf(xBuf, "%-3d", xPos);
  sprintf(yBuf, "%-3d", yPos);
  tft.print(xBuf);
  tft.setCursor(135, 30);
  tft.print("Y=");
  tft.print(yBuf);
}

