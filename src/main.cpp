#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPUpdate.h>

#define LED_ON  LOW
#define LED_OFF HIGH
#define BUTTON_PIN D0
#define RED_PIN    D1
#define GREEN_PIN  D2
#define BLUE_PIN   D3

const char* ssid = "Threat Level Midnight";
const char* password = "cowabunga2!!";

const char* firmwareURL =
    "https://raw.githubusercontent.com/Sethricks340/Interactive-Wands-V2/main/firmware/firmware.bin";

const char* versionURL =
    "https://raw.githubusercontent.com/Sethricks340/Interactive-Wands-V2/main/firmware/version.txt";

const int FIRMWARE_VERSION = 2;

void UpdateFirmware() {
    WiFiClientSecure client;
    client.setInsecure();

    t_httpUpdate_return result =
        httpUpdate.update(client, firmwareURL);
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    pinMode(LED_BUILTIN, OUTPUT);
    // LED off initially
    digitalWrite(LED_BUILTIN, LED_OFF);

    WiFi.begin(ssid, password); // TODO: change to dynamic from iphone

    while (WiFi.status() != WL_CONNECTED) {
        delay(500); // TODO: add timeout
    }

    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

}

void loop() {
    // run this to test if the update worked
    if (digitalRead(BUTTON_PIN) == LOW) {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
        UpdateFirmware();
    }
    
    // Green
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
}