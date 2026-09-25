#ifndef CCSDS_STATUS_H
#define CCSDS_STATUS_H


typedef enum {
    CCSDS_OK = 0,
    CCSDS_BUFFER_TOO_SMALL,
    CCSDS_TRUNCATED,
    CCSDS_INVALID_LENGTH,
    CCSDS_CRC_FAILED,        // TM only
    CCSDS_NO_SYNC            // TM only
} CcsdsStatus;


#endif