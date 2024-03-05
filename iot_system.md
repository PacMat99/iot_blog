---
title: IoT System
layout: default
nav_order: 9
---

# Test del display SSD1306

Prendendo spunto dagli step precedenti, creare un sistema IoT che permetta lo scambio di dati tra l'ESP, il Raspberry Pi per raccogliere i dati dai sensori, salvarli nel DB MySQL.

## 0. Componenti necessari

1 x Raspberry Pi  
1 x scheda SD per Raspberry Pi  
1 x Alimentatore per Raspberry Pi  
1 x ESP8266 o ESP32  
1 x display SSD1306  
1 x SDS011   
1 x led  
1 x resistenza 220ohm  
1 x breadboard  
Y x cavetti per collegare l'ESP a periferiche esterne

## 1. Creare il seguente schema elettrico

Collegare i pin del sensore SDS011 come segue:

1. 5V --> Vin ESP32
2. GND --> GND ESP32
3. RXD --> TX2 ESP32
4. TXD --> RX2 ESP32

Collegare i pin del display SSD1306 come segue:

1. 5V --> Vin ESP32
2. GND --> GND ESP32
3. SCK --> GPIO22 ESP32
4. SDA --> GPIO21 ESP32

Collegare i pin del led come segue:

1. Anodo --> Resistenza 220ohm
2. Resistenza 220ohm --> GPIO4 ESP32
3. Catodo --> GND ESP32

![ESP32 Wiring Diagram](../../images/wiring_diagrams/esp32_wiring_diagram.png)  
*ESP32 Wiring Diagram*

## 2. Installazione delle librerie

Installare le seguenti librerie nell'Arduino IDE:

1. Adafruit GFX Library by *Adafruit*  
   ![Adafruit GFX Library](../../images/arduino_ide/gfx_library.png)  
2. Adafruit SSD1306 by *Adafruit*  
   ![Adafruit SSD1306 Library](../../images/arduino_ide/ssd1306_library.png)  
3. esp_sds011 by *Dirk O. Kaar*  
   ![SDS011 Library](../../images/arduino_ide/sds011_library.png)

## 3. Eseguire il codice

Collegare l'ESP32 al pc, copiare il codice seguente in un file nell'Arduino IDE e caricarlo sul microcontrollore.  
Ogni 30s l'ESP32 avvia il sensore SDS011, in altri 30s analizza l'aria e infine calcola il corretto valore di PM2.5 e PM10. Successivamente mostra i dati sul display oled e li invia a Node-Red tramite WiFi.  
Quando l'ESP32 riceve da Node-Red un comando sul topic *esp32/output* accende o spegne il led di conseguenza.  
Node-Red si occupa di salvare i dati nel DB MySQL.

```
// sds011 code explanation
// https://electronicsinnovation.com/interfacing-sds011-air-quality-sensor-with-esp8266-diy-air-pollution-monitor-part1/

// libraries for display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>

// libraries for SDS011
#include <SoftwareSerial.h>
#include <esp_sds011.h>

// libraries for MQTT
#include <WiFi.h>
#include <PubSubClient.h>

// SDS011 global variables
#define SDS_PIN_RX 16
#define SDS_PIN_TX 17

// LED pin
#define LED 4

#ifdef ESP32
HardwareSerial& serialSDS(Serial2);
Sds011Async< HardwareSerial > sds011(serialSDS);
#else
EspSoftwareSerial::UART serialSDS;
Sds011Async< EspSoftwareSerial::UART > sds011(serialSDS);
#endif

constexpr int pm_tablesize = 20;
int pm25_table[pm_tablesize];
int pm10_table[pm_tablesize];

bool is_SDS_running = true;

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

// Don't change the function below. This functions connects your ESP32 to your router
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

// This functions is executed when some device publishes a message to a topic that your ESP32 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP32 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String topicMessage;
  int i;
  for (i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    topicMessage += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "esp32/output") {
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

// This functions reconnects your ESP32 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP32
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client", MQTT_username, MQTT_password)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
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

  delay(2000);
}

void loop() {
  constexpr uint32_t down_s = 30;

  stop_SDS();
  Serial.print("stopped SDS011 (is running = ");
  Serial.print(is_SDS_running);
  Serial.println(')');

  uint32_t deadline = millis() + down_s * 1000;
  while (static_cast<int32_t>(deadline - millis()) > 0) {
    delay(1000);
    Serial.println(static_cast<int32_t>(deadline - millis()) / 1000);
    sds011.perform_work();
  }

  constexpr uint32_t duty_s = 30;

  start_SDS();
  Serial.print("started SDS011 (is running = ");
  Serial.print(is_SDS_running);
  Serial.println(')');

  sds011.on_query_data_auto_completed([](int n) {
    Serial.println("Begin Handling SDS011 query data");
    int pm25;
    int pm10;
    Serial.print("n = "); Serial.println(n);
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

  float pm25_float;
  float pm10_float;
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

  deadline = millis() + duty_s * 1000;
  while (static_cast<int32_t>(deadline - millis()) > 0) {
    delay(1000);
    Serial.println(static_cast<int32_t>(deadline - millis()) / 1000);
    sds011.perform_work();
  }

  String pm25_str = String(pm25_float / 10, 2);
  String pm10_str = String(pm10_float, 2);
  // Print the values
  Serial.print("PM2.5: " + pm25_str + " µg/m³   ");
  Serial.print("PM10: " + pm10_str + " µg/m³   ");

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
  display.print(pm25_str);  // 2 decimal places
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
    client.connect("ESP32Client", MQTT_username, MQTT_password);
  // Publishes pm2_5 and pm10 values    
  client.publish("air_quality_monitor/pm2_5", pm25_str.c_str());
  client.publish("air_quality_monitor/pm10", pm10_str.c_str());
}
```
