#pragma once
/* Bloom.h
 * Max Zinkus
 */

#include "Buffer.h"

#include <stdint.h>
#include <stdbool.h>

#define MAX_HASHES 2048

typedef size_t (hash_t)(size_t,uint32_t);

typedef struct {
   size_t nelems;
   size_t nhashes;
   hash_t *hashes;
   buffer b;
} bloom;

/* Initialize a bloom filter of a given size in bytes
 * Space must be supplied and maintained by the caller
 * to prevent malloc's and allow for extensible
 * backing stores
 */
bool bloom_init(bloom * const b, const size_t size,
                hash_t * const hashes, uint8_t * const buf,
                const size_t expected, const double fp);

/* Perform membership lookup */
bool bloom_contains(const bloom * const b, const size_t elem);

/* Add an element to the filter */
void bloom_add(bloom * const b, const size_t elem);

/* Estimate the number of elements in the filter */
size_t bloom_count(const bloom * const b);

/* Estimate the false positive rate for the filter */
double bloom_fp_rate(const bloom * const b);

/* Set or replace the hash functions */
void bloom_sethashes(bloom * const b, hash_t * const hashes);
