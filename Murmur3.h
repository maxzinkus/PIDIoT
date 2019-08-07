#pragma once
/* Murmur3.h
 * Max Zinkus
 * Based on the Public Domain code by Austin Appleby
 */

#include <stddef.h>
#include <stdint.h>

void MurmurHash3_x86_32(const void *key, int len, uint32_t seed, void *out);

void MurmurHash3_x86_128(const void *key, const int len, uint32_t seed,
                         void *out);
void MurmurHash3_x64_128(const void *key, const int len, const uint32_t seed,
                         void *out);
