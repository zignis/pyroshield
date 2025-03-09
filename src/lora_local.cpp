#include <LoRa.h>

#define LORA_FREQ 433E6 // LoRa module frequency (433 MHz)

void setup_lora(const int ss, const int reset, const int dio0) {
    LoRa.setPins(ss, reset, dio0);

    if (!LoRa.begin(LORA_FREQ)) {
        Serial.println("Failed to initialize LoRa");
        exit(EXIT_FAILURE);
    }

    Serial.println("LoRa initialized");
}
