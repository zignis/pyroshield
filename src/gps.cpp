#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include <gps.h>

#define GPS_BAUD_RATE 115200

extern HardwareSerial GlobalSerial;
auto GPS_Serial = HardwareSerial(USART3);
TinyGPSPlus tiny_gps;

GPS::GPS() { last_read = millis(); }

void GPS::set_satellites(const TinyGPSInteger &satellites) { this->satellites = satellites; }
void GPS::set_location(const TinyGPSLocation &location) { this->location = location; }
void GPS::set_date(const TinyGPSDate &date) { this->date = date; }
void GPS::set_time(const TinyGPSTime &time) { this->time = time; }
void GPS::set_altitude(const TinyGPSAltitude &altitude) { this->altitude = altitude; }
void GPS::update_last_read() { last_read = millis(); }

TinyGPSInteger GPS::get_satellites() const { return this->satellites; }
TinyGPSLocation GPS::get_location() const { return this->location; }
TinyGPSDate GPS::get_date() const { return this->date; }
TinyGPSTime GPS::get_time() const { return this->time; }
TinyGPSAltitude GPS::get_altitude() const { return this->altitude; }
uint32_t GPS::get_last_read() const { return this->last_read; }

String GPS::format_lat_lng(const uint decimal_places = 6) {
    if (location.isValid()) {
        return String(location.lat(), decimal_places) + "," + String(location.lng(), decimal_places);
    }

    return "Invalid";
}

String GPS::format_date() {
    if (date.isValid()) {
        return String(date.day()) + "/" + String(date.month()) + "/" + String(date.year());
    }

    return "Invalid";
}

/**
 * Prepends '0' if the input value is less than 10.
 * @param value Integer value.
 * @return Formatted integer value.
 */
String prepend_zero(const uint value) {
    if (value < 10) {
        return "0" + String(value);
    }

    return String(value);
}

String GPS::format_time() {
    if (time.isValid()) {
        return prepend_zero(time.hour()) + ":" + prepend_zero(time.minute()) + ":" + prepend_zero(time.second());
    }

    return "Invalid";
}

GPS gps;

void setup_gps() {
    GPS_Serial.begin(GPS_BAUD_RATE);
    GlobalSerial.println("GPS initialized");
}

void update_gps_object() {
    if (GPS_Serial.available() && tiny_gps.encode(GPS_Serial.read())) {
        gps.update_last_read();

        if (tiny_gps.satellites.isValid()) {
            gps.set_satellites(tiny_gps.satellites);
        }

        if (tiny_gps.location.isValid()) {
            gps.set_location(tiny_gps.location);
        }

        if (tiny_gps.date.isValid()) {
            gps.set_date(tiny_gps.date);
        }
        if (tiny_gps.time.isValid()) {
            gps.set_time(tiny_gps.time);
        }

        if (tiny_gps.altitude.isValid()) {
            gps.set_altitude(tiny_gps.altitude);
        }
    }
}

GPS get_gps_object() { return gps; }
