#include "../include/frame_codec.h"
#include "cobs.h"

#include <string.h>

size_t frame_encode(const uint8_t *data, size_t len,
                    uint8_t *out, size_t out_size)
{
    if (!data || !out || len == 0) {
        return 0;
    }
    if (out_size < FRAME_ENCODED_SIZE(len)) {
        return 0;
    }

    uint32_t crc = frame_crc32(data, len);

    /* CRC32 big-endian (MSB first) */
    const uint8_t crc_bytes[4] = {
        (uint8_t)(crc >> 24),
        (uint8_t)(crc >> 16),
        (uint8_t)(crc >>  8),
        (uint8_t)(crc)
    };

    /* COBS-encode [data || crc_bytes] without a staging buffer */
    return cobs_encode2(data, len, crc_bytes, 4, out);
}

size_t frame_decode(const uint8_t *frame, size_t frame_len,
                    uint8_t *out, size_t out_size)
{
    if (!frame || !out || frame_len == 0) {
        return 0;
    }

    /*
     * VLA: decoded size is always <= frame_len (COBS never expands data).
     * On stack-constrained targets frame_len should be bounded by the
     * protocol's maximum frame size.
     */
    uint8_t tmp[frame_len];
    size_t decoded_len = cobs_decode(frame, frame_len, tmp);

    /* minimum valid payload: 1 byte data + 4 bytes CRC32 */
    if (decoded_len < 5) {
        return 0;
    }

    size_t data_len = decoded_len - 4;
    if (data_len > out_size) {
        return 0;
    }

    /* verify CRC32 (big-endian, last 4 bytes) */
    uint32_t received_crc =
        ((uint32_t)tmp[decoded_len - 4] << 24) |
        ((uint32_t)tmp[decoded_len - 3] << 16) |
        ((uint32_t)tmp[decoded_len - 2] <<  8) |
        ((uint32_t)tmp[decoded_len - 1]);

    if (frame_crc32(tmp, data_len) != received_crc) {
        return 0;
    }

    memcpy(out, tmp, data_len);
    return data_len;
}
