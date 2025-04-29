#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <bmp280.h>
#include <dht22.h>
#include <gps.h>
#include <lora_local.h>
#include <mtp40f_local.h>
#include <power.h>

#define LORA_SYNC_WORD 0xB4 // Sync word for this network
#define LORA_SS_PIN PA8
#define LORA_RESET_PIN PB14
#define LORA_DIO0_PIN PB15
#define EMERGENCY_MODE_CO2_THRESHOLD 800 // Threshold for emergency mode (in PPM).
#define DEBUG_MODE_PIN PinName::PB_12
#define STATUS_LED PC13

auto GlobalSerial = HardwareSerial(USART1);

SPIClass LoRa_SPI(PB5, PB4, PB3);
byte device_id = 0x01; // Unique device identifier
GPS gps; // GPS object.

uint32_t last_packet_sent = 0; // Timestamp of the last packet that was transmitted.
uint32_t transmission_interval = 60 * 1000; // Interval for transmission (in ms).
uint32_t emergency_mode_interval = 10 * 1000; // Interval for emergency mode (in ms).
uint32_t debug_mode_transmission_interval = 5 * 1000; // Interval for transmission in debug mode (in ms)
uint32_t debug_mode_emergency_interval = 2 * 1000; // Interval for transmission in emergency mode (in ms)

void setup() {
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // Disable JTAG

    pinMode(DEBUG_MODE_PIN, INPUT_PULLDOWN);
    pinMode(STATUS_LED, OUTPUT);

    digitalWrite(STATUS_LED, HIGH);

    GlobalSerial.begin(9600);

    delay(5000); // Wait for Serial communication

    GlobalSerial.println("starting...");

    setup_power_sources();
    setup_lora(LORA_SYNC_WORD, LORA_SS_PIN, LORA_RESET_PIN, LORA_DIO0_PIN, LoRa_SPI);
    setup_bmp280();
    setup_dht22();
    setup_mtp40f();
    setup_gps();
}

void loop() {
    const uint16_t co2_ppm = read_mtp40f_gas_concentration();
    const bool debug = digitalReadFast(DEBUG_MODE_PIN) == HIGH;
    const uint32_t transmission_interval_val = debug ? debug_mode_transmission_interval : transmission_interval;
    const uint32_t emergency_mode_interval_val = debug ? debug_mode_emergency_interval : emergency_mode_interval;
    const bool emergency = co2_ppm >= EMERGENCY_MODE_CO2_THRESHOLD;
    // ReSharper disable once CppTooWideScopeInitStatement
    const uint32_t interval = emergency ? emergency_mode_interval_val : transmission_interval_val;

    if (millis() - last_packet_sent > interval) {
        digitalWrite(STATUS_LED, LOW);

        TinyGPSLocation loc = gps.get_location();
        LoRa_Payload payload;

        payload.co2_ppm = co2_ppm;

        payload.pressure = static_cast<uint16_t>(read_bmp280_pressure());
        payload.bmp280_altitude = static_cast<uint16_t>(read_bmp280_altitude());
        payload.bmp280_temp = static_cast<uint16_t>(read_bmp280_temperature() * 10);

        payload.dht22_temp = static_cast<uint16_t>(read_dht22_temperature() * 10);
        payload.humidity = static_cast<uint8_t>(read_dht22_humidity());

        payload.gps_altitude = static_cast<uint16_t>(gps.get_altitude().meters());
        payload.gps_satellites = static_cast<uint16_t>(gps.get_satellites().value());
        payload.gps_lat = static_cast<float>(loc.lat());
        payload.gps_lng = static_cast<float>(loc.lng());

        payload.battery_temp = static_cast<uint16_t>(read_battery_temperature() * 10);
        payload.battery_voltage = static_cast<uint16_t>(read_battery_voltage() * 100);
        payload.charger_voltage = static_cast<uint16_t>(read_charger_voltage() * 100);

        last_packet_sent = millis();
        send_lora_message(payload, emergency);

        digitalWrite(STATUS_LED, HIGH);
    } else {
        update_gps_object();
    }

    handle_lora_reception();
}
