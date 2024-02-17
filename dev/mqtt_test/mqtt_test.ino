#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Pac's wifi_EXT";
const char* password = "Guglielmo06121937gp";

const char* mqtt_server = "192.168.68.100";

// MQTT broker credentials
const char* MQTT_username = "pacmat";
const char* MQTT_password = "Mattia2102";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;

int pm2_5 = 1;
int pm10 = 2;

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

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// For this project, you don't need to change anything in the loop function. Basically it ensures that you ESP is connected to your broker
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client", MQTT_username, MQTT_password);

  now = millis();
  // Publishes new pm2_5 and pm10 every 5 seconds
  if (now - lastMeasure > 5000) {
    lastMeasure = now;

    if (pm2_5 > 10)
      pm2_5 = 1;
    if (pm10 > 20)
      pm10 = 1;

    // Publishes pm2_5 and pm10 values    
    client.publish("air_quality_monitor/pm2_5", String(pm2_5).c_str());
    client.publish("air_quality_monitor/pm10", String(pm10).c_str());

    pm2_5++;
    pm10++;

    Serial.print("pm10: ");
    Serial.println(pm10);
    Serial.print("pm2_5: ");
    Serial.println(pm2_5);
  }
} 
