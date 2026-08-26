#include <Arduino.h>
#include <WiFi.h>
// #include <ArduinoOTA.h>
#include <WiFiClientSecure.h>
#include <HTTPUpdate.h>

#define LED_ON  LOW
#define LED_OFF HIGH

const char* ssid = "Threat Level Midnight";
const char* password = "cowabunga2!!";

const char* firmwareURL =
    // "https://github.com/Sethricks340/Interactive-Wands-V2/raw/refs/heads/main/firmware/firmware.bin";
    "https://raw.githubusercontent.com/Sethricks340/Interactive-Wands-V2/main/firmware/firmware.bin";

const char* versionURL =
    "https://raw.githubusercontent.com/Sethricks340/Interactive-Wands-V2/main/firmware/version.txt";

const int FIRMWARE_VERSION = 1;

void UpdateFirmware() {
    WiFiClientSecure client;
    client.setInsecure();

    t_httpUpdate_return result =
        httpUpdate.update(client, firmwareURL);

    // If update fails, execution continues here
}

void CheckUpdates() {
    HTTPClient versionhttp;
    versionhttp.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    versionhttp.begin(versionURL);

    int httpCode = versionhttp.GET();

    if (httpCode == HTTP_CODE_OK) {
        String serverVersion = versionhttp.getString();
        serverVersion.trim();

        if (serverVersion.toInt() > FIRMWARE_VERSION) {
            UpdateFirmware();
        }
    }

    versionhttp.end();
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    pinMode(LED_BUILTIN, OUTPUT);
    // Test LED off initially
    digitalWrite(LED_BUILTIN, LED_OFF);

    WiFi.begin(ssid, password); // TODO: change to dynamic from iphone

    while (WiFi.status() != WL_CONNECTED) {
        delay(500); // TODO: add timeout
    }

   CheckUpdates();

}

void loop() {
    // run this to test if the update worked
    digitalWrite(LED_BUILTIN, LED_ON);
    delay(1000);
    digitalWrite(LED_BUILTIN, LED_OFF);
    delay(1000);
}