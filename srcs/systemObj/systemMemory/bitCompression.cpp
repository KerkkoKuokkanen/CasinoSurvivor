
#include <zlib.h>
#include <stdlib.h>
#include "endianess.h"
#include <stdio.h>

void CompressData(const char *input, size_t inputSize, char **output, size_t *outputSize)
{
	uLongf compSize = compressBound(inputSize);
	*output = (char *)malloc(compSize + sizeof(uint32_t));
	if (compress((Bytef *)(*output + sizeof(uint32_t)), &compSize, (const Bytef *)input, inputSize) != Z_OK)
	{
		free(*output);
		*output = NULL;
		*outputSize = 0;
		return ;
	}
	uint32_t inputUsed = (uint32_t)inputSize;
	inputUsed = ToLittleEndian(inputUsed);
	*(uint32_t *)*output = inputUsed;
	*outputSize = compSize + sizeof(uint32_t);
}

void DecompressData(const char *input, size_t inputSize, char **output, size_t *outputSize)
{
	if (inputSize <= sizeof(uint32_t))
	{
		*output = NULL;
		*outputSize = 0;
		return;
	}
	uint32_t originalSize = *(uint32_t *)input;
	originalSize = FromLittleEndian(originalSize);
	*output = (char *)malloc(originalSize);
	if (uncompress((Bytef *)*output, (uLongf *)&originalSize, (const Bytef *)(input + sizeof(uint32_t)), inputSize - sizeof(uint32_t)) != Z_OK) {
		free(*output);
		*output = NULL;
		*outputSize = 0;
		return;
	}
	*outputSize = originalSize;
}

