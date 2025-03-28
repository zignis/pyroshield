#pragma once

#include <Arduino.h>
#include <TinyGPSPlus.h>

/**
 * @class GPS
 */
class GPS {
    TinyGPSInteger satellites; /// Number of satellites in view.
    TinyGPSLocation location; /// GPS latitude and longitude.
    TinyGPSDate date; /// GPS date.
    TinyGPSTime time; /// GPS time.
    TinyGPSAltitude altitude; /// GPS altitude.
    uint32_t last_read; /// Timestamp of the last GPS data update.

public:
    /**
     * @brief Constructs a GPS object with default values.
     */
    GPS();

    /**
     * @brief Sets the number of satellites in view.
     * @param satellites The number of satellites.
     */
    void set_satellites(const TinyGPSInteger &satellites);

    /**
     * @brief Sets the GPS location (latitude and longitude).
     * @param location The GPS location.
     */
    void set_location(const TinyGPSLocation &location);

    /**
     * @brief Sets the GPS date.
     * @param date The GPS date.
     */
    void set_date(const TinyGPSDate &date);

    /**
     * @brief Sets the GPS time.
     * @param time The GPS time.
     */
    void set_time(const TinyGPSTime &time);

    /**
     * @brief Sets the GPS altitude.
     * @param altitude The GPS altitude.
     */
    void set_altitude(const TinyGPSAltitude &altitude);

    /**
     * @brief Updates the timestamp of the last GPS data update.
     */
    void update_last_read();

    /**
     * @return The number of satellites.
     */
    [[nodiscard]] TinyGPSInteger get_satellites() const;

    /**
     * @return The GPS location.
     */
    [[nodiscard]] TinyGPSLocation get_location() const;

    /**
     * @return The GPS date.
     */
    [[nodiscard]] TinyGPSDate get_date() const;

    /**
     * @return The GPS time.
     */
    [[nodiscard]] TinyGPSTime get_time() const;

    /**
     * @return The GPS altitude.
     */
    [[nodiscard]] TinyGPSAltitude get_altitude() const;

    /**
     * @return The timestamp of the last GPS update.
     */
    [[nodiscard]] uint32_t get_last_read() const;

    /**
     * @return Comma-separated latitude and longitude.
     * @param decimal_places The round-off parameter.
     */
    String format_lat_lng(uint decimal_places);

    /**
     * @return The current date in DD/MM/YYYY format.
     */
    String format_date();

    /**
     * @return The current time in HH:MM:SS format.
     */
    String format_time();
};

/**
 * @brief Initializes the GPS module.
 */
void setup_gps();

/**
 * @brief Updates the GPS object by reading GPS data from the receiver.
 */
void update_gps_object();
