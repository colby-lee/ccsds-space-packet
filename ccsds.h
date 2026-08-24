#ifndef CCSDS_H
#define CCSDS_H

#include <stdint.h>
#include <stdlib.h> 
#include <stddef.h>


#define CCSDS_PRIMARY_HEADER_SIZE 6

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


typedef enum {
    CCSDS_OK = 0,
    CCSDS_BUFFER_TOO_SMALL,
    CCSDS_TRUNCATED,
    CCSDS_INVALID_LENGTH
} CcsdsStatus;


uint16_t readU16BE(const uint8_t* p);
uint32_t readU32BE(const uint8_t* p);

void writeU16BE(uint8_t* p, uint16_t v);
void writeU32BE(uint8_t* p, uint32_t v);

void encodeHeader(const SpacePacketHeader* h, uint8_t* out);
void decodeHeader(const uint8_t* in, SpacePacketHeader* h);

CcsdsStatus encodePacket(const SpacePacketHeader *h,
                         const uint8_t *payload, size_t payloadLen,
                         uint8_t *out, size_t outLen,
                         size_t *bytesWritten);

#endif