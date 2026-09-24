
#include <stdint.h>
#include <assert.h>
#include <stdio.h> 
#include <string.h>

#include "ssp_ccsds.h"


int main() {
    printf("Hello, space packets\n");

    SpacePacketHeader v1 = {0, 0, 1, 100, 3, 1, 9};
    uint8_t expected[6] = {0x08, 0x64, 0xC0, 0x01, 0x00, 0x09};
    uint8_t got[6];
    sspEncodeHeader(&v1, got);
    assert(memcmp(got, expected, 6) == 0);

    for (int i = 0; i < 1000; ++i) {
        SpacePacketHeader h;
        h.version    = rand() & 0x07;
        h.type       = rand() & 0x01;
        h.secHdrFlag = rand() & 0x01;
        h.apid       = rand() & 0x07FF;
        h.seqFlags   = rand() & 0x03;
        h.seqCount   = rand() & 0x3FFF;
        h.dataLength = (uint16_t)(rand() & 0xFFFF);

        uint8_t buf[6];
        sspEncodeHeader(&h, buf);

        SpacePacketHeader out;

        sspDecodeHeader(buf, &out);

        assert(out.version == h.version);
        assert(out.secHdrFlag == h.secHdrFlag);
        assert(out.type == h.type);
        assert(out.apid == h.apid);

        assert(out.seqCount == h.seqCount);
        assert(out.seqFlags == h.seqFlags);

        assert(out.dataLength == h.dataLength);
    }

    printf("all tests passed\n");

    return 0;
}