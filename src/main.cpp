// #include <Arduino.h>
// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <HTTPUpdate.h>

// #define LED_ON  LOW
// #define LED_OFF HIGH
// #define BUTTON_PIN D0
// #define RED_PIN    D1
// #define GREEN_PIN  D2
// #define BLUE_PIN   D3

// const char* ssid = "Threat Level Midnight";
// const char* password = "cowabunga2!!";

// const char* firmwareURL =
//     "https://raw.githubusercontent.com/Sethricks340/Interactive-Wands-V2/main/firmware/firmware.bin";

// const char* versionURL =
//     "https://raw.githubusercontent.com/Sethricks340/Interactive-Wands-V2/main/firmware/version.txt";

// const int FIRMWARE_VERSION = 2;

// void UpdateFirmware() {
//     WiFiClientSecure client;
//     client.setInsecure();

//     Serial.println("Starting OTA update...");

//     t_httpUpdate_return result =
//         httpUpdate.update(client, firmwareURL);

//     Serial.print("OTA result: ");
//     Serial.println(result);
// }

// void setup() {
//     Serial.begin(115200);
//     delay(2000);

//     pinMode(LED_BUILTIN, OUTPUT);
//     // LED off initially
//     digitalWrite(LED_BUILTIN, LED_OFF);

//     WiFi.begin(ssid, password); // TODO: change to dynamic from iphone

//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500); // TODO: add timeout
//     }

//     pinMode(RED_PIN, OUTPUT);
//     pinMode(GREEN_PIN, OUTPUT);
//     pinMode(BLUE_PIN, OUTPUT);
//     pinMode(BUTTON_PIN, INPUT_PULLUP);

// }

// void loop() {
//     // run this to test if the update worked
//     if (digitalRead(BUTTON_PIN) == LOW) {
//         digitalWrite(RED_PIN, LOW);
//         digitalWrite(GREEN_PIN, LOW);
//         digitalWrite(BLUE_PIN, LOW);
//         UpdateFirmware();
//     }
    
//     // change color test
//     digitalWrite(RED_PIN, HIGH);
//     digitalWrite(GREEN_PIN, HIGH);
//     digitalWrite(BLUE_PIN, HIGH);
// }

// MPU-6050       XIAO ESP32-S3
// ----------------------------
// VCC       →    3.3V
// GND       →    GND
// SDA       →    D4
// SCL       →    D5

#include <Arduino.h>
#include <Wire.h>

#define MPU_ADDR 0x68

#define PWR_MGMT_1   0x6B
#define GYRO_CONFIG  0x1B
#define ACCEL_XOUT_H 0x3B

#define SDA_PIN D4
#define SCL_PIN D5

// ±2000 degrees/second
const float GYRO_LSB_PER_DPS = 16.4;
const int movement_threshold = 800;

void setup() {
    Serial.begin(115200);
    delay(2000);

    // XIAO ESP32-S3 I2C
    Wire.begin(SDA_PIN, SCL_PIN);

    // Wake up MPU-6050
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(PWR_MGMT_1);
    Wire.write(0x00);
    Wire.endTransmission();

    // Set gyro to ±2000 °/s
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(GYRO_CONFIG);
    Wire.write(0x18);
    Wire.endTransmission();

    Serial.println("MPU-6050 ready");
}

void loop() {
    uint8_t buf[14];

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(ACCEL_XOUT_H);

    if (Wire.endTransmission(false) == 0) {
        Wire.requestFrom(MPU_ADDR, 14);

        for (uint8_t i = 0; i < 14; i++) {
            if (Wire.available()) {
                buf[i] = Wire.read();
            }
        }

        int16_t gyro_x = (int16_t)((buf[8] << 8) | buf[9]);
        int16_t gyro_y = (int16_t)((buf[10] << 8) | buf[11]);
        int16_t gyro_z = (int16_t)((buf[12] << 8) | buf[13]);

        float gx = gyro_x / GYRO_LSB_PER_DPS;
        float gy = gyro_y / GYRO_LSB_PER_DPS;
        float gz = gyro_z / GYRO_LSB_PER_DPS;

        // Serial.print("X: ");
        // Serial.print(gx);
        // Serial.print("  Y: ");
        // Serial.print(gy);
        // Serial.print("  Z: ");
        // Serial.println(gz);
        // Simple threshold detection
        if (gx >= movement_threshold) Serial.println("Pitch Back (PB)");
        if (gx <= -movement_threshold) Serial.println("Pitch Forward (PF)");
        if (gy >= movement_threshold) Serial.println("Roll Clockwise (RCW)");
        if (gy <= -movement_threshold) Serial.println("Roll Counter-Clockwise (RCCW)");
        if (gz >= movement_threshold) Serial.println("Yaw Left (YL)");
        if (gz <= -movement_threshold) Serial.println("Yaw Right (YR)");
    }

    delay(10);
}