
#include <stdint.h>
#include <assert.h>
#include <stdio.h> 
#include <string.h>

#include "ccsds_spp.h"
#include "ccsds_tm.h"


int main() {
    
    printf("Testing space packet protocol implementation...\n");

    SpacePacketHeader v1 = {0, 0, 1, 100, 3, 1, 9};
    uint8_t expected[6] = {0x08, 0x64, 0xC0, 0x01, 0x00, 0x09};
    uint8_t got[6];
    sppEncodeHeader(&v1, got);
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
        sppEncodeHeader(&h, buf);

        SpacePacketHeader out;

        sppDecodeHeader(buf, &out);

        assert(out.version == h.version);
        assert(out.secHdrFlag == h.secHdrFlag);
        assert(out.type == h.type);
        assert(out.apid == h.apid);

        assert(out.seqCount == h.seqCount);
        assert(out.seqFlags == h.seqFlags);

        assert(out.dataLength == h.dataLength);
    }

    printf("space packet protocol tests passed\n");

    /// ----------------------------------------------------------------------------

    printf("Testing TM frame header implementation...\n");

    TMFrameHeader tm1 = {1, 17, 3, 0, 45, 66, 0, 1, 0, 2, 64};
    uint8_t expectedTm[6] = {0x41, 0x16, 0x2d, 0x42, 0x50, 0x40};
    uint8_t gotTm[6];
    tmEncodeHeader(&tm1, gotTm);
    assert(memcmp(gotTm, expectedTm, 6) == 0);

    for (int i = 0; i < 1000; ++i) {
        TMFrameHeader h;
        h.version                  = rand() & 0x03;
        h.spacecraftId             = rand() & 0x3FF;
        h.virtualChannel           = rand() & 0x07;
        h.ocfFlag                  = rand() & 0x01;
        h.masterChannelFrameCount  = (uint8_t)(rand() & 0xFF);
        h.virtualChannelFrameCount = (uint8_t)(rand() & 0xFF);
        h.secHdrFlag               = rand() & 0x01;
        h.syncFlag                 = rand() & 0x01;
        h.pktOrderFlag             = rand() & 0x01;
        h.segLengthId              = rand() & 0x03;
        h.firstHeaderPointer       = rand() & 0x7FF;

        uint8_t buf[6];
        tmEncodeHeader(&h, buf);

        TMFrameHeader out;

        tmDecodeHeader(buf, &out);

        assert(out.version == h.version);
        assert(out.spacecraftId == h.spacecraftId);
        assert(out.virtualChannel == h.virtualChannel);
        assert(out.ocfFlag == h.ocfFlag);
        assert(out.masterChannelFrameCount == h.masterChannelFrameCount);
        assert(out.virtualChannelFrameCount == h.virtualChannelFrameCount);
        assert(out.secHdrFlag == h.secHdrFlag);
        assert(out.syncFlag == h.syncFlag);
        assert(out.pktOrderFlag == h.pktOrderFlag);
        assert(out.segLengthId == h.segLengthId);
        assert(out.firstHeaderPointer == h.firstHeaderPointer);
    }

    printf("TM frame header tests passed...\n");

    //----------------------------------------------------------------------

    printf("Testing TM CRC...\n");

    const char *msg = "123456789";
    uint16_t crc = tmComputeCrc16((const uint8_t*)msg, 9);
    printf("crc = %04X\n", crc);
    assert(crc == 0x29B1);

    printf("TM CRC test passed\n");

    //---------------------------------------------------------------------

    printf("Testing TM Frame encoding...\n");

    TMFrameHeader tmh = {1, 17, 3, 0, 45, 66, 0, 1, 0, 2, 64};
    SpacePacketHeader spph = {0, 0, 1, 100, 3, 1, 9};

    uint8_t packet[6];
    sppEncodeHeader(&spph, packet);

    uint8_t out[128];
    size_t bytesWritten = 0;

    CcsdsStatus st = tmEncodeFrame(&tmh, packet, sizeof(packet),
                                out, sizeof(out), &bytesWritten);
    assert(st == CCSDS_OK);
    assert(bytesWritten == CCSDS_TM_FRAME_LENGTH);

    TMFrameHeader tmh2;
    const uint8_t* data = NULL;
    size_t dataLen = 0;

    CcsdsStatus st2 = tmDecodeFrame(out, bytesWritten, &tmh2,
                                &data, &dataLen);

    assert(st2 == CCSDS_OK);
    assert(dataLen == CCSDS_TM_DATA_FIELD_SIZE);
    assert(memcmp(data, packet, sizeof(packet)) == 0);


    assert(tmh2.version == tmh.version);
    assert(tmh2.spacecraftId == tmh.spacecraftId);
    assert(tmh2.virtualChannel == tmh.virtualChannel);
    assert(tmh2.ocfFlag == tmh.ocfFlag);
    assert(tmh2.masterChannelFrameCount == tmh.masterChannelFrameCount);
    assert(tmh2.virtualChannelFrameCount == tmh.virtualChannelFrameCount);
    assert(tmh2.secHdrFlag == tmh.secHdrFlag);
    assert(tmh2.syncFlag == tmh.syncFlag);
    assert(tmh2.pktOrderFlag == tmh.pktOrderFlag);
    assert(tmh2.segLengthId == tmh.segLengthId);
    assert(tmh2.firstHeaderPointer == tmh.firstHeaderPointer);


    out[50] ^= 0xFF;
    assert(tmDecodeFrame(out, bytesWritten, &tmh2, &data, &dataLen) == CCSDS_CRC_FAILED);
    out[50] ^= 0xFF;
    
    printf("TM Frame encoding tests passed\n");


    return 0;
}