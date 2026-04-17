
  /*

   ██╗  ██╗██╗███████╗███╗   ███╗ ██████╗ ███████╗
   ██║  ██║██║     ██╝████╗ ████║██╔═══██╗██╔════╝
   ███████║██║  ███╗  ██╔████╔██║██║   ██║███████╗
   ██╔══██║██║██╔══╝  ██║╚██╔╝██║██║   ██║╚════██║
   ██║  ██║██║███████╗██║ ╚═╝ ██║╚██████╔╝███████║
   ╚═╝  ╚═╝╚═╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚══════╝

              — CREATED BY HIKTRON —

   - HIZMOS IS A FLIPPER ZERO REPLICA 'OPEN SOURCE' -
              
             ......... DEVELOPERS ........
            {YOUSSEF I HIKAL && OMAR KAMEL} 
              
              """ MULTI TOOL DEVICE """

              <FOR>

              [*PENTESTERS & EMBED ENGINEERS 
              HOBBYIST , CYBER SECURITY EXPERTS*]

               #-FEATURES:

               1-WIFI ATTACKS
               2-BLE ATTACKS
               3-BAD USB
               4-NFC
               5-INFRARED
               6-SUB-GHZ
               7-GPIO
               8-APPS
               9-SETTINGS
               10-FILES   

               *********NOTE**********
               ********SCREEN CONFIG*******

               UNCOMMENT THIS LINE DOWN IN CODE IF YOU ARE USING 1.3 OLED
               U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
               UNCOMMENT THIS LINE DOWN IN CODE IF YOU ARE USING 0.96 OR 1.54 OLED
               U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);
               ********************************************************************

*/
#include "ELECHOUSE_CC1101_SRC_DRV.h"
#include "animations.h"
#include "mainmenu.h"
#include "dolphinreactions.h"
#include <stdint.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_NeoPixel.h>
#include <SD.h>
#include <Update.h>
#include <vector>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include <BleMouse.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <WiFi.h>
#include <IRremote.h>
#include <Wire.h>


#include <esp_wifi.h>
#include "esp_heap_caps.h"
#include "esp_spi_flash.h"
#include "esp_chip_info.h"
#include "esp_system.h"


USBHIDKeyboard Keyboard;

BleMouse mouse_ble("hizmos", "hizmos", 100);





struct blescanner_Device {
  String name;
  String address;
  int rssi;
  String manufacturer;
  String deviceType;
};

std::vector<blescanner_Device> blescanner_devices;
int blescanner_selectedIndex = 0;
BLEScan* blescanner_pBLEScan;









int wifi_selectedIndex = 0;
int wifi_networkCount = 0;
bool wifi_showInfo = false;








#define LED_PIN    48   
#define LED_COUNT  1    

Adafruit_NeoPixel pixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); 

//  function to set color FOR NEO PIXEL LED (R,G,B) 
void setColor(uint8_t r, uint8_t g, uint8_t b) {
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
}


/*
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
  
 U8G2_R0,
  U8X8_PIN_NONE // Reset pin not in action
 );



*/


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);







#define I2C_SDA 8
#define I2C_SCL 9

// ==== IR Pins ====
#define irsenderpin  35
#define irrecivepin  36

#define SD_DETECT_PIN 0

// nRF24 via FSPI
#define NRF_SCK   18
#define NRF_MISO  16
#define NRF_MOSI  17

// nRF24 via FSPI
#define cc1101_SCK   15
#define cc1101_MISO  3
#define cc1101_MOSI  46


//////////////cc1101(1)//////////
#define CC1101_CS    45
#define CC1101_GDO0  21
#define CC1101_GDO2  47
/////////////cc1101(2)//////////
#define CC1101_2_CS    40
#define CC1101_2_GDO0  41
#define CC1101_2_GDO2  42




// SD Card via HSPI
#define SD_SCK    14
#define SD_MISO   39
#define SD_MOSI   38
#define SD_CS     37

// RF24 Modules
#define CE1_PIN   10
#define CSN1_PIN  11

#define CE2_PIN   12
#define CSN2_PIN  13

// RF24 objects using fspi
SPIClass RADIO_SPI(FSPI);
SPIClass SD_SPI(HSPI);


RF24 radio1(CE1_PIN, CSN1_PIN);
RF24 radio2(CE2_PIN, CSN2_PIN);



// ==== IR ====
IRrecv irrecv(irrecivepin);
IRsend irsend(irsenderpin);
decode_results results;





// Pins for buttons
#define BTN_UP     4
#define BTN_DOWN   5
#define BTN_SELECT 6
#define BTN_BACK   7
#define BTN_LEFT   1
#define BTN_RIGHT  2

// Jamming time per channel (ms)
#define JAM_DURATION 500


const int totalAutoImages = 406;
const int totalManualImages = 11;


int batteryPercent = 87;    // أو من قراءاتك
bool sdOK = true;           // يعتمد على if (SD.begin(...))


///////////////////////////////////////
///////////////////////////////////////
bool inhizmosmenu =0;

int autoImageIndex = 0;
int manualImageIndex = 0;

bool autoMode = true;
unsigned long lastImageChangeTime = 0;
unsigned long lastButtonPressTime = 0;
const unsigned long autoModeTimeout = 120000; //
///////////////////////////////////////////////////////////////
/*







void runLoop(void (*func)()) {
  while (digitalRead(BTN_BACK) == HIGH) {
    func();

   
    if (digitalRead(BTN_SELECT) == LOW) {
      waitForRelease(BTN_SELECT);
    }
  }

  waitForRelease(BTN_BACK);  
}

void waitForRelease(uint8_t pin) {
  while (digitalRead(pin) == LOW);  
}

*/

////////////////////////////////////////////////////////////////////

/////////////////cc1101 vars//////////////

// ============== RF SETTINGS ==============
float currentFreq = 433.92;
float dataRate = 3.79372;  
float deviation = 0.0;    
float rxBW = 325.0;
int powerLevel = 10;


//////////////////////////

///////////////////////////

// Modulation modes
enum ModType { MOD_ASK_OOK = 0, MOD_2FSK = 1, MOD_GFSK = 2, MOD_MSK = 3 };
ModType currentMod = MOD_ASK_OOK; // Default for car remotes

// ============== UI STATE ==============
int mode = -1;  

int menuIndex = 0;
int settingsIndex = 0;

// ============== SD CARD STATE ==============
bool sdCardAvailable = false;
SPIClass hspi(HSPI);  // HSPI for SD card

// ============== BUTTON HANDLING ==============
unsigned long lastButtonPress[4] = {0, 0, 0, 0};
const unsigned long debounceDelay = 150;

bool isButtonPressed(int btn) {
  static int btnMap[] = {BTN_UP, BTN_DOWN, BTN_SELECT, BTN_BACK};
  int idx = -1;
  for (int i = 0; i < 4; i++) {
    if (btnMap[i] == btn) {
      idx = i;
      break;
    }
  }
  if (idx == -1) return false;
  
  if (!digitalRead(btn)) {
    if (millis() - lastButtonPress[idx] > debounceDelay) {
      lastButtonPress[idx] = millis();
      return true;
    }
  }
  return false;
}

// ============== RAW BUFFER (for pulses) ==============
#define RAW_BUF_MAX 512
#define MAX_SLOTS 5

struct RawPulse {
  unsigned long pulses[RAW_BUF_MAX];
  int length;
  bool valid;
  float frequency;
  unsigned long timestamp;
  char protocol[20];  // NEW: Protocol name
  uint32_t code;      // NEW: Decoded code
  int bitCount;       // NEW: Number of bits
};

RawPulse capturedSlots[MAX_SLOTS];
int currentSlot = 0;
int activeSlot = 0;

// For real-time capture
volatile unsigned long lastEdgeTime = 0;
volatile int pulseIndex = 0;
volatile bool capturing = false;
unsigned long capturePulses[RAW_BUF_MAX];

// Helper to get pulseIndex safely
int getPulseIndex() {
  noInterrupts();
  int idx = pulseIndex;
  interrupts();
  return idx;
}

// ============== PROTOCOL DEFINITIONS ==============
// Most common RF protocols and their characteristics
struct ProtocolDef {
  const char* name;
  int shortPulse;    // Short pulse duration (μs)
  int longPulse;     // Long pulse duration (μs)
  int syncPulse;     // Sync pulse duration (μs)
  int tolerance;     // Tolerance percentage
  int minBits;       // Minimum bits
  int maxBits;       // Maximum bits
};

const ProtocolDef PROTOCOLS[] = {
  {"PT2262",     350,  1050,  10500, 30, 24, 24},  // Most common remote chips
  {"EV1527",     350,  1050,  10500, 30, 24, 24},  // Clone of PT2262
  {"HT12E",      450,  900,   9000,  25, 12, 12},  // Holtek encoder
  {"HT6P20B",    450,  900,   9000,  25, 20, 20},  // Holtek 20-bit
  {"Princeton",  350,  1050,  10500, 30, 24, 24},  // Princeton PT2260/2262
  {"SC5262",     500,  1500,  15000, 30, 24, 24},  // Silan SC5262
  {"Came",       320,  640,   12800, 25, 12, 12},  // Came gate remotes
  {"Nice",       700,  1400,  2800,  25, 12, 24},  // Nice Flo series
  {"Chamberlain",200,  400,   1200,  25, 9,  10},  // Garage door openers
  {"Linear",     500,  1500,  4500,  30, 10, 10},  // Linear MegaCode
  {"KeeLoq",     400,  800,   9600,  25, 66, 66},  // KeeLoq encrypted
  {"Somfy RTS",  640,  1280,  2560,  20, 56, 56},  // Somfy shutters
};

const int PROTOCOL_COUNT = 12;

// ============== STATUS ==============
String statusMsg = "";
unsigned long statusMsgTime = 0;

// ============== HELPER FUNCTIONS ==============
void safeDelay(unsigned long ms) {
  unsigned long end = millis() + ms;
  while (millis() < end) delay(1);
}

void setStatusMsg(String msg) {
  statusMsg = msg;
  statusMsgTime = millis();
  Serial.println(msg);
}

void drawStatusBar() {
  if (millis() - statusMsgTime < 2000 && statusMsg.length() > 0) {
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.drawStr(0, 63, statusMsg.c_str());
  }
}

// ============== CC1101 SETUP FOR OOK ==============
void setupOOKMode() {
  Serial.println("Setting up ASK/OOK mode for car remotes...");
  
  ELECHOUSE_cc1101.SetRx();
  ELECHOUSE_cc1101.setMHZ(currentFreq);
  
  // ASK/OOK modulation
  ELECHOUSE_cc1101.setModulation(0); // 0 = ASK/OOK
  
  // Data rate for OOK
  ELECHOUSE_cc1101.setDRate(3.79372);
  
  // No deviation for OOK
  ELECHOUSE_cc1101.setDeviation(0);
  
  // RX Bandwidth
  ELECHOUSE_cc1101.setRxBW(325.0);
  
  // Manchester encoding OFF (car remotes usually don't use it)
  ELECHOUSE_cc1101.setSyncMode(0); // No preamble/sync
  
  // Power
  ELECHOUSE_cc1101.setPA(powerLevel);
  
  Serial.println("OOK mode configured!");
}

void setupFSKMode() {
  Serial.println("Setting up 2-FSK mode...");
  
  ELECHOUSE_cc1101.setModulation(1); // 2-FSK
  ELECHOUSE_cc1101.setDRate(dataRate);
  ELECHOUSE_cc1101.setDeviation(deviation);
  ELECHOUSE_cc1101.setRxBW(rxBW);
  ELECHOUSE_cc1101.setSyncMode(2);
  
  Serial.println("FSK mode configured!");
}

// ============== INTERRUPT FOR PULSE CAPTURE ==============
void IRAM_ATTR pulseISR() {
  unsigned long now = micros();
  if (capturing && pulseIndex < RAW_BUF_MAX) {
    unsigned long duration = now - lastEdgeTime;
    if (duration > 50 && duration < 100000) { // Filter noise
      capturePulses[pulseIndex++] = duration;
    }
    lastEdgeTime = now;
  }
}

enum CCState {
  CC_NOT_INIT,
  CC_OK,
  CC_FAIL
};

enum CCSelect {
  CC_NONE,
  CC_1,
  CC_2
};

CCState cc1State = CC_NOT_INIT;
CCState cc2State = CC_NOT_INIT;

CCSelect activeCC = CC_NONE;

/*
void lazyinit(){
  // Initialize CC1101
  ELECHOUSE_cc1101.setSpiPin(cc1101_SCK, cc1101_MISO, cc1101_MOSI, CC1101_CS);
  ELECHOUSE_cc1101.setGDO(CC1101_GDO0, CC1101_GDO2);
  
  if (ELECHOUSE_cc1101.getCC1101()) {
    Serial.println("CC1101 detected!");
  } else {
    Serial.println("ERROR: CC1101 not found!");
    u8g2.clearBuffer();
    u8g2.drawStr(10, 30, "CC1101 ERROR!");
    u8g2.sendBuffer();
     delay(1000);
  }
  
  ELECHOUSE_cc1101.Init();
  
  // Setup for car remotes (OOK mode)
  setupOOKMode();

  // Setup interrupt for pulse capture
  pinMode(CC1101_GDO0, INPUT);
  attachInterrupt(digitalPinToInterrupt(CC1101_GDO0), pulseISR, CHANGE);

}
*/



bool cc1Inited = false;
bool cc2Inited = false;
void lazyInitCC1101(uint8_t which) {
  struct CCModule {
    uint8_t cs;
    uint8_t gdo0;
    uint8_t gdo2;
    bool *initedFlag;
    const char* name;
  };

  CCModule modules[2] = {
    {CC1101_CS, CC1101_GDO0, CC1101_GDO2, &cc1Inited, "CC1101 #1"},
    {CC1101_2_CS, CC1101_2_GDO0, CC1101_2_GDO2, &cc2Inited, "CC1101 #2"}
  };

  // أولاً، نفك أي interrupt قديم
  detachInterrupt(digitalPinToInterrupt(CC1101_GDO0));
  detachInterrupt(digitalPinToInterrupt(CC1101_2_GDO0));

  // عمل init لكل موديول لو مش متعمل قبل كده
  for (int i = 0; i < 2; i++) {
    if (!*(modules[i].initedFlag)) {
      ELECHOUSE_cc1101.setSpiPin(cc1101_SCK, cc1101_MISO, cc1101_MOSI, modules[i].cs);
      ELECHOUSE_cc1101.setGDO(modules[i].gdo0, modules[i].gdo2);

      if (!ELECHOUSE_cc1101.getCC1101()) {
        Serial.print(modules[i].name);
        Serial.println(" NOT FOUND");
        *(modules[i].initedFlag) = false;
      } else {
        ELECHOUSE_cc1101.Init();
        setupOOKMode();
        pinMode(modules[i].gdo0, INPUT);
        *(modules[i].initedFlag) = true;
      }
    }
  }

  // بعد الـ init، نختار الـ active module
  if (which == 1 || which == 2) {
    CCModule &selected = modules[which - 1];
    ELECHOUSE_cc1101.setSpiPin(cc1101_SCK, cc1101_MISO, cc1101_MOSI, selected.cs);
    ELECHOUSE_cc1101.setGDO(selected.gdo0, selected.gdo2);
    attachInterrupt(digitalPinToInterrupt(selected.gdo0), pulseISR, CHANGE);
  } else {
    Serial.println("Invalid selection");
  }

 
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  for (int i = 0; i < 2; i++) {
    const char* status = (*(modules[i].initedFlag)) ? "INIT OK" : "NOT FOUND";
    if ((i + 1) == which) {
      u8g2.drawStr(0, 15 * (i+1), modules[i].name);
      u8g2.drawStr(80, 15 * (i+1), status);
      u8g2.drawStr(100, 15 * (i+1), "<SELECTED>");
    } else {
      u8g2.drawStr(0, 15 * (i+1), modules[i].name);
      u8g2.drawStr(80, 15 * (i+1), status);
    }
  }
  u8g2.sendBuffer();

  Serial.print("CC1101 ACTIVE: ");
  Serial.println(which);
}






// ====== Device Control Functions ======
void deactivateSD() {
  digitalWrite(SD_CS, HIGH);
}

void deactivateNRF1() {
  digitalWrite(CSN1_PIN, HIGH);
  digitalWrite(CE1_PIN, LOW);
}

void deactivateNRF2() {
  digitalWrite(CSN2_PIN, HIGH);
  digitalWrite(CE2_PIN, LOW);
}


// ====== Device Control Functions ======
void activateSD() {
  digitalWrite(SD_CS, LOW);
}






bool selectPressed() {
  if (digitalRead(BTN_SELECT) == LOW) {
    while (digitalRead(BTN_SELECT) == LOW);  
    return true;
  }
  return false;
}

void runLoop(void (*func)()) {
  unsigned long lastRun = millis();

  while (true) {
   
    if (millis() - lastRun > 130) {
      func();                 
      lastRun = millis();    
    }

    // الخروج بـ BACK
    if (digitalRead(BTN_BACK) == LOW) {
      while (digitalRead(BTN_BACK) == LOW);  
      break;
    }
  }
}












/////////////////////////////////////////////////////////////////////////

void handleButtons() {
  static unsigned long lastInputTime = 0; // 
  bool buttonPressed = false;

  
  if (millis() - lastInputTime > 150) {

    
    if (digitalRead(BTN_UP) == LOW) {
      manualImageIndex = (manualImageIndex - 1 + totalManualImages) % totalManualImages;
      autoMode = false;        
      buttonPressed = true;     
    }

    
    if (digitalRead(BTN_DOWN) == LOW) {
      manualImageIndex = (manualImageIndex + 1) % totalManualImages;
      autoMode = false;
      buttonPressed = true;
    }

    
    if (digitalRead(BTN_LEFT) == LOW) {
      autoMode = false;
      buttonPressed = true;
    }

    
    if (digitalRead(BTN_RIGHT) == LOW) {
      autoMode = false;
      buttonPressed = true;
    }

    
    if (digitalRead(BTN_BACK) == LOW) {
      autoMode = true;         
      buttonPressed = false;    
    }

    if (buttonPressed) {
      lastButtonPressTime = millis();  
    }

    lastInputTime = millis(); 
  }
}


void autoModeCheck() {
  if (!autoMode && millis() - lastButtonPressTime > autoModeTimeout) {
    autoMode = true;
  }
}

void displayImage(const uint8_t* image) {
  u8g2.clearBuffer();
  
  u8g2.drawXBMP(0, 0, 128, 64, image);
  u8g2.sendBuffer();
}

void displaymainanim(const uint8_t* image, int batteryPercent, bool sdOK) {


  static const unsigned char image_Background_bits[] U8X8_PROGMEM = {0xfe,0x01,0x00,0x00,0x00,0x00,0x00,0xe0,0xff,0xff,0xff,0x0f,0x00,0x00,0x00,0x00,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x7d,0x06,0x00,0x00,0x00,0x00,0x00,0x18,0xff,0xb7,0x55,0x31,0x00,0x00,0x00,0x00,0x81,0xfc,0xff,0xff,0xff,0xff,0xff,0x8f,0x00,0x00,0x00,0xe2,0xff,0xff,0xff,0x7f,0x3d,0x01,0x00,0x00,0x00,0x00,0x00,0x40,0xb6,0xea,0xff,0x04,0x00,0x00,0x00,0x80,0x41,0xfe,0xff,0xff,0xaa,0xfe,0xff,0x3f,0x01,0x00,0x00,0xf9,0xff,0xff,0xff,0xab,0x9f,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xf8,0xff,0x7f,0x02,0x00,0x00,0x00,0x80,0x3e,0xff,0xff,0xff,0xff,0x55,0xfd,0x7f,0xfc,0xff,0xff,0x6c,0xff,0xff,0xff,0xb5,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x80,0x01,0x00,0x00,0x00,0x80,0xc0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x03,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0x80,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x01,0x00,0x00,0xfe,0xff,0xff,0xff,0x7f};
static const unsigned char image_Battery_bits[] U8X8_PROGMEM = {0xfe,0xff,0x7f,0x00,0x01,0x00,0x80,0x00,0x01,0x00,0x80,0x03,0x01,0x00,0x80,0x02,0x01,0x00,0x80,0x02,0x01,0x00,0x80,0x03,0x01,0x00,0x80,0x00,0xfe,0xff,0x7f,0x00};
static const unsigned char image_SDcardMounted_bits[] U8X8_PROGMEM = {0xff,0x07,0xff,0x04,0xff,0x07,0xff,0x04,0xff,0x07,0xff,0x04,0xff,0x07,0x67,0x00};




  u8g2.clearBuffer();

  // الخلفية (الأنيميشن)
  u8g2.setDrawColor(1);  // أبيض
  u8g2.drawXBMP(0, 0, 128, 64, image);

  
  u8g2.setDrawColor(2);  // XOR mode

  

  // عرض كل شيء
  u8g2.sendBuffer();
}





void setup() {

  pinMode(BTN_UP, INPUT_PULLUP);
   pinMode(BTN_DOWN, INPUT_PULLUP);
   pinMode(BTN_SELECT, INPUT_PULLUP);
   pinMode(BTN_BACK, INPUT_PULLUP);
   pinMode(BTN_LEFT, INPUT_PULLUP);
   pinMode(BTN_RIGHT, INPUT_PULLUP);
   pinMode(SD_DETECT_PIN, INPUT_PULLUP);

  // Set pin modes
   pinMode(SD_CS, OUTPUT);
   pinMode(CSN1_PIN, OUTPUT);
   pinMode(CSN2_PIN, OUTPUT);
   pinMode(CE1_PIN, OUTPUT);
   pinMode(CE2_PIN, OUTPUT);

   
  IrReceiver.begin(irrecivepin);
  IrSender.begin(irsenderpin);



   deactivateSD();
   deactivateNRF1();
   deactivateNRF2();


   // Start both SPI buses
   RADIO_SPI.begin(NRF_SCK, NRF_MISO, NRF_MOSI); // FSPI for radios
   SD_SPI.begin(SD_SCK, SD_MISO, SD_MOSI);    // HSPI for SD
   




     Serial.begin(9600);
     u8g2.begin();

     pixel.begin();                                         // INITIALIZE NEOPIXEL LED  
     pixel.setBrightness(80);                              // Optional: reduce brightness
     pixel.show();                                         // Initialize all pixels to 'off'
     USB.begin();
     Keyboard.begin();

   

     //////////////////nessesarry to activate modules or activate manually
  
  
  drawstartinfo();
  delay(1000);
  u8g2.clearBuffer();
  displayImage(config2);
  u8g2.sendBuffer();
  delay(1000);
  checksysdevices();
  delay(1000);
  activateSD();
  
}

void loop() {
  handleButtons();
  autoModeCheck();

  setColor(0, 0, 0);
 

  if (autoMode) {
    if (millis() - lastImageChangeTime > 300) {
      autoImageIndex = (autoImageIndex + 1) % totalAutoImages;
      lastImageChangeTime = millis();
    }
     
    displaymainanim(autoImages[autoImageIndex], batteryPercent, sdOK);
  
     
   
  } else {
   handlemainmenu(); 
  }


}
