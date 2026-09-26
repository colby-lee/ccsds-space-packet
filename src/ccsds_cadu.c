#include "ccsds_cadu.h"
#include "byteorder.h"


CcsdsStatus caduWrap(uint8_t* out, size_t outLen) {

    if (outLen < CCSDS_CADU_SYNC_SIZE) {
        return CCSDS_BUFFER_TOO_SMALL;
    }

    writeU32BE(out, CCSDS_CADU_SYNC_MARKER);

    return CCSDS_OK;
}


CcsdsStatus caduUnwrap(const uint8_t* cadu, size_t caduLen, 
                       const uint8_t **frame, size_t* frameLen) {
    //
    if (caduLen <= CCSDS_CADU_SYNC_SIZE) {
        return CCSDS_TRUNCATED;
    }

    if (readU32BE(cadu) != CCSDS_CADU_SYNC_MARKER) {
        return CCSDS_NO_SYNC;
    }

    *frame = cadu + CCSDS_CADU_SYNC_SIZE;
    *frameLen = caduLen - CCSDS_CADU_SYNC_SIZE;

    return CCSDS_OK;
}
