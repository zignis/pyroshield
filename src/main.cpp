#include <Arduino.h>
#include <Scheduler.h>
#include <bmp280.h>
#include <dht22.h>
#include <gps.h>
#include <lora_local.h>
#include <mtp40f_local.h>

#define LORA_SYNC_WORD 0xB4
#define LORA_SS_PIN 10
#define LORA_RESET_PIN 9
#define LORA_DIO0_PIN 2
#define DHT22_PIN 3

byte device_id = 0x01; // Unique device identifier
int8_t BMP280_CS = 10; // Chip select for BMP280 sensor.
GPS gps_obj = get_gps_object();

uint32_t last_packet_sent = 0; // Timestamp of the last packet that was transmitted.
uint32_t transmission_interval = 30 * 1000; // Interval for transmission (in ms).

void setup() {
    Serial.begin(9600);

    while (!Serial) {
    }

    setup_lora(LORA_SYNC_WORD, LORA_SS_PIN, LORA_RESET_PIN, LORA_DIO0_PIN);
    setup_bmp280();
    setup_dht22(DHT22_PIN);
    setup_mtp40f();
    setup_gps();

    SchedulerClass::startLoop(update_gps_object);
    SchedulerClass::startLoop(handle_lora_reception);
}

void loop() {
    if (millis() - last_packet_sent > transmission_interval) {
        last_packet_sent = millis();

        uint32_t gas_concentration = read_mtp40f_gas_concentration();

        float altitude = read_bmp280_altitude();
        float pressure = read_bmp280_pressure();
        float bmp280_temp = read_bmp280_temperature();

        float humidity = read_dht22_humidity();
        float dht22_temp = read_dht22_temperature();
    }
}
