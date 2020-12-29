//json解析
#include <Arduino_JSON.h>
//oled显示器
#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
//定义引脚
#define SCL 0
#define SDA 2

//处理字符串
#include<string>
#include<cstring>
#include<iostream>

//WIFI库
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>
const uint8_t fingerprint[20] = {0x58, 0x0a, 0xb6, 0x35, 0x55, 0xc7, 0xe9, 0x68, 0xed, 0xbc, 0xbd, 0x15, 0x32, 0x7a, 0x9e, 0x40, 0x65, 0x83, 0xb9, 0xf9};
ESP8266WiFiMulti WiFiMulti;

/*配置WIFI信息*/
const char* ssid = "Android...";      // wifi名
const char* password = "××××××××××";  // wifi密码

//RFID读卡库
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         5           // 配置针脚
#define SS_PIN          4
MFRC522 mfrc522(SS_PIN, RST_PIN);   // 创建新的RFID实例
MFRC522::MIFARE_Key key;

uint8_t successRead;    // Variable integer to keep if we have Successful Read from Reader
byte readCard[4];   // Stores scanned ID read from RFID Module
int intUID[10];


//oled显示器
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

void setup() {

  Serial.begin(9600); // 设置串口波特率为9600
  SPI.begin();        // SPI开始
  mfrc522.PCD_Init(); // Init MFRC522 card
  Serial.println("test-demo-start");
  //WiFi.mode(WIFI_STA);
  //WiFiMulti.addAP("赶风", "×××××××××××");
  u8g2.begin();
  u8g2.enableUTF8Print();   // enable UTF8 support for the Arduino print() function
  connectWifi();
  //demoCreation();
}


void loop() {
  successRead = getID();
  if (successRead == 1) {
    Serial.println("读取uid成功~");
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy15_t_chinese2);  // use chinese2 for all the glyphs
    u8g2.setFontDirection(0);
    u8g2.clearBuffer();
    u8g2.setCursor(0, 15);
    u8g2.print("uid:");
    u8g2.setCursor(0, 40);
    u8g2.print(intTOstring(intUID));   // Chinese "Hello World"
    u8g2.sendBuffer();
    //Serial.println(intTOstring(intUID));
    if (postData("uid=" + intTOstring(intUID)+"&doorid=0"))
    {
      Serial.println("##post Successful##");
      u8g2.setCursor(0, 60);
      u8g2.print("#post Successful#");   // Chinese "Hello World"
      u8g2.sendBuffer();

    } else {
      Serial.println("***post failed**");
    }

  }

}


/*
  将数组内的十进制(int)转成16进制（string）
*/
String intTOstring(int intUID[])
{
  //Serial.println("********start******");
  String str;
  char s[10];
  for (int i = 0; i < 4; i++) {
    sprintf(s, "%x", intUID[i]);
    str = str + s;
  }
  //Serial.println(str);
  return str;

}

/*
  post数据到服务器
*/
int postData(String stringUID)
{
  Serial.println("post start=====" + stringUID);
  // wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint(fingerprint);

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://www.auster.fun/doortp/public/api/door/upup?token=api2020")) {  // HTTPS
      Serial.print("[HTTPS] post...\n");
      // start connection and send HTTP header
      https.addHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
      // int httpCode = https.POST("test=123&test2=233");
      int httpCode = https.POST(stringUID);
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] post... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
          //开始测试
          JSONVar myObject = JSON.parse(payload);
          // JSON.typeof(jsonVar) can be used to get the type of the var
          if (JSON.typeof(myObject) == "undefined") {
            Serial.println("Parsing input failed!");
            //return;
          }

          Serial.print("JSON.typeof(myObject) = ");
          Serial.println(JSON.typeof(myObject)); // prints: object
          if (myObject.hasOwnProperty("data")) {
    Serial.print("myObject[\"data\"] = ");

    Serial.println(myObject["data"]);
  }
          //结束测试
        }

        return 1;
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  return 0;
  Serial.println("Wait 10s before next round...");
  delay(10000);
}

/*读取uid*/
uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
  Serial.println(F("Scanned PICC's UID:"));
  for ( uint8_t i = 0; i < 4; i++) {  //
    //readCard[i] = mfrc522.uid.uidByte[i];
    intUID[i] = (int)mfrc522.uid.uidByte[i];
    //Serial.println(readCard[i], HEX);
    /// Serial.println(intUID[i]);
  }

  // Serial.println("");
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

/**
   连接wifi
*/
void connectWifi() {
  Serial.println();
  Serial.print("connecting to AP:");
  Serial.println(ssid);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy15_t_chinese2);  // use chinese2 for all the glyphs
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print("connecting to AP:");
  u8g2.setCursor(5, 40);
  u8g2.print(ssid);   // Chinese "Hello World"
  u8g2.sendBuffer();

  WiFi.mode(WIFI_STA);    // 设置wifi模式
  WiFi.begin(ssid, password);   // 连接wifi
  int x = 0;
  int y=0;
  while (WiFi.status() != WL_CONNECTED) { //判断连接状态
    delay(500);
    Serial.print(".");
    u8g2.setCursor(3 + x, 50+y);
    x = x + 8;
    if(x>128)
    {
      x=0;
      y=y+5;
    }
    u8g2.print(".");   // Chinese "Hello World"
    u8g2.sendBuffer();
  }


  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy15_t_chinese2);  // use chinese2 for all the glyphs
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print("WiFi connected");
  u8g2.setCursor(0, 40);
  u8g2.print(WiFi.localIP());   // Chinese "Hello World"
  u8g2.sendBuffer();

}
