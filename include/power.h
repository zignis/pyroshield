#pragma once

/**
 * @brief Initializes the battery and charger sources.
 */
void setup_power_sources();

/**
 * @brief Reads the battery temperature via thermistor using the Steinhart-Hart equation.
 * @return Battery temperature reading in degrees celsius.
 */
float read_battery_temperature();

/**
 * @return Battery voltage reading (in volts).
 */
float read_battery_voltage();

/**
 * @return Charger voltage reading (in volts).
 */
float read_charger_voltage();
