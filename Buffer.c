/* Buffer.c
 * Max Zinkus
 */

#include "Buffer.h"

#include <stdio.h>

bool is_set(const buffer b, const size_t i) {
   const size_t offset = i % CHAR_BIT;
   const size_t block = (i - offset) / CHAR_BIT;
   if (block >= b.len) {
      printf("Bit %zu out of range.\n", i);
      return false;
   }
   return ((b.data)[block] & (1 << offset)) != 0;
}

void set_bit(const buffer b, const size_t i) {
   const size_t offset = i % CHAR_BIT;
   const size_t block = (i - offset) / CHAR_BIT;
   if (block >= b.len) {
      printf("Can't set bit %zu.\n", i);
      return;
   }
   (b.data)[block] = (b.data)[block] | (1 << offset);
}

size_t count_ones(const buffer b) {
   size_t i = 0, c = 0;
   for (i = 0; i < b.len*CHAR_BIT; i++) {
      c += is_set(b, i) ? 1 : 0;
   }
   return c;
}
