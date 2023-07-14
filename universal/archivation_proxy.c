#include "archivation_proxy.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "LzmaEnc.h"
#include "LzmaDec.h"
#include "packet_net_transmission.h"

/*
to compile a test run, so I need to add those to the Makefile

gcc -I./lib_LZMA_SDK -o lzma_sample -D_7ZIP_ST test.c ./lib_LZMA_SDK/LzmaDec.c ./lib_LZMA_SDK/LzmaEnc.c ./lib_LZMA_SDK/LzFind.c ./lib_LZMA_SDK/CpuArch.c
*/

uint8_t *lzmaCompress(const uint8_t *input, uint32_t inputSize, uint32_t *outputSize); 
uint8_t *lzmaDecompress(const uint8_t *input, uint32_t inputSize, uint32_t *outputSize);


void archive_string(char *src, uint32_t length, char **dest, uint32_t *output_length){
	*dest = lzmaCompress(src, length, output_length);

    if(dest == NULL){
        printf("Error: couldn't compress a packet, dropping it\n");
    }
}

void dearchive_string(char *src, uint32_t length, char **dest, uint32_t *output_length){
    *dest = lzmaDecompress(src, length, output_length);

    if(dest == NULL){
        printf("Error: couldn't decompress a packet, dropping it\n");
    }
}


// I converted an example for LZMA_SDK in C++ to C
static void *_lzmaAlloc(ISzAllocPtr ptr, size_t size) {
	return calloc(1, size);
}
static void _lzmaFree(ISzAllocPtr ptr, void *addr) {
	if (!addr)
		return;

    free(addr);
}

static ISzAlloc _allocFuncs = {
	_lzmaAlloc, _lzmaFree
};

uint8_t *lzmaCompress(const uint8_t *input, uint32_t inputSize, uint32_t *outputSize){
	uint8_t *result;

	// set up properties
	CLzmaEncProps props;
	LzmaEncProps_Init(&props);
	if (inputSize >= (1 << 20))
		props.dictSize = 1 << 20; // 1mb dictionary
	else
		props.dictSize = inputSize; // smaller dictionary = faster!
	props.fb = 40;

	// prepare space for the encoded properties
	size_t propsSize = 5;
	uint8_t propsEncoded[5];

	// allocate some space for the compression output
	// this is way more than necessary in most cases...
	// but better safe than sorry
	size_t outputSize64 = MAX_SIZE_ARCHIVED_PACKET;
	uint8_t *output = calloc(1, outputSize64);

	int lzmaStatus = LzmaEncode(
		output, &outputSize64, input, inputSize,
		&props, propsEncoded, &propsSize, 0,
		NULL,
		&_allocFuncs, &_allocFuncs);

	*outputSize = outputSize64 + 13;
	if (lzmaStatus == SZ_OK) {
		// tricky: we have to generate the LZMA header
		// 5 bytes properties + 8 byte uncompressed size
		result = calloc(1, 13 + outputSize64) ;
		uint8_t *resultData = result;

		memcpy(resultData, propsEncoded, 5);
		for (int i = 0; i < 8; i++)
			resultData[5 + i] = (inputSize >> (i * 8)) & 0xFF;
		memcpy(resultData + 13, output, outputSize64);
	}

	return result;
}

uint8_t *lzmaDecompress(const uint8_t *input, uint32_t inputSize, uint32_t *outputSize){
	if (inputSize < 13)
		return NULL; // invalid header!

	// extract the size from the header
	uint64_t size = 0;
	for (int i = 0; i < 8; i++)
		size |= (input[5 + i] << (i * 8));

	if (size <= MAX_LENGTH_OF_NET_TRANSMITTED_PACKET - LENGTH_DEFINING_HEADER_LENGTH) {
		uint8_t *blob = calloc(1,size);

		ELzmaStatus lzmaStatus;
		size_t procOutSize = size, procInSize = inputSize - 13;
		int status = LzmaDecode(blob, &procOutSize, &input[13], &procInSize, input, 5, LZMA_FINISH_END, &lzmaStatus, &_allocFuncs);

		if (status == SZ_OK && procOutSize == size) {
			*outputSize = size;
			return blob;
		}
	}

	return NULL;
}