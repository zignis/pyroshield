#include <Arduino.h>

#define THERMISTOR_PIN PA0 // ADC-0 pin
#define BATTERY_PIN PA1 // ADC-1 pin
#define CHARGER_PIN PB0 // ADC-8 pin

#define V_SUPPLY 3.3 // Supply voltage
#define RESISTANCE_10K (10 * 1000) // 10k resistor resistance (in ohms)
#define B_VALUE 3950 // B-coefficient of 10k thermistor
#define TEMP_0 298.15 // 25°C in kelvin

void setup_power_sources() {
    pinMode(THERMISTOR_PIN, INPUT_ANALOG);
    pinMode(BATTERY_PIN, INPUT_ANALOG);
    pinMode(CHARGER_PIN, INPUT_ANALOG);
}

uint8_t read_battery_temperature() {
    const double reading = analogRead(THERMISTOR_PIN) * (V_SUPPLY / 4095); // Voltage divider output
    const double resistance_ntc = reading * RESISTANCE_10K / (V_SUPPLY - reading); // Resistance of the thermistor
    const double kelvin = (TEMP_0 * B_VALUE) / (TEMP_0 * log(resistance_ntc / RESISTANCE_10K) + B_VALUE);
    return static_cast<uint8_t>(kelvin - 273.15); // Convert kelvin to celsius.
}

float read_battery_voltage() {
    // TODO
    return 0;
}

float read_charger_voltage() {
    // TODO
    return 0;
}
