#pragma once
/* Buffer.h
 * Max Zinkus
 */

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
   size_t len;
   uint8_t *data;
} buffer;

bool is_set(const buffer b, const size_t i);

void set_bit(const buffer b, const size_t i);

size_t count_ones(const buffer b);
