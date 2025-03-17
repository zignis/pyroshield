#include <HardwareSerial.h>
#include <LoRa.h>
#include <lora_local.h>

#define LORA_FREQ 433E6 // LoRa module frequency (433 MHz)
#define PACKET_TTL 5 // Time to live for a packet

extern HardwareSerial GlobalSerial;
extern byte device_id;

uint16_t msg_count = 0;
bool allow_forwarding = true;

void setup_lora(const int sync_word, const int ss, const int reset, const int dio0) {
    LoRa.setPins(ss, reset, dio0);
    LoRa.setSyncWord(sync_word);

    if (!LoRa.begin(LORA_FREQ)) {
        GlobalSerial.println("Failed to initialize LoRa");
        exit(EXIT_FAILURE);
    }

    GlobalSerial.println("LoRa initialized");
}

void send_lora_message(LoRa_Payload payload) {
    payload.forwarder_id = device_id;
    payload.transmitter_id = device_id;
    payload.message_id = msg_count++;
    payload.allow_forwarding = allow_forwarding;
    payload.ttl = PACKET_TTL;

    LoRa.beginPacket();
    LoRa.write(reinterpret_cast<byte *>(&payload), sizeof(payload));
    LoRa.endPacket();

    GlobalSerial.println("[LoRa][" + String(payload.message_id) + "]: message sent (" + String(sizeof(payload)) +
                         " bytes)");
}

void handle_lora_reception() {
    const int packet_size = LoRa.parsePacket();

    if (packet_size == 0)
        return;

    LoRa_Payload payload;
    LoRa.readBytes(reinterpret_cast<byte *>(&payload), packet_size);

    // Skip messages that do not allow forwarding
    if (!payload.allow_forwarding) {
        return;
    }

    // Do not forward packets sent by this device
    if (payload.forwarder_id == device_id || payload.transmitter_id == device_id) {
        return;
    }

    // Drop stuck packet
    if (--payload.ttl == 0) {
        return;
    }

    const String preamble = "[LoRa][" + String(payload.forwarder_id, HEX) + "][" + String(payload.transmitter_id, HEX) +
                            ":" + String(payload.message_id) + "]";

    GlobalSerial.println(preamble + ": forwarding packet (" + String(packet_size) + " bytes)...");
    GlobalSerial.println(preamble + ": outgoing TTL " + payload.ttl);
    GlobalSerial.println(preamble + ": RSSI " + String(LoRa.packetRssi()));
    GlobalSerial.println(preamble + ": SNR " + String(LoRa.packetSnr()));

    // Forward the packet
    LoRa.beginPacket();
    payload.forwarder_id = device_id;
    LoRa.write(reinterpret_cast<byte *>(&payload), sizeof(payload));
    LoRa.endPacket(true); // Non-blocking

    GlobalSerial.println(preamble + ": successfully forwarded");
    GlobalSerial.println();
}
