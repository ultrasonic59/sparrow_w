#ifndef THK_BANK_H
#define THK_BANK_H

/* Assemble the DAC buffer from THK frames. sparrow5 RAM is 8192 x 16.
   The playback counter is 13 bits, so at most 8191 samples are accepted. */

#include "thk_proto.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
    THK_DAC_SAMPLES = 8191,
    THK_ACK_OK = 0,
    THK_ACK_FORMAT = 1,
    THK_ACK_OVERFLOW = 2,
    THK_ACK_CHUNK = 3,
    THK_ERR_RAM = 4,
    THK_ERR_DAC = 5,
    THK_ERR_COM = 6,
    THK_ERR_I2C = 7
};

typedef struct ThkBank {
    int16_t samples[THK_DAC_SAMPLES];
    uint32_t total;
    uint32_t rate;
    uint16_t chunk_count;
    uint16_t got_mask;
    uint16_t last_error;
    int complete;
} ThkBank;

typedef struct ThkStream {
    uint8_t raw[sizeof(ThkHeader) + THK_MAX_PAYLOAD];
    uint16_t filled;
    uint16_t frame_len;
} ThkStream;

static inline void thk_bank_reset(ThkBank* bank)
{
    memset(bank, 0, sizeof(*bank));
}

static inline uint32_t thk_samples_crc(const int16_t* samples, uint32_t count)
{
    return thk_crc32((const uint8_t*)samples, (size_t)count * sizeof(int16_t));
}

static inline void thk_bank_status(const ThkBank* bank, ThkStatusBody* status)
{
    memset(status, 0, sizeof(*status));
    status->link = 1;
    status->last_error = bank->last_error;
    status->flags = bank->complete ? 1u : 0u;
    if (!bank->complete)
        return;
    status->sample_count = bank->total;
    status->sample_crc = thk_samples_crc(bank->samples, bank->total);
}

static inline uint16_t thk_bank_wave(ThkBank* bank, const uint8_t* payload, uint16_t length)
{
    ThkWaveInfo info;
    uint32_t sample_bytes;
    uint32_t sample_count;
    uint32_t offset;
    uint32_t expect;
    int fresh;

    if (length < sizeof(ThkWaveInfo))
        return THK_ACK_FORMAT;
    memcpy(&info, payload, sizeof(info));
    sample_bytes = (uint32_t)length - (uint32_t)sizeof(ThkWaveInfo);
    if (info.sample_format != THK_SAMPLE_INT16 || (sample_bytes & 1u))
        return THK_ACK_FORMAT;
    sample_count = sample_bytes / 2u;
    if (info.chunk_count == 0 || info.chunk_index >= info.chunk_count)
        return THK_ACK_CHUNK;
    if (info.total_samples == 0 || info.total_samples > THK_DAC_SAMPLES || info.chunk_count > 16) {
        bank->last_error = THK_ACK_OVERFLOW;
        return THK_ACK_OVERFLOW;
    }

    offset = (uint32_t)info.chunk_index * (uint32_t)THK_WAVE_SAMPLES_PER_CHUNK;
    if (offset >= info.total_samples)
        return THK_ACK_FORMAT;
    if ((uint16_t)(info.chunk_index + 1u) == info.chunk_count)
        expect = info.total_samples - offset;
    else
        expect = THK_WAVE_SAMPLES_PER_CHUNK;
    if (sample_count != expect)
        return THK_ACK_FORMAT;

    fresh = bank->chunk_count == 0 || info.total_samples != bank->total || info.chunk_count != bank->chunk_count
        || (info.chunk_index == 0 && bank->complete);
    if (fresh) {
        memset(bank, 0, sizeof(*bank));
        bank->rate = info.sample_rate_hz;
        bank->total = info.total_samples;
        bank->chunk_count = info.chunk_count;
    }

    memcpy(bank->samples + offset, payload + sizeof(ThkWaveInfo), sample_bytes);
    bank->got_mask = (uint16_t)(bank->got_mask | (uint16_t)(1u << info.chunk_index));
    if (bank->got_mask == (uint16_t)((1u << info.chunk_count) - 1u)) {
        bank->complete = 1;
        bank->last_error = 0;
    }
    return THK_ACK_OK;
}

static inline void thk_stream_reset(ThkStream* stream)
{
    memset(stream, 0, sizeof(*stream));
}

static inline void thk_stream_drop(ThkStream* stream)
{
    if (stream->filled == 0)
        return;
    memmove(stream->raw, stream->raw + 1, stream->filled - 1u);
    stream->filled--;
}

static inline void thk_stream_consume(ThkStream* stream)
{
    uint16_t count = stream->frame_len;
    if (count > stream->filled)
        count = stream->filled;
    memmove(stream->raw, stream->raw + count, stream->filled - count);
    stream->filled = (uint16_t)(stream->filled - count);
    stream->frame_len = 0;
}

/* 1 when the buffer starts with a complete frame and a valid CRC. */
static inline int thk_stream_byte(ThkStream* stream, uint8_t byte)
{
    if (stream->filled >= sizeof(stream->raw))
        thk_stream_drop(stream);
    stream->raw[stream->filled++] = byte;
    for (;;) {
        ThkHeader header;
        size_t total;
        if (stream->filled < sizeof(ThkHeader))
            return 0;
        memcpy(&header, stream->raw, sizeof(header));
        if (header.signature != THK_SIGNATURE || header.version != THK_VERSION || header.length > THK_MAX_PAYLOAD) {
            thk_stream_drop(stream);
            continue;
        }
        total = sizeof(ThkHeader) + header.length;
        if (stream->filled < total)
            return 0;
        if (!thk_check(stream->raw, total)) {
            thk_stream_drop(stream);
            continue;
        }
        stream->frame_len = (uint16_t)total;
        return 1;
    }
}

#ifdef __cplusplus
}
#endif

#endif
