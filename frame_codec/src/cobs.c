#include "cobs.h"

size_t cobs_encode2(const uint8_t *buf1, size_t len1,
                    const uint8_t *buf2, size_t len2,
                    uint8_t *output)
{
    size_t total = len1 + len2;
    size_t ri    = 0;   /* virtual read index over [buf1 || buf2] */
    size_t wi    = 1;   /* write index (slot 0 reserved for first code byte) */
    size_t ci    = 0;   /* position of current code byte in output */
    uint8_t code = 1;

    while (ri < total) {
        uint8_t byte = (ri < len1) ? buf1[ri] : buf2[ri - len1];
        ri++;

        if (byte == 0x00) {
            output[ci] = code;
            ci         = wi++;
            code       = 1;
        } else {
            output[wi++] = byte;
            code++;
            if (code == 0xFF) {
                output[ci] = code;
                ci         = wi++;
                code       = 1;
            }
        }
    }

    output[ci]   = code;
    output[wi++] = 0x00;  /* frame delimiter */
    return wi;
}

size_t cobs_decode(const uint8_t *input, size_t len, uint8_t *output)
{
    size_t ri = 0;
    size_t wi = 0;

    while (ri < len) {
        uint8_t code = input[ri++];

        if (code == 0x00) {
            break;  /* frame delimiter — stop */
        }

        for (uint8_t i = 1; i < code; i++) {
            if (ri >= len) {
                return 0;  /* truncated frame */
            }
            output[wi++] = input[ri++];
        }

        /*
         * Add the implicit 0x00 that was replaced by this code byte,
         * but only when:
         *   - code < 0xFF  (0xFF means "no implicit zero follows")
         *   - more input remains  (ri < len)
         *   - the next byte is not the frame delimiter
         * The last condition distinguishes intermediate groups (whose
         * implicit zero was original data) from the final group (whose
         * implicit zero is the COBS end-of-message marker, not data).
         */
        if (code < 0xFF && ri < len && input[ri] != 0x00) {
            output[wi++] = 0x00;
        }
    }

    return wi;
}
