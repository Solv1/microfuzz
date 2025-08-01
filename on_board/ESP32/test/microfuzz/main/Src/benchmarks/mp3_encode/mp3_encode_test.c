#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shine_mp3.h"

static uint32_t g_wav_file_size;
static const uint32_t g_header_size_bytes = 44;
static const uint32_t g_output_bitrate = 16;

#define SHINE_ENCODER_SIZE 23960
uint8_t g_shine_encoder_block[SHINE_ENCODER_SIZE];
// shine_t shine_encoder;


void verify_output(void *output_buffer, int32_t num_bytes, uint32_t *checksum)
{
    for (size_t i = 0; i < num_bytes; i++)
    {
        *checksum += ((uint8_t *)output_buffer)[i];
    }
}

int32_t mp3_encode(uint8_t * input, size_t size)
{
    	shine_config_t config;
    	int32_t bytes_written;
    	uint8_t *output_data;
    	uint32_t output_size = 0;
    	uint32_t checksum = 0;
    	volatile uint32_t noprint_output;
	g_wav_file_size = size;	
    	shine_set_config_mpeg_defaults(&config.mpeg);

    	config.mpeg.bitr = g_output_bitrate;
    	config.mpeg.mode = MONO;

    	//printf("Input sample rate: %d Hz\r\n", SAMPLE_RATE);
    	//printf("Input channels: %d\r\n", NUM_CHANNELS);
    	//printf("Input wav file size (total): %u bytes\r\n", g_wav_file_size);
    	//printf("Input wav file size (data): %u bytes\r\n", (g_wav_file_size - g_header_size_bytes));

    /* Initiate encoder */
    	shine_t shine_encoder = (shine_t)g_shine_encoder_block;
    // printf("Shine encoder size: %u bytes\r\n", sizeof_shine_t);
    	shine_initialise(&config, shine_encoder);

    	int16_t *input_buffer = (int16_t *)(input + g_header_size_bytes); // skip the header

    	uint32_t totalSampleCount = (g_wav_file_size - g_header_size_bytes) / sizeof(int16_t);
    	uint32_t samples_per_pass = shine_samples_per_pass(shine_encoder) * NUM_CHANNELS;
    	uint32_t num_passes = totalSampleCount / samples_per_pass;


    	for (uint32_t i = 0; i < num_passes; i++)
    	{
        	output_data = shine_encode_buffer(shine_encoder, &input_buffer, &bytes_written);
        	verify_output(output_data, bytes_written, &checksum);
        	input_buffer += samples_per_pass;
        	output_size += bytes_written;
    	}

    	size_t last = totalSampleCount % samples_per_pass;
    	if (last != 0)
    	{
        	int16_t *cache = (int16_t *)alloca(samples_per_pass * sizeof(int16_t));
        	memset(cache, 0, samples_per_pass * sizeof(int16_t));
        	memcpy(cache, input_buffer, last * sizeof(int16_t));
        	output_data = shine_encode_buffer(shine_encoder, &cache, &bytes_written);
        	verify_output(output_data, bytes_written, &checksum);
        	output_size += bytes_written;
    	}

    	/* Flush and write remaining data. */
    	output_data = shine_flush(shine_encoder, &bytes_written);
    	verify_output(output_data, bytes_written, &checksum);
    	output_size += bytes_written;

    	/* Close encoder. */
    	shine_close(shine_encoder);

    	//printf("Output bitrate: %u kbps\r\n", g_output_bitrate);
    	//printf("Output file size: %u bytes\r\n", output_size);
    	//printf("Checksum = %u\r\n", checksum);
    	noprint_output = output_size;
    	(void)noprint_output;

    return 0;
}
