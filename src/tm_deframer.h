#ifndef TM_DEFRAMER_H
#define TMDEFRAMER_H


#include <stdint.h>
#include <stddef.h>
#include "ccsds_status.h"
#include "ccsds_tm.h"


typedef enum {
    DEFRAMER_SCANNING,
    DEFRAMER_LOCKED
} DeframerState;

typedef struct {
    DeframerState state;
    uint32_t reg;                           // sync shift register
    uint8_t frame[CCSDS_TM_FRAME_LENGTH];
    size_t frameBytes;

    // stats
    uint32_t framesAccepted;
    uint32_t crcFailures;
    uint32_t syncLosses;
} TmDeframer;


#endif