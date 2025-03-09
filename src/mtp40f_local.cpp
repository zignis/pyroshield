#include <MTP40F.h>

#define MTP40F_READ_INTERVAL 2000 // Measurement interval
#define MTP40F_BAUD_RATE 9600

USARTClass mtp40f_usart = Serial1;
MTP40F mtp(&mtp40f_usart);

void setup_mtp40f() {
    mtp40f_usart.begin(MTP40F_BAUD_RATE);

    if (mtp.begin() == false) {
        Serial.println("Failed to initialize MTP40F");
        exit(EXIT_FAILURE);
    }

    Serial.println("MTP40F initialized");
}

bool has_mtp40f_updated() { return millis() - mtp.lastRead() >= MTP40F_READ_INTERVAL; }

uint32_t read_mtp40f_gas_concentration() { return mtp.getGasConcentration(); }
