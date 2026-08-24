# CCSDS Space Packet

C implementation of the CCSDS Space Packet Protocol (CCSDS 133.0-B) primary
header and packet encode/decode.

## Usage

```c
SpacePacketHeader h = { .apid = 100, .seqFlags = 3, .seqCount = 42 };
uint8_t payload[] = {0xDE, 0xAD, 0xBE, 0xEF};
uint8_t packet[64];
size_t  len;

encodePacket(&h, payload, sizeof(payload), packet, sizeof(packet), &len);
```

## Build
`gcc -Wall -Wextra -Wconversion -std=c11 ccsds.c test.c -o test`
`./test`

## Notes

Fields are serialized with explicit shifts and masks rather than by casting a
packed struct onto the buffer, since struct padding and host byte order do not
match the wire format.

`decodePacket` checks the packet's claimed data length against the actual
buffer length before returning a pointer into it.

The decoded payload points into the caller's buffer. Nothing is copied or
allocated.