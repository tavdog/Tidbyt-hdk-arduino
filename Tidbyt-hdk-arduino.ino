// Tidbyt Arduino Basic.  A simple starter project to get you going with your Tidbyt in the Arduino IDE. Comes with WifiManager

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFiManager.h>

WiFiManager wifiManager;

// Initialize the HUB75 panel.
//HUB75_I2S_CFG::i2s_pins _pins = {21, 2, 22, 23, 4, 27, 26, 5, 25, 18, -1, 19, 32, 33}; // newer model
HUB75_I2S_CFG::i2s_pins _pins = {2, 22, 21, 4, 27, 23, 26, 5, 25, 18, -1, 19, 32, 33}; // older model
HUB75_I2S_CFG mxconfig(64,                     // width
                       32,                     // height
                       1,                      // chain length
                       _pins);                   // pin mapping
//                       HUB75_I2S_CFG::FM6126A, // driver chip
//                       true,                   // double-buffering
//                       HUB75_I2S_CFG::HZ_10M);

MatrixPanel_I2S_DMA matrix = MatrixPanel_I2S_DMA(mxconfig);

void setup() {
  Serial.begin(115200);
  while ( !Serial ) {
    delay(1);
  }
  Serial.println("Staring up");
  if (!matrix.begin()) {
    Serial.println("[setup] matrix begin failed");
  } else {
    Serial.println("[setup] matrix initialized");
  }
  matrix.setBrightness8(100); //0-255
  matrix.setLatBlanking(1);
  matrix.clearScreen();
  marqueeText("Tidbyt", matrix.color565(0,255,255));  

  WiFi.setHostname("Tidbyt_Config");
  WiFi.setAutoReconnect(true);
  WiFi.begin();
  bool res;
  res = wifiManager.autoConnect("Tidbyt_Config"); // no password

    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
  

}

void loop() {
  // put your main code here, to run repeatedly:

}


void marqueeText(const String &buf, uint16_t color) {
    Serial.println("in marqueeText");
    matrix.clearScreen();
    matrix.drawRect(0,0,MATRIX_WIDTH,MATRIX_HEIGHT,color);
    int16_t x1, y1;
    uint16_t w, h;
    matrix.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h); //calc width of new string
    matrix.setCursor(32 - w / 2, 16 - h / 2);
    matrix.print(buf);
    
}
