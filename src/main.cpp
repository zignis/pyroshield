#include <Arduino.h>
#include <HardwareSerial.h>
#include <bmp280.h>
#include <dht22.h>
#include <gps.h>
#include <lora_local.h>
#include <mtp40f_local.h>
#include <power.h>

#define LORA_SYNC_WORD 0xB4 // Sync word for this network
#define LORA_SS_PIN PA8
#define LORA_RESET_PIN PB15
#define LORA_DIO0_PIN PB14
#define EMERGENCY_MODE_CO2_THRESHOLD 800 // Threshold for emergency mode (in PPM).

auto GlobalSerial = HardwareSerial(USART1);

byte device_id = 0x01; // Unique device identifier
int8_t BMP280_CS = 10; // Chip select for BMP280 sensor.
GPS gps_obj = get_gps_object();

uint32_t last_packet_sent = 0; // Timestamp of the last packet that was transmitted.
uint32_t transmission_interval = 60 * 1000; // Interval for transmission (in ms).
uint32_t emergency_mode_interval = 10 * 1000; // Interval for emergency mode (in ms).

void setup() {
    GlobalSerial.begin(9600);

    while (!GlobalSerial) {
    }

    setup_power_sources();
    setup_lora(LORA_SYNC_WORD, LORA_SS_PIN, LORA_RESET_PIN, LORA_DIO0_PIN);
    setup_bmp280();
    setup_dht22();
    setup_mtp40f();
    setup_gps();
}

void loop() {
    update_gps_object();

    const uint16_t co2_ppm = read_mtp40f_gas_concentration();
    // ReSharper disable once CppTooWideScopeInitStatement
    const uint32_t interval = co2_ppm >= EMERGENCY_MODE_CO2_THRESHOLD ? emergency_mode_interval : transmission_interval;

    if (millis() - last_packet_sent > interval) {
        TinyGPSLocation loc = gps_obj.get_location();
        LoRa_Payload payload;

        payload.co2_ppm = co2_ppm;

        payload.pressure = static_cast<uint16_t>(read_bmp280_pressure());
        payload.bmp280_altitude = static_cast<uint16_t>(read_bmp280_altitude());
        payload.bmp280_temp = static_cast<uint8_t>(read_bmp280_temperature());

        payload.dht22_temp = static_cast<uint8_t>(read_dht22_temperature());
        payload.humidity = static_cast<uint8_t>(read_dht22_humidity());

        payload.gps_altitude = static_cast<uint16_t>(gps_obj.get_altitude().meters());
        payload.gps_satellites = static_cast<uint16_t>(gps_obj.get_satellites().value());
        payload.gps_lat = static_cast<float>(loc.lat());
        payload.gps_lng = static_cast<float>(loc.lng());

        payload.battery_temp = read_battery_temperature();
        payload.battery_voltage = static_cast<uint16_t>(read_battery_voltage() * 100);
        payload.charger_voltage = static_cast<uint16_t>(read_charger_voltage() * 100);

        last_packet_sent = millis();
        send_lora_message(payload);
    }

    handle_lora_reception();
}
