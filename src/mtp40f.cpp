// Adapted from https://github.com/RobTillaart/MTP40F

#include <mtp40f.h>

MTP40F::MTP40F(Stream *stream) {
    _stream = stream;
    _buffer[0] = '\0';
}

bool MTP40F::begin() {
    _timeout = 100;
    _last_read = 0;
    _air_pressure_reference = 0;
    _gas_level = 0;
    _suppress_error = false;
    _last_error = MTP40F_OK;

    return true;
}

int MTP40F::get_air_pressure_reference() {
    _last_error = MTP40F_OK;

    if (uint8_t cmd[9] = {0x42, 0x4D, 0xA0, 0x00, 0x02, 0x00, 0x00, 0x01, 0x31}; request(cmd, 9, 11)) {
        _air_pressure_reference = _buffer[7] * 256 + _buffer[8];
        return _air_pressure_reference;
    }

    _last_error = MTP40F_INVALID_AIR_PRESSURE;

    if (_suppress_error) {
        return _air_pressure_reference;
    }

    return _last_error;
}

bool MTP40F::set_air_pressure_reference(const int apr) {
    if (apr < 700 || apr > 1100) {
        return false;
    }

    uint8_t cmd[11] = {0x42, 0x4D, 0xA0, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00};
    cmd[7] = apr / 256;
    cmd[8] = apr % 256;

    return request(cmd, 11, 11);
}

uint32_t MTP40F::get_gas_concentration() {
    _last_error = MTP40F_OK;

    // Maximum read freq. is once per 2 seconds
    if (millis() - _last_read < 2000) {
        return _gas_level; // Previous value
    }

    _last_read = millis();

    if (uint8_t cmd[9] = {0x42, 0x4D, 0xA0, 0x00, 0x03, 0x00, 0x00, 0x01, 0x32}; request(cmd, 9, 14)) {
        if (const uint8_t status = _buffer[11]; status == 0x00) {
            _gas_level = _buffer[7];
            _gas_level <<= 8;
            _gas_level |= _buffer[8];
            _gas_level <<= 8;
            _gas_level |= _buffer[9];
            _gas_level <<= 8;
            _gas_level += _buffer[10];
            return _gas_level;
        }

        _last_error = MTP40F_INVALID_GAS_LEVEL;

        if (_suppress_error) {
            return _gas_level; // Previous value
        }

        return _last_error;
    }

    _last_error = MTP40F_REQUEST_FAILED;

    if (_suppress_error) {
        return _gas_level; // Previous value
    }

    return _last_error;
}

bool MTP40F::open_self_calibration() {
    uint8_t cmd[10] = {0x42, 0x4D, 0xA0, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00};
    return request(cmd, 10, 9);
}

bool MTP40F::close_self_calibration() {
    uint8_t cmd[10] = {0x42, 0x4D, 0xA0, 0x00, 0x06, 0x00, 0x01, 0xFF, 0x00, 0x00};
    return request(cmd, 10, 9);
}

uint8_t MTP40F::get_self_calibration_status() {
    if (uint8_t cmd[9] = {0x42, 0x4D, 0xA0, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00}; request(cmd, 9, 10)) {
        return _buffer[7]; //  0x00 or 0xFF
    }

    _last_error = MTP40F_REQUEST_FAILED;
    return _last_error;
}

uint16_t MTP40F::get_self_calibration_hours() {
    if (uint8_t cmd[9] = {0x42, 0x4D, 0xA0, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00}; request(cmd, 9, 11)) {
        const uint16_t hours = _buffer[7] * 256 + _buffer[8];
        return hours;
    }

    _last_error = MTP40F_REQUEST_FAILED;
    return _last_error;
}

bool MTP40F::set_self_calibration_hours(const uint16_t hours) {
    if (hours < 24 || hours > 720) {
        return false;
    }

    uint8_t cmd[11] = {0x42, 0x4D, 0xA0, 0x00, 0x09, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00};
    cmd[7] = hours / 256;
    cmd[8] = hours & 0xFF;

    if (request(cmd, 11, 10)) {
        return _buffer[7] == 0x00;
    }

    return false;
}

int MTP40F::last_error() {
    const int error = _last_error;
    _last_error = MTP40F_OK;
    return error;
}

/// Protected

bool MTP40F::request(uint8_t *data, uint8_t command_length, const uint8_t response_length) {
    // Calculate CRC of command
    const uint16_t crc = compute_crc(data, command_length - 2);
    data[command_length - 2] = crc / 256;
    data[command_length - 1] = crc & 0xFF;

    while (command_length--) {
        _stream->write(*data++);
        yield();
    }

    const uint32_t start = millis();
    uint8_t i = 0;

    while (i < response_length) {
        if (millis() - start > _timeout) {
            return false;
        }

        if (_stream->available()) {
            _buffer[i++] = _stream->read();
        }

        yield();
    }

    if (response_length > 2) {
        // ReSharper disable once CppRedundantParentheses
        const uint16_t expected_crc = (_buffer[response_length - 2] << 8) | _buffer[response_length - 1];
        const uint16_t calc_crc = compute_crc(_buffer, response_length - 2);

        if (calc_crc != expected_crc) {
            _last_error = MTP40F_INVALID_CRC;
            return false;
        }

        return calc_crc == expected_crc;
    }

    return true;
}

uint16_t MTP40F::compute_crc(const uint8_t *data, const uint16_t length) {
    uint16_t sum = 0;

    for (uint16_t i = 0; i < length; i++) {
        sum += *data++;
    }

    return sum;
}
