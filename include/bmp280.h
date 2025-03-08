#pragma once

/**
 * @brief Initializes the BMP280 sensor.
 */
void setup_bmp280();

/**
 * @return Temperature reading in degrees celsius.
 */
float read_bmp280_temperature();

/**
 * @return Pressure reading in Pa.
 */
float read_bmp280_pressure();

/**
 * @return Altitude reading in meters.
 */
float read_bmp280_altitude();
