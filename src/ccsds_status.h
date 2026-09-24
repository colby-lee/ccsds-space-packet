#ifndef CCSDS_STATUS_H
#define CCSDS_STATUS_H


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


#endif