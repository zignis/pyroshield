#include <Arduino.h>

#define THERMISTOR_PIN PA0 // ADC-0 pin
#define BATTERY_PIN PA1 // ADC-1 pin
#define CHARGER_PIN PB0 // ADC-8 pin

#define V_SUPPLY 3.04 // Supply voltage
#define V_REF 1.2 // Reference voltage
#define V_DIV_R1 33000.0 // Voltage divider resistor-1 value (in ohms, other end is connected to input voltage)
#define V_DIV_R2 15000.0 // Voltage divider resistor-2 value (in ohms, other end is connected to ground)

#define THERMISTOR_NOMINAL 10000 // Thermistor resistance at 25°C (in ohms)
#define THERMISTOR_SERIES_RESISTOR 10000 // Value of the resistor connected to the thermistor.
#define TEMPERATURE_NOMINAL 25 // Temperature for nominal resistance (almost always 25°C)
#define TEMP_0 273.15 // 0°C in kelvin
#define B_COEFFICIENT 3950 // B-coefficient of 10k thermistor

#define ADC_RES_UPPER_BOUND (pow(2, ADC_RESOLUTION) - 1) // Max value of the ADC.

void setup_power_sources() {
    analogReadResolution(ADC_RESOLUTION);

    pinMode(THERMISTOR_PIN, INPUT_PULLDOWN);
    pinMode(BATTERY_PIN, INPUT_PULLDOWN);
    pinMode(CHARGER_PIN, INPUT_PULLDOWN);
}

uint8_t read_battery_temperature() {
    auto reading = static_cast<float>(analogRead(THERMISTOR_PIN));
    reading = ADC_RES_UPPER_BOUND / reading - 1;
    reading = THERMISTOR_SERIES_RESISTOR / reading;

    double steinhart = reading / THERMISTOR_NOMINAL; // (R/Ro)
    steinhart = log(steinhart); // ln(R/Ro)
    steinhart /= B_COEFFICIENT; // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURE_NOMINAL + TEMP_0); // add (1/To)
    steinhart = 1.0 / steinhart; // Invert
    steinhart -= TEMP_0; // Convert absolute temp to celsius.

    return static_cast<uint8_t>(steinhart);
}

/**
 * @brief Reads a maximum of 10 volts at the provided pin.
 * @param pin The analog input pin for measurement.
 * @return Voltage reading at the pin.
 */
float read_voltage(const uint32_t pin) {
    const double reading = analogRead(pin);
    const double voltage = reading * (V_REF * V_SUPPLY / ADC_RES_UPPER_BOUND) * ((V_DIV_R1 + V_DIV_R2) / V_DIV_R2);
    return static_cast<float>(voltage);
}

float read_battery_voltage() { return read_voltage(BATTERY_PIN); }

float read_charger_voltage() { return read_voltage(CHARGER_PIN); }
