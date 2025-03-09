#pragma once

/**
 * @brief Initializes the LoRa module.
 * @param ss The NSS pin.
 * @param reset The NRESET pin.
 * @param dio0 The DIO0 pin.
 */
void setup_lora(int ss, int reset, int dio0);
