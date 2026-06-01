#ifndef FRAME_CODEC_H
#define FRAME_CODEC_H

#include <stdint.h>
#include <stddef.h>

/* Розмір вихідного буфера для frame_encode */
#define FRAME_ENCODED_SIZE(data_len) \
    (((data_len) + 4) + (((data_len) + 4) / 254) + 2)

/* ----------------------------------------------------------------
 * frame_encode
 *   data     — вхідні дані
 *   len      — кількість байт
 *   out      — вихідний буфер (мінімум FRAME_ENCODED_SIZE(len))
 *   out_size — розмір вихідного буфера
 *
 *   Повертає: кількість байт у out (включно з 0x00 delimiter)
 *             0 — якщо out_size замалий або data == NULL або out == NULL
 * ---------------------------------------------------------------- */
size_t frame_encode(const uint8_t *data, size_t len,
                    uint8_t *out, size_t out_size);

/* ----------------------------------------------------------------
 * frame_decode
 *   frame      — COBS-фрейм (без або з trailing 0x00)
 *   frame_len  — довжина frame
 *   out        — вихідний буфер для даних
 *   out_size   — розмір вихідного буфера
 *
 *   Повертає: кількість байт у out
 *             0 — CRC помилка, пошкоджений фрейм або NULL
 * ---------------------------------------------------------------- */
size_t frame_decode(const uint8_t *frame, size_t frame_len,
                    uint8_t *out, size_t out_size);

/* ----------------------------------------------------------------
 * frame_crc32  — weak symbol
 *
 *   За замовчуванням: програмна реалізація CRC-32/MPEG-2
 *   (поліном 0x04C11DB7, init 0xFFFFFFFF, без рефлексії).
 *
 *   Щоб використати апаратний CRC — визначити цю функцію
 *   у власному файлі проекту без атрибута weak.
 *   Примітка: апаратний CRC32 (STM32) не є потоко-безпечним
 *   через єдиний глобальний апаратний блок — документувати
 *   у проекті користувача.
 *
 *   Сигнатура повинна точно відповідати:
 *     uint32_t frame_crc32(const uint8_t *data, size_t len);
 * ---------------------------------------------------------------- */
uint32_t frame_crc32(const uint8_t *data, size_t len);

#endif /* FRAME_CODEC_H */
