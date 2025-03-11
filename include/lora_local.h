#pragma once

/**
 * @brief Initializes the LoRa module.
 * @param sync_word The sync word to isolate the network.
 * @param ss The NSS pin.
 * @param reset The NRESET pin.
 * @param dio0 The DIO0 pin.
 */
void setup_lora(int sync_word, int ss, int reset, int dio0);

/**
 * @brief Sends a LoRa message.
 * @param payload The payload for the LoRa message.
 */
void send_lora_message(String payload);

/**
 * @brief Incoming LoRa messages handler.
 */
void handle_lora_reception();
