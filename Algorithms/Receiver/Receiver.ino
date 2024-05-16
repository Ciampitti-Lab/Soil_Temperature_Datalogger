// Author: Gustavo N. Santiago

//////// Modify only this parts of the code ///////
#define file_name "/test.txt" // Change "test.txt" to the local database txt file you set before, keeping the "/"
long frequency = 915E6;
///////////////////////////////////////////////////

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

// Libraries for microSD card
#include <SD.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Libraries for WiFi and MongoDB
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"

//Define the pins used by the LoRa transceiver module
#define SCLK 5
#define MISO 19
#define MOSI 27
#define CS 18
#define RST 23
#define DIO0 26

// SD SPI pins
#define SD_CS 13
#define SD_SCK 14
#define SD_MOSI 15
#define SD_MISO 2

//OLED pins
#define OLED_SDA 21
#define OLED_SCL 22 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

// For SD card
File myFile;
SPIClass sd_spi(HSPI);

// Setting up a variable to store data from LoRa
String LoRaData;

// Monitore sending data to cloud
int httpResponseCode;

// Json document
DynamicJsonDocument doc(1024);

void setup() { 

  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.display();

  Serial.println("LoRa Receiver Test");
  
  //SPI LoRa pins
  SPI.begin(SCLK, MISO, MOSI, CS);
  LoRa.setPins(CS, RST, DIO0);

  //Initialize LoRa
  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  //Set the specific communication between devices
  LoRa.setSyncWord(0xF3);

  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.println("LoRa Initializing OK!");
  display.display();

  // Initialize microSD card
  sd_spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  if (SD.begin(SD_CS, sd_spi)) {
    display.clearDisplay();
    display.setCursor(0,10);
    display.println("Card initialized.");
    display.display();
    Serial.println("Card initialized.");
  }
  else{
    display.clearDisplay();
    display.setCursor(0,10);
    display.println("Card failed, or not present");
    display.display();
    Serial.println("Card failed, or not present");
    while(1);
  }

  // Connecting to internet
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println("Connected to WiFi");
  display.clearDisplay();
  display.setCursor(0,10);
  display.println("Connected to WiFi");
  display.display();

}

void loop() {

  // Try to parse packet
  int packetSize = LoRa.parsePacket();

  String id;
  String reading;
  String date;

  if (packetSize) {

    doc.clear();

    // Read packet
    while (LoRa.available()) {

      LoRaData = LoRa.readStringUntil(':');

      if(LoRaData.endsWith("ID"))
        id = LoRa.readStringUntil(',');
      if(LoRaData.endsWith("Readings"))
        reading = LoRa.readStringUntil(',');
      if(LoRaData.endsWith("DateTime"))
        date = LoRa.readStringUntil(',');

    }

    myFile = SD.open(file_name, FILE_APPEND); 
    myFile.print(id);
    myFile.print(",");
    myFile.print(reading);
    myFile.print(",");
    myFile.print(date);
    myFile.println(",");
    myFile.close();

    // Print RSSI of packet
    int rssi = LoRa.packetRssi();

    //Parsing Data to JSON:
    doc["group"] = id;
    doc["readings"] = reading;
    doc["dateTime"] = date;
    doc["LoRa RSSI"] = rssi;
    doc["Wifi RSSI"] = String(WiFi.RSSI());

    POSTData();

    // Display information
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Received packet");

    display.setCursor(0,10);
    display.print("Group:");
    display.setCursor(40,10);
    display.print(id);

    display.setCursor(0,20);
    display.print("RSSI:");
    display.setCursor(50,20);
    display.print(rssi);

    display.setCursor(0,30);
    display.print("Remote:");
    display.setCursor(60,30);
    if(httpResponseCode == 200){
      display.print("Sucess");
    }
    else{
      display.print("Error");
    }

    display.setCursor(0,40);
    display.print("Saved data to card");

    display.display();

  }

}
