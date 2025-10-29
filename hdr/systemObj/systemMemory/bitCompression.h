
#ifndef BIT_COMPRESSION_H
# define BIT_COMPRESSION_H

void CompressData(const char *input, size_t inputSize, char **output, size_t *outputSize);
void DecompressData(const char *input, size_t inputSize, char **output, size_t *outputSize);

#endif
