#include <DHT.h>
#include <HardwareSerial.h>

extern HardwareSerial GlobalSerial;
DHT dht;

void setup_dht22(const uint8_t pin) {
    dht.setup(pin, DHT::DHT22);
    GlobalSerial.println("DHT22 initialized");
}

int get_dht22_min_sampling_period() { return dht.getMinimumSamplingPeriod(); }

const char *read_dht22_status() { return dht.getStatusString(); }

float read_dht22_temperature() { return dht.getTemperature(); }

float read_dht22_humidity() { return dht.getHumidity(); }
