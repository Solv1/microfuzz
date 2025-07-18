#include <stdint.h>
#include <stdio.h>

#include "lzfx.h"

#define DECOMP_DATA_LEN 0x1800
uint8_t decompressed_data[DECOMP_DATA_LEN];

int32_t lzfx_decomp_test(uint8_t * data, size_t size)
{
    uint32_t compressed_size = size;
    uint32_t decompressed_size = DECOMP_DATA_LEN;

    //printf("Decompressing %u bytes of data...\r\n", compressed_size);
    int32_t ret = lzfx_decompress_tiny(data, compressed_size, decompressed_data, &decompressed_size);

    if (ret < 0)
    {
        //printf("Compression failed with error code %d\r\n", ret);
        return 1;
    }

    //printf("Decompressed size: %u bytes\r\n", decompressed_size);

    // compute checksum on decompressed data
    uint32_t checksum = 0;
    volatile uint32_t noprint_output;
    for (uint32_t i = 0; i < compressed_size; i++)
    {
        checksum += decompressed_data[i];
    }
    //printf("Decompressed data checksum: 0x%08X\r\n", checksum);
    noprint_output = checksum;
    (void)noprint_output;

    return 0;
}
