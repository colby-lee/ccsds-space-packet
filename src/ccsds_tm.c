#include "ccsds_tm.h"

void tmEncodeHeader(const TMFrameHeader* h, uint8_t* out) {
    uint16_t w0 = 0;
    uint16_t w1 = 0;
    uint16_t w2 = 0;

    w0 |= h->ocfFlag & 0x01;
    w0 |= (h->virtualChannel & 0x07) << 1;
    w0 |= (h->spacecraftId & 0x3FF) << 4;
    w0 |= (h->version & 0x3) << 14;

    w1 |= h->virtualChannelFrameCount & 0xFF;
    w1 |= (h->masterChannelFrameCount & 0xFF) << 8;

    w2 |= h->firstHeaderPointer & 0x7FF;
    w2 |= (h->segLengthId & 0x3) << 11;
    w2 |= (h->pktOrderFlag & 0x1) << 13;
    w2 |= (h->syncFlag & 0x1) << 14;
    w2 |= (h->secHdrFlag & 0x1) << 15;

    writeU16BE(out, w0);
    writeU16BE(out + 2, w1);
    writeU16BE(out + 4, w2);
}

void tmDecodeHeader(const uint8_t *in, TMFrameHeader *h) {
    uint16_t w0 = readU16BE(in);
    uint16_t w1 = readU16BE(in + 2);
    uint16_t w2 = readU16BE(in + 4);

    h->ocfFlag = w0 &0x01;
    h->virtualChannel = (w0 >> 1) & 0x07;
    h->spacecraftId = (w0 >> 4) & 0x3FF;
    h->version = (w0 >> 14) & 0x3;

    h->virtualChannelFrameCount = (uint8_t)(w1 & 0xFF);
    h->masterChannelFrameCount = (uint8_t)((w1 >> 8) & 0xFF);

    h->firstHeaderPointer = w2 & 0x7FF;
    h->segLengthId = (w2 >> 11) & 0x3;
    h->pktOrderFlag = (w2 >> 13) & 0x1;
    h->syncFlag = (w2 >> 14) & 0x1;
    h->secHdrFlag = (w2 >> 15) & 0x1;
}

CcsdsStatus tmEncodeFrame(const TMFrameHeader* h,
                        const uint8_t* data, size_t dataLen,
                        uint8_t* out, size_t outLen, 
                        size_t* bytesWritten) {

    if (dataLen == 0 || dataLen > 2048) {
        return CCSDS_INVALID_LENGTH;
    }

    if (outLen < CCSDS_TM_PRIMARY_HEADER_SIZE + dataLen) {
        return CCSDS_BUFFER_TOO_SMALL;
    }
    
    tmEncodeHeader(h, out);
    memcpy(out + CCSDS_TM_PRIMARY_HEADER_SIZE, data, dataLen);

    *bytesWritten = CCSDS_TM_PRIMARY_HEADER_SIZE + dataLen;

    return CCSDS_OK;

}