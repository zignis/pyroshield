#include <Arduino.h>

#define THERMISTOR_PIN PA0 // ADC-0 pin
#define BATTERY_PIN PA1 // ADC-1 pin
#define CHARGER_PIN PB0 // ADC-8 pin

#define V_SUPPLY 3.3 // Supply voltage
#define RESISTANCE_10K 10000 // 10k resistor resistance (in ohms)
#define B_VALUE 3950 // B-coefficient of 10k thermistor
#define TEMP_0 298.15 // 25°C in kelvin

#define V_DIVIDER_R1 33000.0 // Voltage divider resistor-1 value (in ohms, other end is connected to input voltage)
#define V_DIVIDER_R2 15000.0 // Voltage divider resistor-2 value (in ohms, other end is connected to ground)

void setup_power_sources() {
    pinMode(THERMISTOR_PIN, INPUT_ANALOG);
    pinMode(BATTERY_PIN, INPUT_ANALOG);
    pinMode(CHARGER_PIN, INPUT_ANALOG);
}

uint8_t read_battery_temperature() {
    const double reading = analogRead(THERMISTOR_PIN) * (V_SUPPLY / 4095); // Voltage divider output
    const double resistance_ntc = reading * RESISTANCE_10K / (V_SUPPLY - reading); // Resistance of the thermistor
    const double kelvin = TEMP_0 * B_VALUE / (TEMP_0 * log(resistance_ntc / RESISTANCE_10K) + B_VALUE);
    return static_cast<uint8_t>(kelvin - 273.15); // Convert kelvin to celsius.
}

/**
 * @brief Reads a maximum of 10 volts at the provided pin.
 * @param pin The analog input pin for measurement.
 * @return Voltage reading at the pin.
 */
float read_voltage(const uint32_t pin) {
    const double reading = analogRead(pin);
    const double v_out = reading * V_SUPPLY / 4095.0;
    return static_cast<float>(v_out / (V_DIVIDER_R2 / (V_DIVIDER_R1 + V_DIVIDER_R2)));
}

float read_battery_voltage() { return read_voltage(BATTERY_PIN); }

float read_charger_voltage() { return read_voltage(CHARGER_PIN); }
