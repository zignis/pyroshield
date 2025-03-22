#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <HardwareSerial.h>

#define DHT_PIN PB7
#define DHT_TYPE DHT22

extern HardwareSerial GlobalSerial;

DHT_Unified dht(DHT_PIN, DHT_TYPE);
sensors_event_t dht_event;

void setup_dht22() { dht.begin(); }

float read_dht22_temperature() {
    dht.temperature().getEvent(&dht_event);

    if (isnan(dht_event.temperature)) {
        return 0;
    }

    return dht_event.temperature;
}

float read_dht22_humidity() {
    dht.humidity().getEvent(&dht_event);

    if (isnan(dht_event.relative_humidity)) {
        return 0;
    }

    return dht_event.relative_humidity;
}
