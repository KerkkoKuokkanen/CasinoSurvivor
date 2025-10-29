
static bool endianSet = false;
static bool littleEndian = true;

#include <cstdint>

bool SystemIsLittleEndian()
{
	if (endianSet)
		return (littleEndian);
	endianSet = true;
	union
	{
		uint32_t i;
		char c[4];
	} test = {0x01020304};
	littleEndian = (test.c[0] == 0x04);
	return (littleEndian);
}

uint8_t ToLittleEndian(uint8_t value)
{
	return (value);
}

uint16_t ToLittleEndian(uint16_t value)
{
	if (SystemIsLittleEndian())
		return (value);
	return (value >> 8) | (value << 8);
}

uint32_t ToLittleEndian(uint32_t value)
{
	if (SystemIsLittleEndian())
		return (value);
	return ((value >> 24) & 0x000000FF) |
			((value >> 8) & 0x0000FF00) |
			((value << 8) & 0x00FF0000) |
			((value << 24) & 0xFF000000);
}

uint64_t ToLittleEndian(uint64_t value)
{
	if (SystemIsLittleEndian())
		return (value);
	return ((value >> 56) & 0x00000000000000FFULL) |
			((value >> 40) & 0x000000000000FF00ULL) |
			((value >> 24) & 0x0000000000FF0000ULL) |
			((value >> 8)  & 0x00000000FF000000ULL) |
			((value << 8)  & 0x000000FF00000000ULL) |
			((value << 24) & 0x0000FF0000000000ULL) |
			((value << 40) & 0x00FF000000000000ULL) |
			((value << 56) & 0xFF00000000000000ULL);
}

uint8_t FromLittleEndian(uint8_t value)
{
	return ToLittleEndian(value);
}

uint16_t FromLittleEndian(uint16_t value)
{
	return ToLittleEndian(value);
}

uint32_t FromLittleEndian(uint32_t value)
{
	return ToLittleEndian(value);
}

uint64_t FromLittleEndian(uint64_t value)
{
	return ToLittleEndian(value);
}

