#pragma once

/**
 * @brief Initializes the DHT22 sensor.
 */
void setup_dht22();

/**
 * @return Temperature reading in degrees celsius.
 */
float read_dht22_temperature();

/**
 * @return Humidity reading.
 */
float read_dht22_humidity();
