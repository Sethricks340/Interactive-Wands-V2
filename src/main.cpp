


// #include <Arduino.h>
// #include <WiFi.h>
// #include <ArduinoOTA.h>

// // TODO: change this to be dynamic
// const char* ssid = "Threat Level Midnight";
// const char* password = "cowabunga2!!";

// void setup() {
//     Serial.begin(115200);

//     WiFi.begin(ssid, password);

//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//     }

//     ArduinoOTA.setHostname("WandTest");
//     ArduinoOTA.begin();

//     pinMode(LED_BUILTIN, OUTPUT);
// }

// void loop() {
//     // PERMANENT for OTA updates
//     ArduinoOTA.handle();
//     //
//     // "C:\Users\sethr\backup\Desktop\Interactive Wands"

//     digitalWrite(LED_BUILTIN, LOW); // Active High
//     delay(2000);
//     digitalWrite(LED_BUILTIN, HIGH);
//     delay(250);
// }





#include <Arduino.h>
#include <WiFi.h>
// #include <ArduinoOTA.h>
#include <WiFiClientSecure.h>
#include <HTTPUpdate.h>

const char* ssid = "Threat Level Midnight";
const char* password = "cowabunga2!!";

const char* firmwareURL =
    // "https://github.com/Sethricks340/Interactive-Wands-V2/raw/refs/heads/main/firmware/firmware.bin";
    "https://raw.githubusercontent.com/Sethricks340/Interactive-Wands-V2/main/firmware/firmware.bin";

const char* versionURL =
    "https://raw.githubusercontent.com/Sethricks340/Interactive-Wands-V2/main/firmware/version.txt";

const int FIRMWARE_VERSION = 0;

void setup() {
    Serial.begin(115200);
    delay(2000);

    pinMode(LED_BUILTIN, OUTPUT);

    // LED OFF initially
    digitalWrite(LED_BUILTIN, HIGH);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    // ArduinoOTA.setHostname("WandTest");
    // ArduinoOTA.begin();




    // // Try GitHub
    // HTTPClient http;
    // http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    // http.begin(firmwareURL);

    // int httpCode = http.GET();

    // if (httpCode == HTTP_CODE_OK) {
    //     // SUCCESS: LED ON
    //     digitalWrite(LED_BUILTIN, LOW);

    //     WiFiClientSecure client;
    //     client.setInsecure(); // TODO: change this to secure

    //     t_httpUpdate_return result =
    //         httpUpdate.update(client, firmwareURL);

    // } else {
    //     // FAILURE: LED OFF
    //     digitalWrite(LED_BUILTIN, HIGH);
    // }

    // http.end();





    // Check the version number for updates
    HTTPClient versionhttp;
    versionhttp.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    versionhttp.begin(versionURL);

    int httpCode = versionhttp.GET();

    if (httpCode == HTTP_CODE_OK) {
        String serverVersion = versionhttp.getString();
        serverVersion.trim();
        if (serverVersion.toInt() > FIRMWARE_VERSION){
            
            // Try GitHub
            HTTPClient firmwarehttp;
            firmwarehttp.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
            firmwarehttp.begin(firmwareURL);

            int httpCode = firmwarehttp.GET();

            if (httpCode == HTTP_CODE_OK) {
                // SUCCESS: LED ON
                // digitalWrite(LED_BUILTIN, LOW);

                WiFiClientSecure client;
                client.setInsecure(); // TODO: change this to secure

                t_httpUpdate_return result =
                    httpUpdate.update(client, firmwareURL);

                    // TODO: update the version macro
            } 
            // else {
            //     // FAILURE: LED OFF
            //     digitalWrite(LED_BUILTIN, HIGH);
            // }

            firmwarehttp.end();

        }

    }

    versionhttp.end();


}

void loop() {
    // ArduinoOTA.handle();


    // run this to test if the update worked
    digitalWrite(LED_BUILTIN, LOW); // Active High
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
}