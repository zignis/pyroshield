#pragma once

/**
 * @brief Initializes the DHT22 sensor.
 * @param pin Analog input pin
 */
void setup_dht22(uint8_t pin);

/**
 * @return Minimum sampling period of the sensor in milliseconds.
 */
int get_dht22_min_sampling_period();

/**
 * @return Sensor status.
 */
const char *read_dht22_status();

/**
 * @return Temperature reading in degrees celsius.
 */
float read_dht22_temperature();

/**
 * @return Humidity reading.
 */
float read_dht22_humidity();
