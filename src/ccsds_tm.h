#ifndef CCSDS_TM_H
#define CCSDS_TM_H

#include <stdint.h>
#include <stdlib.h> 
#include <stddef.h>
#include <string.h>

#include "byteorder.h"
#include "ccsds_status.h"


#define CCSDS_TM_PRIMARY_HEADER_SIZE 6
#define CCSDS_TM_CRC_SIZE            2
#define CCSDS_TM_FRAME_LENGTH        128
#define CCSDS_TM_DATA_FIELD_SIZE \
    (CCSDS_TM_FRAME_LENGTH - CCSDS_TM_PRIMARY_HEADER_SIZE - CCSDS_TM_CRC_SIZE)
#define CCSDS_TM_CRC_COVERAGE (CCSDS_TM_PRIMARY_HEADER_SIZE + CCSDS_TM_DATA_FIELD_SIZE)

typedef struct {
    // word 0
    uint8_t version;                    // 2 bits
    uint16_t spacecraftId;              // 10 bits
    uint8_t virtualChannel;             // 3 bits
    uint8_t ocfFlag;                    // 1 bit
    // word 1
    uint8_t masterChannelFrameCount;    // 8 bits
    uint8_t virtualChannelFrameCount;   // 8 bits
    //word 2
    uint8_t  secHdrFlag;          // 1 bit
    uint8_t  syncFlag;            // 1 bit
    uint8_t  pktOrderFlag;        // 1 bit
    uint8_t  segLengthId;         // 2 bits
    uint16_t firstHeaderPointer;  // 11 bits    
} TMFrameHeader;


void tmEncodeHeader(const TMFrameHeader* h, uint8_t* out);

void tmDecodeHeader(const uint8_t* in, TMFrameHeader* h);

CcsdsStatus tmEncodeFrame(const TMFrameHeader* h,
                        const uint8_t* data, size_t dataLen,
                        uint8_t* out, size_t outLen, 
                        size_t* bytesWritten);

CcsdsStatus tmDecodeFrame(const uint8_t* in, size_t inLen, 
                        TMFrameHeader* h, 
                        const uint8_t** data, size_t* dataLen);


uint16_t tmComputeCrc16(const uint8_t *data, size_t len);


#endif