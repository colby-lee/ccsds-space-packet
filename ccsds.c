#include <string.h>

#include "ccsds.h"


void writeU16BE(uint8_t* p, uint16_t v) {
    p[0] = (uint8_t)((v >> 8) & 0xFF);
    p[1] = (uint8_t)(v & 0xFF);
}

void writeU32BE(uint8_t* p, uint32_t v) {
    p[0] = (uint8_t)((v >> 24) & 0xFF);
    p[1] = (uint8_t)((v >> 16) & 0xFF);
    p[2] = (uint8_t)((v >> 8) & 0xFF);
    p[3] = (uint8_t)(v & 0xFF);
}

uint16_t readU16BE(const uint8_t* p) {
    return ((uint16_t)(p[0]) << 8) | p[1];
}

uint32_t readU32BE(const uint8_t* p) {
    return (
        (uint32_t)(p[0]) << 24 |
        (uint32_t)(p[1]) << 16 |
        (uint32_t)(p[2]) << 8  |  
        (uint32_t)(p[3])
        );
}

void encodeHeader(const SpacePacketHeader* h, uint8_t* out) {
    uint16_t w0 = 0;
    uint16_t w1 = 0;
    uint16_t w2 = 0;

    w0 |= h->apid & 0x07FF;
    w0 |= (h->secHdrFlag & 0x01) << 11;
    w0 |= (h->type & 0x01) << 12;
    w0 |= (h->version & 0x07) << 13;

    w1 |= h->seqCount & 0x3FFF;
    w1 |= (h->seqFlags & 0x03) << 14;

    w2 = h->dataLength;

    writeU16BE(out, w0);
    writeU16BE(out + 2, w1);
    writeU16BE(out + 4, w2);
}

void decodeHeader(const uint8_t* in, SpacePacketHeader* h) {
    uint16_t w0 = readU16BE(in);
    uint16_t w1 = readU16BE(in + 2);
    uint16_t w2 = readU16BE(in + 4);

    h->apid = w0 & 0x07FF;
    h->secHdrFlag = (w0 >> 11) & 0x01;
    h->type = (w0 >> 12) & 0x01;
    h->version = (w0 >> 13) & 0x07;

    h->seqCount = (w1 & 0x3FFF);
    h->seqFlags = (w1 >> 14) & 0x03;

    h->dataLength = w2;
}

CcsdsStatus encodePacket(const SpacePacketHeader *h,
                         const uint8_t *payload, size_t payloadLen,
                         uint8_t *out, size_t outLen,
                         size_t *bytesWritten) {
    
    if (payloadLen == 0 || payloadLen > 65536) {
        return CCSDS_INVALID_LENGTH;
    }

    if (outLen < CCSDS_PRIMARY_HEADER_SIZE + payloadLen) {
        return CCSDS_BUFFER_TOO_SMALL;
    }

    SpacePacketHeader headerc = *h;
    headerc.dataLength =  (uint16_t)(payloadLen - 1);

    encodeHeader(&headerc, out);
    memcpy(out + CCSDS_PRIMARY_HEADER_SIZE, payload, payloadLen);

    *bytesWritten = CCSDS_PRIMARY_HEADER_SIZE + payloadLen;

    return CCSDS_OK;
}

CcsdsStatus decodePacket(const uint8_t *in, size_t inLen,
                         SpacePacketHeader *h,
                         const uint8_t **payload, size_t *payloadLen) {
    
    if (inLen < CCSDS_PRIMARY_HEADER_SIZE) {
        return CCSDS_TRUNCATED;
    }

    decodeHeader(in, h);

    size_t claimedLen = h->dataLength + 1;

    if (claimedLen + CCSDS_PRIMARY_HEADER_SIZE > inLen) {
        return CCSDS_TRUNCATED;
    }

    *payload = in + 6;
    *payloadLen = claimedLen;

    return CCSDS_OK;
}