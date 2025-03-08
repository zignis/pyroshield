#pragma once

/**
 * @brief Initializes the MTP40F sensor.
 */
void setup_mtp40f();

/**
 * @return `true` ff the sensor value has been updated since it was last read.
 */
bool has_mtp40f_updated();

/**
 * @return CO2 gas concentration in PPM.
 */
uint32_t read_mtp40f_gas_concentration();
