#include <HardwareSerial.h>
#include <MTP40F.h>

#define MTP40F_READ_INTERVAL 2000 // Measurement interval
#define MTP40F_BAUD_RATE 9600

extern HardwareSerial GlobalSerial;

auto MTP40F_Serial = HardwareSerial(USART2);
MTP40F mtp(&MTP40F_Serial);

void setup_mtp40f() {
    MTP40F_Serial.begin(MTP40F_BAUD_RATE);

    if (mtp.begin() == false) {
        GlobalSerial.println("Failed to initialize MTP40F");
        exit(EXIT_FAILURE);
    }

    GlobalSerial.println("MTP40F initialized");
}

bool has_mtp40f_updated() { return millis() - mtp.last_read() >= MTP40F_READ_INTERVAL; }

uint16_t read_mtp40f_gas_concentration() { return mtp.get_gas_concentration(); }
