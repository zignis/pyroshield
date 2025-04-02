#include <Adafruit_BMP280.h>
#include <HardwareSerial.h>

extern HardwareSerial GlobalSerial;

#define BMP_SCK (PB8)
#define BMP_MOSI (PB9)
#define BMP_MISO (PC15)
#define BMP_CS (PC14)

Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);

void setup_bmp280() {
    if (!bmp.begin(0x76)) {
        GlobalSerial.print("BMP280 not found. SensorID was: 0x");
        GlobalSerial.println(bmp.sensorID(), 16);
        exit(EXIT_FAILURE);
    }

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, Adafruit_BMP280::SAMPLING_X2, Adafruit_BMP280::SAMPLING_X16,
                    Adafruit_BMP280::FILTER_X16, Adafruit_BMP280::STANDBY_MS_2000);

    GlobalSerial.println("BMP280 initialized");
}

float read_bmp280_temperature() { return bmp.readTemperature(); }

float read_bmp280_pressure() { return bmp.readPressure() / 100.0F; }

float read_bmp280_altitude() { return bmp.readAltitude(); }
