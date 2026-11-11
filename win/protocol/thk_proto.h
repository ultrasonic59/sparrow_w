#ifndef THK_PROTO_H
#define THK_PROTO_H

/* Emulator UDP frame: little-endian, port 45000.
   CRC32 covers the whole frame with the crc32 field set to zero. */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define THK_SIGNATURE 0x314B4854u /* 'THK1' */
#define THK_VERSION 1u
#define THK_UDP_PORT 45000u
#define THK_MAX_PAYLOAD 1400u
#define THK_SAMPLE_INT16 1u

enum {
    THK_HELLO = 1,
    THK_WAVE = 2,
    THK_CTRL = 3,
    THK_ACK = 4,
    THK_STATUS = 5,
    THK_TEXT = 6
};

enum {
    THK_CTRL_STOP = 1,
    THK_CTRL_FREERUN = 2,
    THK_CTRL_ARM = 3,
    THK_CTRL_ONCE = 4,
    THK_CTRL_SINE = 5,
    THK_CTRL_ANALOG = 6
};

#pragma pack(push, 1)

typedef struct ThkHeader {
    uint32_t signature;
    uint16_t version;
    uint16_t type;
    uint32_t sequence;
    uint16_t length;
    uint16_t reserved;
    uint32_t crc32;
} ThkHeader;

typedef struct ThkWaveInfo {
    uint32_t sample_rate_hz;
    uint16_t sample_format;
    uint16_t reserved;
    uint32_t total_samples;
    uint16_t chunk_index;
    uint16_t chunk_count;
} ThkWaveInfo;

typedef struct ThkCtrlBody {
    uint16_t command;
    uint16_t reserved; /* SINE: amplitude, percent. ANALOG: offset code 0..4095 */
    uint32_t period_us; /* SINE: frequency, Hz. ANALOG: amplitude code 0..4095. Otherwise pause, us */
} ThkCtrlBody;

typedef struct ThkAckBody {
    uint32_t ack_sequence;
    uint16_t chunk_index;
    uint16_t status;
} ThkAckBody;

typedef struct ThkStatusBody {
    uint16_t link;
    uint16_t active_bank;
    uint32_t trigger_count;
    uint16_t last_error;
    uint16_t flags;
    uint32_t sample_count;
    uint32_t sample_crc;
} ThkStatusBody;

/* TEXT: baud, then the bytes the board writes to USB-COM. */
typedef struct ThkTextInfo {
    uint32_t baud;
} ThkTextInfo;

#pragma pack(pop)

enum { THK_WAVE_SAMPLES_PER_CHUNK = (THK_MAX_PAYLOAD - 16) / 2 };

static inline uint32_t thk_crc32(const uint8_t* data, size_t length)
{
    uint32_t crc = 0xFFFFFFFFu;
    size_t i;
    for (i = 0; i < length; ++i) {
        int bit;
        crc ^= data[i];
        for (bit = 0; bit < 8; ++bit) {
            const uint32_t mask = (crc & 1u) ? 0xFFFFFFFFu : 0u;
            crc = (crc >> 1) ^ (0xEDB88320u & mask);
        }
    }
    return ~crc;
}

static inline void thk_seal(uint8_t* packet, size_t total)
{
    ThkHeader* header = (ThkHeader*)packet;
    header->crc32 = 0;
    header->crc32 = thk_crc32(packet, total);
}

static inline int thk_check(const uint8_t* packet, size_t total)
{
    uint8_t copy[sizeof(ThkHeader) + THK_MAX_PAYLOAD];
    ThkHeader* header;
    uint32_t found;
    if (total < sizeof(ThkHeader) || total > sizeof(copy))
        return 0;
    memcpy(copy, packet, total);
    header = (ThkHeader*)copy;
    found = header->crc32;
    header->crc32 = 0;
    return thk_crc32(copy, total) == found;
}

#ifdef __cplusplus
}
#endif

#endif
