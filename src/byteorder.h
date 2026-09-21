#ifndef BYTEORDER_H
#define BYTEORDER_H

#include <stdint.h>


static inline void writeU16BE(uint8_t* p, uint16_t v) {
    p[0] = (uint8_t)((v >> 8) & 0xFF);
    p[1] = (uint8_t)(v & 0xFF);
}


static inline void writeU32BE(uint8_t* p, uint32_t v) {
    p[0] = (uint8_t)((v >> 24) & 0xFF);
    p[1] = (uint8_t)((v >> 16) & 0xFF);
    p[2] = (uint8_t)((v >> 8) & 0xFF);
    p[3] = (uint8_t)(v & 0xFF);
}


static inline uint16_t readU16BE(const uint8_t* p) {
    return ((uint16_t)(p[0]) << 8) | p[1];
}


static inline uint32_t readU32BE(const uint8_t* p) {
    return (
        (uint32_t)(p[0]) << 24 |
        (uint32_t)(p[1]) << 16 |
        (uint32_t)(p[2]) << 8  |  
        (uint32_t)(p[3])
        );
}

#endif