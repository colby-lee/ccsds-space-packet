#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/adc.h"
#include  "ccsds.h"          

int main(void) {
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    SpacePacketHeader h = {
        .version = 0,
        .type = 0,
        .secHdrFlag = 0,
        .apid = 100,
        .seqFlags = 3,
        .seqCount = 0,
        .dataLength = 0,
    };

    static uint16_t seqCount = 0;

    while (true) {
        uint16_t raw = adc_read();

        uint8_t payload[6];

        writeU32BE(payload, to_ms_since_boot(get_absolute_time()));
        writeU16BE(payload + 4, raw);
        h.seqCount = seqCount++ & 0x3FFF;
        

        uint8_t packet[12];
        size_t packetLen;
        
        if (encodePacket(&h, payload, 6, packet, sizeof(packet), &packetLen) != CCSDS_OK) {
            printf("encode failed\n");
            continue;
        }

        for (size_t i = 0; i < packetLen; i++) {
            printf("%02X", packet[i]);
        }
        printf("\n");

        sleep_ms(1000);
    }
}
