#include <stdio.h>
#include <stdint.h>
#include "lzfx.h"

//#define OUTPUT_FILENAME "lzfx_compressed_data.h"
#define COMPRESSED_SIZE 5000
uint8_t compressed_data[COMPRESSED_SIZE];

int32_t lzfx_compress_test(uint8_t * data, size_t size)
{
    uint32_t compressed_size = COMPRESSED_SIZE;

    //printf("Compressing %u bytes of data...\r\n", text_size);
    int32_t ret = lzfx_compress(data, size, compressed_data, &compressed_size);

    if (ret < 0)
    {
        //printf("Compression failed with error code %d\r\n", ret);
        return 1;
    }

    //printf("Compressed size: %u bytes\r\n", compressed_size);

    // compute checksum on compressed data
    uint32_t checksum = 0;
    volatile uint32_t noprint_output;
    for (uint32_t i = 0; i < compressed_size; i++)
    {
        checksum += compressed_data[i];
    }
    //printf("Compressed data checksum: 0x%08X\r\n", checksum);
    noprint_output = checksum;
    (void)noprint_output;

    return 0;
}
