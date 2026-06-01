#ifndef H_CRC_H
#define H_CRC_H

#include <stdint.h>

void init_crc();

uint32_t get_crc(uint32_t block, size_t size);

#endif //H_CRC_H
