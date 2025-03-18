/*
 * Hexdump helper functions.
 *
 * Copyright (c) 2020 Xilinx Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef HEXDUMP_H_
#define HEXDUMP_H_
#include <stdio.h>
#include <stdlib.h>

static inline void hexdump(const char *prefix, unsigned char *buf, size_t len)
{
        unsigned char *u8 = buf;
        size_t i;

        if (prefix)
                printf("%s @ %p len=%u:\n", prefix, buf, (unsigned int) len);
        for (i = 0; i < len; i++) {
                if ((i % 16) == 0)
                        printf("%u: ", (unsigned int) i);
                printf("%2.2x ", u8[i]);
                if (((i + 1) % 16) == 0)
                        putchar('\n');
        }
        putchar('\n');
}
#endif
