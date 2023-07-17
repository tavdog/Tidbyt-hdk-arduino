// Tidbyt Arduino Basic.  A simple starter project to get you going with your Tidbyt in the Arduino IDE. Comes with WifiManager
// by tavdog

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFiManager.h>
#include <HTTPClient.h>

int update_interval_seconds = 1;
String data_url = "http://wildc.net/wind/minsec.pl";  // must return text string only
int brightness = 50;

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
  matrix.setBrightness8(brightness); //0-255
  matrix.setLatBlanking(1);
  matrix.clearScreen();
  marqueeText("Tidbyt", matrix.color565(0,255,255));
  delay(5000);

  WiFi.setAutoReconnect(true);
  WiFi.begin();
  bool res;
  marqueeText("WiFi", matrix.color565(255,0,255));
  
  res = wifiManager.autoConnect("Tidbyt_Config"); // no password

    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        marqueeText("Connected!", matrix.color565(255,255,0));

    }
}

HTTPClient http;
void loop() {
  WiFiClient client;
  Serial.print("[HTTP] begin...\n");
  http.begin(client,data_url.c_str()); //HTTP
  
  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  
  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
          marqueeText(payload,matrix.color565(0,255,255));
      }
  } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  
  http.end();
  delay(update_interval_seconds * 1000);

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
