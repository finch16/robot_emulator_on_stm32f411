# Емулятор робота на STM32F411

Прошивка для STM32F411, яка емулює робота. Приймає 6 значень (float) через UART з COBS-кодуванням, виводить їх на 7-сегментний індикатор та передає назад значення з аналогового входу, до якого під'єднаний потенціометр.

## Функції

- **UART** — прийом/передача даних з COBS-фреймами та CRC
- **7-сегментний індикатор** — відображення поточного каналу
- **Аналоговий вхід** — читання потенціометра з low-pass фільтром
- **Кнопка** — перемикання каналів (0–5)

## Сторонні бібліотеки

Файли:
- `frame_codec/include/frame_codec.h`
- `frame_codec/src/cobs.c`
- `frame_codec/src/cobs.h`
- `frame_codec/src/frame_codec.c`

були скопійовані з бібліотеки [frame_codec](https://github.com/Zirow0/frame_codec/tree/c16047528b56bf6026be8f350abc0278fc6462b3).


Та потрібно бібліотеку [libopencm3](https://github.com/libopencm3/libopencm3).

## Збірка

```bash
export LIBOPENCM3_DIR=/шлях/до/libopencm3
cmake -B build
cmake --build build
```
