#ifndef COBS_H
#define COBS_H

#include <stdint.h>
#include <stddef.h>

/*
 * Encode two buffers as a single concatenated COBS frame.
 * Appends trailing 0x00 delimiter; returns total bytes written.
 * output must be at least (len1+len2) + (len1+len2)/254 + 2 bytes.
 */
size_t cobs_encode2(const uint8_t *buf1, size_t len1,
                    const uint8_t *buf2, size_t len2,
                    uint8_t *output);

/*
 * Decode a COBS frame — accepts input with OR without trailing 0x00.
 * output must be at least len bytes.
 * Returns decoded byte count, or 0 on truncated/malformed input.
 */
size_t cobs_decode(const uint8_t *input, size_t len, uint8_t *output);

#endif /* COBS_H */
