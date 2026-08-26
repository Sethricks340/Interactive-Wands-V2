// #include <Arduino.h>

// // put function declarations here:
// int myFunction(int, int);

// void setup() {
//   // put your setup code here, to run once:
//   int result = myFunction(2, 3);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// }

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }



#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

// TODO: change this to be dynamic
const char* ssid = "Threat Level Midnight";
const char* password = "cowabunga2!!";

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    ArduinoOTA.setHostname("WandTest");
    ArduinoOTA.begin();

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    // PERMANENT for OTA updates
    ArduinoOTA.handle();
    //
    // "C:\Users\sethr\backup\Desktop\Interactive Wands"

    digitalWrite(LED_BUILTIN, LOW); // Active High
    delay(2000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
}