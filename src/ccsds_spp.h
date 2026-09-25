#ifndef CCSDS_SSP_H
#define CCSDS_SSP_H

#include <stdint.h>
#include <stdlib.h> 
#include <stddef.h>

#include "byteorder.h"
#include "ccsds_status.h"


#define CCSDS_SPP_PRIMARY_HEADER_SIZE 6

typedef struct {
    // word 0
    uint8_t version;        // 3 bits
    uint8_t type;           // 1 bit (0 = tm, 1 = tc)
    uint8_t secHdrFlag;     // 1 bits
    uint16_t apid;          // 11 bits
    // word 1
    uint8_t  seqFlags;      // 2 bits
    uint16_t seqCount;      // 14 bits
    // word 2
    uint16_t dataLength;    // 16 bits - payload minus one
} SpacePacketHeader;

void sppEncodeHeader(const SpacePacketHeader* h, uint8_t* out);
void sppDecodeHeader(const uint8_t* in, SpacePacketHeader* h);

CcsdsStatus sppEncodePacket(const SpacePacketHeader *h,
                        const uint8_t *payload, size_t payloadLen,
                        uint8_t *out, size_t outLen,
                        size_t *bytesWritten);

CcsdsStatus sppDecodePacket(const uint8_t *in, size_t inLen,
                         SpacePacketHeader *h,
                         const uint8_t **payload, size_t *payloadLen);

#endif