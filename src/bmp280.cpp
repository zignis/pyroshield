#include <Adafruit_BMP280.h>
#include <HardwareSerial.h>

extern HardwareSerial GlobalSerial;
extern int8_t BMP280_CS;

Adafruit_BMP280 bmp(BMP280_CS);

void setup_bmp280() {
    if (!bmp.begin(0x76)) {
        GlobalSerial.print("BMP280 not found. SensorID was: 0x");
        GlobalSerial.println(bmp.sensorID(), 16);
        exit(EXIT_FAILURE);
    }

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, Adafruit_BMP280::SAMPLING_X2, Adafruit_BMP280::SAMPLING_X16,
                    Adafruit_BMP280::FILTER_X16, Adafruit_BMP280::STANDBY_MS_500);

    GlobalSerial.print("BMP280 initialized");
}

float read_bmp280_temperature() { return bmp.readTemperature(); }

float read_bmp280_pressure() { return bmp.readPressure() / 100.0F; }

float read_bmp280_altitude() { return bmp.readAltitude(); }
