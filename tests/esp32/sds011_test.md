---
title: SDS011 Test
layout: default
nav_order: 2
parent: ESP32
grand_parent: ESPx tests
---

# Test del sensore SDS011

## 1. Creare il seguente schema elettrico

Collegare i pin del sensore SDS011 come segue:

1. 5V --> Vin ESP32
2. GND --> GND ESP32
3. RXD --> TX2 ESP32
4. TXD --> RX2 ESP32

![SDS011 Wiring Diagram](../../images/esp32_sds011_wiring_diagram.png)  
*ESP32 SDS011 Wiring Diagram*

## 2. Eseguire il codice

Collegare l'ESP32 al pc, copiare il codice seguente in un file nell'Arduino IDE e caricarlo sul microcontrollore.

**ATTENZIONE**: modificare il codice seguente utilizzando la libreria [esp_sds011](https://github.com/dok-net/esp_sds011)

```
#include <SoftwareSerial.h>

// SDS011 software serial pins
#define SDS_RX D5
#define SDS_TX D6
SoftwareSerial sds(SDS_RX, SDS_TX);

void setup() {
    // Initialize Serial communication with the computer
    Serial.begin(115200);
    delay(100);  // Short delay after initializing Serial
    Serial.println("Initializing SDS011 Air Quality Monitor...");
    // Initialize SoftwareSerial communication with SDS011
    sds.begin(9600);
}

void loop() {
    // Look for the starting byte of the SDS011 data frame
    while (sds.available() && sds.read() != 0xAA) { }
    if (sds.available()) {
        Serial.println("Data available from SDS011...");
    }

    // Once we have the starting byte, attempt to read the next 9 bytes
    byte buffer[10];
    buffer[0] = 0xAA;  // The starting byte we already found
    if (sds.available() >= 9) {
        sds.readBytes(&buffer[1], 9);
        // Check if the last byte is the correct ending byte
        if (buffer[9] == 0xAB) {
            int pm25int = (buffer[3] << 8) | buffer[2];
            int pm10int = (buffer[5] << 8) | buffer[4];
            float pm2_5 = pm25int / 10.0;
            float pm10 = pm10int / 10.0;
            // Print the values
            Serial.print("PM2.5: ");
            Serial.print(pm2_5, 2);  // 2 decimal places
            Serial.print(" µg/m³   ");
            Serial.print("PM10: ");
            Serial.print(pm10, 2);  // 2 decimal places
            Serial.println(" µg/m³   ");
        } else {
            Serial.println("Invalid ending byte from SDS011.");
        }
    } else {
        Serial.println("Not enough data from SDS011.");
    }
    delay(1000);  // Delay before the next read to avoid flooding the serial
}
```
