// sds011 code explanation
// https://electronicsinnovation.com/interfacing-sds011-air-quality-sensor-with-esp8266-diy-air-pollution-monitor-part1/

// libraries for display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>

// library for SDS011
#include <SoftwareSerial.h>

// libraries for MQTT
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// SDS011 global variables
#define SDS_RX D5
#define SDS_TX D6
SoftwareSerial sds(SDS_RX, SDS_TX);

// SSD1306 global variables
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MQTT global variables
const char* ssid = "Pac's wifi_EXT";
const char* password = "Guglielmo06121937gp";

const char* mqtt_server = "192.168.68.101";

// MQTT broker credentials
const char* MQTT_username = "pacmat";
const char* MQTT_password = "Mattia2102";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

// MQTT auxiliary functions

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", MQTT_username, MQTT_password)) {
      Serial.println("connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // Initialize Serial communication with the computer
  Serial.begin(115200);
  delay(100);  // Short delay after initializing Serial
  Serial.println("Initializing SDS011 Air Quality Monitor...");
  // Initialize SoftwareSerial communication with SDS011
  sds.begin(9600);

  while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Failed to boot SSD1306");
    delay(500);
  }
  display.clearDisplay();
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Booting...");
  display.display();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  delay(2000);
}

void loop() {
  // Look for the starting byte of the SDS011 data frame
  while (sds.available() && sds.read() != 0xAA) { }
  if (sds.available()) {
    Serial.println("Data available from SDS011...");
  }

  String pm2_5_str;
  String pm10_str; 

  // Once we have the starting byte, attempt to read the next 9 bytes
  byte buffer[10];
  buffer[0] = 0xAA;  // The starting byte we already found
  if (sds.available() >= 9) {
    sds.readBytes(&buffer[1], 9);
    // Check if the last byte is the correct ending byte
    if (buffer[9] == 0xAB) {
      int pm25int = (buffer[3] << 8) | buffer[2];
      int pm10int = (buffer[5] << 8) | buffer[4];
      float pm2_5 = pm25int / 10.00;
      float pm10 = pm10int / 10.00;
      pm2_5_str = String(pm2_5, 2);
      pm10_str = String(pm10, 2);
      // Print the values
      Serial.print("PM2.5: ");
      Serial.print(pm2_5, 2);  // 2 decimal places
      Serial.print(" µg/m³   ");
      Serial.print("PM10: ");
      Serial.print(pm10, 2);  // 2 decimal places
      Serial.println(" µg/m³   ");

      display.clearDisplay();
      display.setFont(&FreeMono9pt7b);
      display.setTextColor(WHITE);

      display.setCursor(0, 10);
      display.print("Analyzing...");
      display.display();
      delay(1000);

      display.setCursor(0, 30);
      display.print("PM2.5: ");
      display.setCursor(0, 50);
      display.print(pm2_5_str);  // 2 decimal places
      display.print(" µg/m³");
      display.display();
      delay(3000);

      display.clearDisplay();
      display.setFont(&FreeMono9pt7b);
      display.setTextColor(WHITE);

      display.setCursor(0, 10);
      display.print("Analyzing...");
      display.setCursor(0, 30);
      display.print("PM10: ");
      display.setCursor(0, 50);
      display.print(pm10_str);  // 2 decimal places
      display.print(" µg/m³");
      display.display();
      delay(3000);

      // MQTT connection and sending data to nodered
      if (!client.connected())
        reconnect();
      if(!client.loop())
        client.connect("ESP8266Client", MQTT_username, MQTT_password);
      // Publishes pm2_5 and pm10 values    
      client.publish("air_quality_monitor/pm2_5", pm2_5_str.c_str());
      client.publish("air_quality_monitor/pm10", pm10_str.c_str());
    }
    else
      Serial.println("Invalid ending byte from SDS011.");
  }
  else
    Serial.println("Not enough data from SDS011.");
}
