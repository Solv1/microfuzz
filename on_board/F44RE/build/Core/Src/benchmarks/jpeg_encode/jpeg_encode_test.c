#include <stdio.h>

#include "jpec.h"

#define OUTPUT_IMAGE_QUALITY 60


int32_t jpeg_encode(uint8_t * data, uint16_t w, uint16_t h)
{
    jpec_enc_t *e = jpec_enc_new2(data, w, h, OUTPUT_IMAGE_QUALITY);

    int32_t len;
    const uint8_t *jpeg = jpec_enc_run(e, &len);
    if (!(jpeg)){
	return -1;
    }

    return 0;
}
