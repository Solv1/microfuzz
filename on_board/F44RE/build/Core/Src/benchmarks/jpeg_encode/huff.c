/**
 * Copyright (c) 2012-2016 Moodstocks SAS
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "huff.h"
#include "conf.h"

#ifdef WORD_BIT
#define JPEC_INT_WIDTH WORD_BIT
#else
#define JPEC_INT_WIDTH (int32_t)(sizeof(int32_t) * CHAR_BIT)
#endif

// Builtin is broken for msp430-gcc, fallback to regular implementation
#if defined(__GNUC__) && !defined(__MSP430FR5994__)
#define JPEC_HUFF_NBITS(JPEC_nbits, JPEC_val) \
    JPEC_nbits = (!JPEC_val) ? 0 : JPEC_INT_WIDTH - __builtin_clz(JPEC_val)
#else
#define JPEC_HUFF_NBITS(JPEC_nbits, JPEC_val) \
    JPEC_nbits = 0;                           \
    while (val)                               \
    {                                         \
        JPEC_nbits++;                         \
        val >>= 1;                            \
    }
#endif

/* Private function prototypes */
static int8_t jpec_huff_encode_block_impl(jpec_block_t *block, jpec_huff_state_t *s);
static int8_t jpec_huff_write_bits(jpec_huff_state_t *s, uint32_t bits, int32_t n);

jpec_huff_t g_jpeg_huff;

int8_t jpec_huff_skel_init(jpec_huff_skel_t *skel)
{
    //assert(skel);
    if(!(skel)){
	exit(-1);
    }
    memset(skel, 0, sizeof(*skel));
    skel->opq = jpec_huff_new();
    skel->del = (void (*)(void *))jpec_huff_del;
    skel->encode_block = (void (*)(void *, jpec_block_t *, jpec_buffer_t *))jpec_huff_encode_block;
    return 0;
}

jpec_huff_t *jpec_huff_new(void)
{
    jpec_huff_t *h = &g_jpeg_huff;
    h->state.buffer = 0;
    h->state.nbits = 0;
    h->state.dc = 0;
    h->state.buf = NULL;
    return h;
}

int8_t jpec_huff_del(jpec_huff_t *h)
{
    //assert(h);
    if(!(h)){
	exit(-1);
    }
    /* Flush any remaining bits and fill in the incomple byte (if any) with 1-s */
    jpec_huff_write_bits(&h->state, 0x7F, 7);
    return 0;
}

int8_t jpec_huff_encode_block(jpec_huff_t *h, jpec_block_t *block, jpec_buffer_t *buf)
{
    int8_t ret_val = 0;
    if(!(h && block && buf)){
	exit(-1);
    }
    jpec_huff_state_t state;
    state.buffer = h->state.buffer;
    state.nbits = h->state.nbits;
    state.dc = h->state.dc;
    state.buf = buf;
    ret_val = jpec_huff_encode_block_impl(block, &state);
    h->state.buffer = state.buffer;
    h->state.nbits = state.nbits;
    h->state.dc = state.dc;
    h->state.buf = state.buf;
    return ret_val;
}

static int8_t jpec_huff_encode_block_impl(jpec_block_t *block, jpec_huff_state_t *s)
{
    int8_t ret_val = 0;
    if(!(block && s)){
	exit(-1);
    }
    int32_t val, bits, nbits;
    /* DC coefficient encoding */
    if (block->len > 0)
    {
        val = block->zz[0] - s->dc;
        s->dc = block->zz[0];
    }
    else
    {
        val = -s->dc;
        s->dc = 0;
    }
    bits = val;
    if (val < 0)
    {
        val = -val;
        bits = ~val;
    }
    JPEC_HUFF_NBITS(nbits, val);
    ret_val = jpec_huff_write_bits(s, jpec_dc_code[nbits], jpec_dc_len[nbits]);
    if (nbits){
        ret_val = jpec_huff_write_bits(s, (uint32_t)bits, nbits);
    }
    /* AC coefficients encoding (w/ RLE of zeros) */
    int32_t nz = 0;
    for (int32_t i = 1; i < block->len; i++)
    {
        if ((val = block->zz[i]) == 0)
            nz++;
        else
        {
            while (nz >= 16)
            {
                ret_val = jpec_huff_write_bits(s, jpec_ac_code[0xF0], jpec_ac_len[0xF0]); /* ZRL code */
                nz -= 16;
            }
            bits = val;
            if (val < 0)
            {
                val = -val;
                bits = ~val;
            }
            JPEC_HUFF_NBITS(nbits, val);
            int32_t j = (nz << 4) + nbits;
            ret_val = jpec_huff_write_bits(s, jpec_ac_code[j], jpec_ac_len[j]);
            if (nbits)
                ret_val = jpec_huff_write_bits(s, (uint32_t)bits, nbits);
            nz = 0;
        }
    }
    if (block->len < 64)
    {
        ret_val = jpec_huff_write_bits(s, jpec_ac_code[0x00], jpec_ac_len[0x00]); /* EOB marker */
    }
    return 0;
}

/* Write n bits into the JPEG buffer, with 0 < n <= 16.
 *
 * == Details
 * - 16 bits are large enough to hold any zig-zag coeff or the longest AC code
 * - bits are chunked into bytes before being written into the JPEG buffer
 * - any remaining bits are kept in memory by the Huffman state
 * - at most 7 bits can be kept in memory
 * - a 32-bit integer buffer is used internally
 * - only the right 24 bits part of this buffer are used
 * - the input bits and remaining bits (if any) are left-justified on this part
 * - a mask is used to mask off any extra bits: useful when the input value has been
 *   first transformed by bitwise complement(|initial value|)
 * - if an 0xFF byte is detected a 0x00 stuff byte is automatically written right after
 */
static int8_t jpec_huff_write_bits(jpec_huff_state_t *s, uint32_t bits, int32_t n)
{
    if(!(s && n > 0 && n <= 16)){
    	exit(-1);
    }
    int32_t mask = (((int32_t)1) << n) - 1;
    int32_t buffer = (int32_t)bits;
    int32_t nbits = s->nbits + n;
    buffer &= mask;
    buffer <<= 24 - nbits;
    buffer |= s->buffer;
    while (nbits >= 8)
    {
        int32_t chunk = (int32_t)((buffer >> 16) & 0xFF);
        jpec_buffer_write_byte(s->buf, chunk);
        if (chunk == 0xFF)
            jpec_buffer_write_byte(s->buf, 0x00);
        buffer <<= 8;
        nbits -= 8;
    }
    s->buffer = buffer;
    s->nbits = nbits;
    return 0;
}
