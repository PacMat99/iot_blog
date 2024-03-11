// libraries for display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>

// libraries for SDS011
#include <SoftwareSerial.h>
#include <esp_sds011.h>

// libraries for MQTT
#include <PubSubClient.h>

#ifdef ESP32

#include <WiFi.h>
HardwareSerial& serialSDS(Serial2);
Sds011Async< HardwareSerial > sds011(serialSDS);
// SDS011 global variables
#define SDS_PIN_RX 16
#define SDS_PIN_TX 17
// LED pin
#define LED 4

#else

#include <ESP8266WiFi.h>
EspSoftwareSerial::UART serialSDS;
Sds011Async< EspSoftwareSerial::UART > sds011(serialSDS);
// SDS011 global variables
#define SDS_PIN_RX 14 // D5
#define SDS_PIN_TX 12 // D6
// LED pin
#define LED 13 // D7

#endif

int timer = 0;
int change_display = 0;
bool working;
bool ready_to_publish;
float pm25_float;
float pm10_float;

constexpr int pm_tablesize = 20;
int pm25_table[pm_tablesize];
int pm10_table[pm_tablesize];

bool is_SDS_running = true;
uint32_t deadline = millis() + 30 * 1000;
int now = 0;

// SDS011 auxiliary functions

void start_SDS() {
  Serial.println("Start wakeup SDS011");

  if (sds011.set_sleep(false)) {
    is_SDS_running = true;
  }

  Serial.println("End wakeup SDS011");
}

void stop_SDS() {
  Serial.println("Start sleep SDS011");

  if (sds011.set_sleep(true)) {
    is_SDS_running = false;
  }

  Serial.println("End sleep SDS011");
}

// SSD1306 global variables
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MQTT global variables
const char* ssid = "SSID";
const char* password = "PASSWORD";

const char* mqtt_server = "IP_ADDRESS";

// MQTT broker credentials
const char* MQTT_username = "USERNAME";
const char* MQTT_password = "MQTT_PASSWORD";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

// MQTT auxiliary functions

// Don't change the function below. This functions connects your ESP to your router
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

// This functions is executed when some device publishes a message to a topic that your ESP is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String topicMessage;
  int i;
  for (i = 0; i < length; i++) {
    topicMessage += (char)message[i];
  }
  Serial.println(topicMessage);

  if (String(topic) == "air_quality_monitor/led") {
    Serial.print("Turn led ");
    if(topicMessage == "on"){
      Serial.println("on");
      digitalWrite(LED, HIGH);
    }
    else if(topicMessage == "off"){
      Serial.println("off");
      digitalWrite(LED, LOW);
    }
  }
}

// This functions reconnects your ESP to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESPClient", MQTT_username, MQTT_password)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("air_quality_monitor/led");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 0.5 seconds before retrying
      delay(500);
    }
  }
}

// This functions publishes a string to the given topic
void publishString(String topic, String str) {
  client.publish(topic.c_str(), str.c_str());
}

void setup() {
  // Initialize Serial communication with the computer
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  Serial.println("Initializing SDS011 Air Quality Monitor...");

// Initialize Serial communication with SDS011
#ifdef ESP32
  serialSDS.begin(9600, SERIAL_8N1, SDS_PIN_RX, SDS_PIN_TX);
  delay(100);
#else
  serialSDS.begin(9600, SWSERIAL_8N1, SDS_PIN_RX, SDS_PIN_TX, false, 192);
#endif

  delay(100); // Short delay after initializing Serial
  Sds011::Report_mode report_mode;
  constexpr int GETDATAREPORTINMODE_MAXRETRIES = 2;
  for (auto retry = 1; retry <= GETDATAREPORTINMODE_MAXRETRIES; ++retry) {
    if (!sds011.get_data_reporting_mode(report_mode)) {
      if (retry == GETDATAREPORTINMODE_MAXRETRIES) {
        Serial.println("Sds011::get_data_reporting_mode() failed");
      }
    }
    else {
      break;
    }
  }
  if (Sds011::REPORT_ACTIVE != report_mode) {
    Serial.println("Turning on Sds011::REPORT_ACTIVE reporting mode");
    if (!sds011.set_data_reporting_mode(Sds011::REPORT_ACTIVE)) {
      Serial.println("Sds011::set_data_reporting_mode(Sds011::REPORT_ACTIVE) failed");
    }
  }

  delay(100);  // Short delay after initializing Serial

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

  bool sds_on = false;
  if (!sds_on) {
    stop_SDS();
    Serial.print("stopped SDS011 (is running = ");
    Serial.print(is_SDS_running);
    Serial.println(')');
  }
  working = false;
  ready_to_publish = false;

  delay(2000);
}

void loop() {
  if (static_cast<int32_t>(deadline - millis()) > 0 && !working) {
    if (millis() > now + 1000) {
      now = millis();
      Serial.println(static_cast<int32_t>(deadline - millis()) / 1000);
      sds011.perform_work();
    }
  }
  else if (static_cast<int32_t>(deadline - millis()) < 0 && !working) {
    deadline = millis() + 30 * 1000;
    now = 0;
    working = true;
    start_SDS();
    Serial.print("started SDS011 (is running = ");
    Serial.print(is_SDS_running);
    Serial.println(')');
    sds011.on_query_data_auto_completed([](int n) {
      Serial.println("Begin Handling SDS011 query data");
      int pm25;
      int pm10;
      if (sds011.filter_data(n, pm25_table, pm10_table, pm25, pm10) && !isnan(pm10) && !isnan(pm25)) {
        Serial.print("PM2.5: ");
        Serial.print(float(pm25) / 10, 2);
        Serial.print(" µg/m³   ");
        Serial.print("PM10: ");
        Serial.print(float(pm10) / 10, 2);
        Serial.println(" µg/m³");
      }
      Serial.println("End Handling SDS011 query data");
    });

    if (!sds011.query_data_auto_async(pm_tablesize, pm25_table, pm10_table)) {
      Serial.println("measurement capture start failed");
    }
    else {
      int i;
      for (i = 0, pm25_float = 0, pm10_float = 0; i < pm_tablesize; i++) {
        pm25_float += pm25_table[i];
        pm10_float += pm10_table[i];
      }
      pm25_float /= pm_tablesize;
      pm10_float /= pm_tablesize;
    }
  }

  if (static_cast<int32_t>(deadline - millis()) > 0 && working) {
    if (millis() > now + 1000) {
      now = millis();
      Serial.println(static_cast<int32_t>(deadline - millis()) / 1000);
      sds011.perform_work();
    }
  }
  else if (static_cast<int32_t>(deadline - millis()) < 0 && working) {
    deadline = millis() + 30 * 1000;
    now = 0;
    working = false;
    ready_to_publish = true;
  }

  String pm25_str = String(pm25_float / 10, 2);
  String pm10_str = String(pm10_float, 2);

  if (timer > timer + 3000) {
    display.clearDisplay();
    display.setFont(&FreeMono9pt7b);
    display.setTextColor(WHITE);

    if (change_display == 0) {
      display.setCursor(0, 30);
      display.print("PM2.5: ");
      display.setCursor(0, 50);
      display.print(pm25_str);  // 2 decimal places
      display.print(" µg/m³");
      display.display();
      change_display = !change_display;
    }
    else {
      display.setCursor(0, 30);
      display.print("PM10: ");
      display.setCursor(0, 50);
      display.print(pm10_str);  // 2 decimal places
      display.print(" µg/m³");
      display.display();
      change_display = !change_display;
    }
  }

  // MQTT connection and sending data to nodered
  if (!client.connected())
    reconnect();
  if(!client.loop())
    client.connect("ESPClient", MQTT_username, MQTT_password);
  if (ready_to_publish) {
    // Publishes pm2_5 and pm10 values  
    publishString("air_quality_monitor/pm2_5", pm25_str);
    publishString("air_quality_monitor/pm10", pm10_str);
    Serial.print("PM2.5: " + pm25_str + " µg/m³   ");
    Serial.print("PM10: " + pm10_str + " µg/m³   ");
    ready_to_publish = false;
  }
  delay(500);
}
