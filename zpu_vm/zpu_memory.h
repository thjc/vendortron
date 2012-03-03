#ifndef ZPU_MEMORY_H
#define ZPU_MEMORY_H

#include <stdint.h>

// Memory size must be a power of 2.
#define ROM_BASE 0x00000000
#define ROM_SIZE 0x000F0000
#define ROM_MASK 0x000FFFFF
#define ROM_TOP  ( ROM_BASE + ROM_SIZE )

#define RAM_BASE 0x00100000
#define RAM_SIZE 0x00070000
#define RAM_MASK 0x0007FFFF
#define RAM_TOP  ( RAM_BASE + RAM_SIZE )

#define STK_BASE 0x0018E000
#define STK_SIZE 0x00002000
#define STK_MASK 0x00001FFF
#define STK_TOP  ( STK_BASE + STK_SIZE )


void memoryInitialize();

uint32_t memoryReadLong(uint32_t address);

void memoryWriteLong(uint32_t address, uint32_t value);

uint8_t memoryReadByte(uint32_t address);

void memoryWriteByte(uint32_t address, uint8_t value);

uint16_t memoryReadWord(uint32_t address);

void memoryWriteWord(uint32_t address, uint16_t value);

void memoryWriteROM(uint32_t address, uint8_t value);

#endif