#include <Arduino.h>
#include <bmp280.h>
#include <dht22.h>
#include <gps.h>
#include <mtp40f_local.h>

int8_t BMP280_CS = 10; // Chip select for BMP280 sensor.
uint8_t DHT22_PIN = 2;

void setup() {
    Serial.begin(9600);

    setup_bmp280();
    setup_dht22(DHT22_PIN);
    setup_mtp40f();
    setup_gps();
}

void loop() { start_gps_receiver(); }
