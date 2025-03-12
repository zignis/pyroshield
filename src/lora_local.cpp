#include <LoRa.h>
#include <Scheduler.h>

/**
 * Packet structure
 *
 * [byte] forwarder_id
 * [byte] transmitter_id
 * [byte] message_id
 * [bool] allow_forwarding
 * [byte] ttl
 */

#define LORA_FREQ 433E6 // LoRa module frequency (433 MHz)
#define PACKET_TTL 5 // Time to live for a packet

extern byte device_id;
byte msg_count = 0;
bool allow_forwarding = true;

void setup_lora(const int sync_word, const int ss, const int reset, const int dio0) {
    LoRa.setPins(ss, reset, dio0);
    LoRa.setSyncWord(sync_word);

    if (!LoRa.begin(LORA_FREQ)) {
        Serial.println("Failed to initialize LoRa");
        exit(EXIT_FAILURE);
    }

    Serial.println("LoRa initialized");
}

void send_lora_message(const String &payload) {
    LoRa.beginPacket();

    LoRa.write(device_id); // Current device ID
    LoRa.write(device_id); // ID of the device that transmitted this message.
    LoRa.write(msg_count); // Message ID
    LoRa.write(allow_forwarding); // Forwarding flag
    LoRa.write(PACKET_TTL); // TTL

    LoRa.write(payload.length());
    LoRa.print(payload);

    LoRa.endPacket();

    msg_count++;
}

void handle_lora_reception() {
    const int packet_size = LoRa.parsePacket();

    if (packet_size == 0)
        return;

    const byte forwarder_id = LoRa.read(); // Transmitter / forwarder device ID
    const byte transmitter_id = LoRa.read(); // Original transmitter ID
    const byte msg_id = LoRa.read();
    const bool allow_forwarding = LoRa.read(); // Forwarding flag
    byte ttl = LoRa.read(); // TTL value

    const byte msg_size = LoRa.read();

    // Skip messages that do not allow forwarding
    if (!allow_forwarding) {
        return;
    }

    // Do not forward packets sent by this device
    if (forwarder_id == device_id || transmitter_id == device_id) {
        return;
    }

    // Drop stuck packet
    if (--ttl == 0) {
        return;
    }

    String incoming = "";
    const String preamble =
            "[LoRa][" + String(forwarder_id, HEX) + "][" + String(transmitter_id, HEX) + ":" + String(msg_id) + "]";

    while (LoRa.available()) {
        incoming += static_cast<char>(LoRa.read());
    }

    if (msg_size != incoming.length()) {
        Serial.println(preamble + ": message length does not match");
        return;
    }

    Serial.println(preamble + ": forwarding packet (" + String(msg_size) + " bytes)...");
    Serial.println(preamble + ": outgoing TTL " + ttl);
    Serial.println(preamble + ": RSSI " + String(LoRa.packetRssi()));
    Serial.println(preamble + ": SNR " + String(LoRa.packetSnr()));

    // Forward the packet
    LoRa.beginPacket();

    LoRa.write(device_id);
    LoRa.write(transmitter_id);
    LoRa.write(msg_id);
    LoRa.write(allow_forwarding);
    LoRa.write(ttl);

    LoRa.write(msg_size);
    LoRa.print(incoming);

    LoRa.endPacket();

    Serial.println(preamble + ": successfully forwarded");
    Serial.println();

    SchedulerClass::yield();
}
