#pragma once

typedef struct {
    /* Header */
    byte forwarder_id = 0; /// Forwarder device ID.
    byte transmitter_id = 0; /// Transmitter device ID.
    uint16_t message_id = 0; /// The message ID.
    bool allow_forwarding = false; /// Forwarding flag.
    uint8_t ttl = 0; /// TTL value.

    /* Body */
    uint16_t co2_ppm{}; /// The CO2 gas concentration.
    uint16_t pressure{}; /// The pressure reading.
    uint16_t bmp280_altitude{}; /// The altitude reading from BMP280 sensor.
    uint8_t bmp280_temp{}; /// The temperature reading from BMP280 sensor.
    uint8_t dht22_temp{}; /// The temperature reading from DHT22 sensor.
    uint8_t humidity{}; /// The humidity reading.
    uint16_t gps_altitude{}; /// The altitude reading from GPS.
    float gps_lat{}; /// The latitude reading from GPS.
    float gps_lng{}; /// The longitude reading from GPS.
    uint16_t gps_satellites{}; /// The number of GPS satellites in view.

    /* Power */
    uint16_t battery_voltage{}; /// The battery voltage reading (scaled by 100).
    uint16_t charger_voltage{}; /// The charger voltage reading (scaled by 100).
} LoRa_Payload;

/**
 * @brief Initializes the LoRa module.
 * @param sync_word The sync word to isolate the network.
 * @param ss The NSS pin.
 * @param reset The NRESET pin.
 * @param dio0 The DIO0 pin.
 */
void setup_lora(int sync_word, int ss, int reset, int dio0);

/**
 * @brief Sends a LoRa message.
 * @param payload The payload to send.
 */
void send_lora_message(LoRa_Payload payload);

/**
 * @brief Incoming LoRa messages handler.
 */
void handle_lora_reception();
