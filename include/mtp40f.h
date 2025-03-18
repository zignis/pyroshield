#pragma once

// Adapted from https://github.com/RobTillaart/MTP40F

#include <Arduino.h>

#define MTP40F_OK 0x00
#define MTP40F_INVALID_AIR_PRESSURE 0x01
#define MTP40F_INVALID_GAS_LEVEL 0x02
#define MTP40F_INVALID_CRC 0x10
#define MTP40F_REQUEST_FAILED 0xFFFF

/**
 * @class MTP40F
 */
class MTP40F {
public:
    /**
     * @brief Constructs a new MTP40F object.
     * @param stream Pointer to a Stream object.
     */
    explicit MTP40F(Stream *stream);

    /**
     * @brief Initializes the sensor.
     * @return true if successful, false otherwise.
     */
    bool begin();

    /**
     * @return Air pressure reference value in hPa.
     */
    int get_air_pressure_reference();

    /**
     * @brief Sets the air pressure reference value.
     * @param apr Air pressure reference value in hPa (default: 1013).
     * @return true if successful, false otherwise.
     */
    bool set_air_pressure_reference(int apr = 1013);

    /**
     * @return CO2 concentration in PPM.
     */
    uint16_t get_gas_concentration();

    /**
     * @brief Enables or disables error suppression.
     * @param value Suppression flag.
     */
    void suppress_error(const bool value) { _suppress_error = value; };

    /**
     * @brief Checks if error suppression is enabled.
     * @return Suppression flag.
     */
    [[nodiscard]] bool get_suppress_error() const { return _suppress_error; };

    /**
     * @brief Enables self-calibration mode.
     * @return true if successful, false otherwise.
     */
    bool open_self_calibration();

    /**
     * @brief Disables self-calibration mode.
     * @return true if successful, false otherwise.
     */
    bool close_self_calibration();

    /**
     * @return Status of self calibration.
     */
    uint8_t get_self_calibration_status();

    /**
     * @brief Sets the number of hours for self-calibration cycle.
     * @param hours Number of hours (default: 168).
     * @return true if successful, false otherwise.
     */
    bool set_self_calibration_hours(uint16_t hours = 168);

    /**
     * @return Currently set number of hours for self-calibration.
     */
    uint16_t get_self_calibration_hours();

    /**
     * @brief Sets the timeout for serial communication.
     * @param timeout Timeout duration in ms (default: 100).
     */
    void set_timeout(const uint32_t timeout = 100) { _timeout = timeout; };

    /**
     * @return Timeout duration for serial communication in ms.
     */
    [[nodiscard]] uint32_t get_timeout() const { return _timeout; };

    /**
     * @return Timestamp of the last successful read (in ms).
     */
    [[nodiscard]] uint32_t last_read() const { return _last_read; };

    /**
     * @return Last recorded error code value.
     */
    int last_error();

protected:
    Stream *_stream; /// Pointer to the communication stream.
    uint8_t _buffer[16]{}; /// Buffer for storing received data.
    uint32_t _timeout = 100; /// Serial communication timeout (in ms).
    uint32_t _last_read = 0; /// Timestamp of the last successful read.
    int _air_pressure_reference = 0; /// Air pressure reference value (in hPa).
    uint16_t _gas_level = 0; /// CO2 concentration value (in PPM).
    bool _suppress_error = false; /// Flag for suppressing errors.
    int _last_error = MTP40F_OK; /// Last recorded error code.

    /**
     * @brief Sends a request to the sensor and processes the response.
     * @param data Pointer to the request data.
     * @param command_length Length of the request command.
     * @param response_length Expected length of the response.
     * @return true if successful, false otherwise.
     */
    bool request(uint8_t *data, uint8_t command_length, uint8_t response_length);

    /**
     * @brief Computes the CRC checksum for a given data buffer.
     * @param data Pointer to the data buffer.
     * @param length Length of the data buffer.
     * @return Computed CRC value.
     */
    static uint16_t compute_crc(const uint8_t *data, uint16_t length);
};
