
#ifndef SYSTEM_ENDIANESS_H
# define SYSTEM_ENDIANESS_H

bool SystemIsLittleEndian();

uint8_t ToLittleEndian(uint8_t value);
uint16_t ToLittleEndian(uint16_t value);
uint32_t ToLittleEndian(uint32_t value);
uint64_t ToLittleEndian(uint64_t value);

uint8_t FromLittleEndian(uint8_t value);
uint16_t FromLittleEndian(uint16_t value);
uint32_t FromLittleEndian(uint32_t value);
uint64_t FromLittleEndian(uint64_t value);

#endif
