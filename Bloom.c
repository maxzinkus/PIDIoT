/* Bloom.c
 * Max Zinkus
 */

#include "Bloom.h"

#include <math.h>

bool bloom_init(bloom * const b, const size_t size,
                hash_t * const hashes, uint8_t * const buf,
                const size_t expected, const double fp) {
   if (!(b && size && hashes && buf)) {
      return false;
   }
   b->b.len = size;
   b->b.data = buf;
   b->hashes = hashes;
   b->nelems = expected; /* TEMPORARY, must set to 0 */
   b->nhashes = 1;
   while (bloom_fp_rate(b) > fp && b->nhashes < MAX_HASHES) {
      b->nhashes += 1;
   }
   b->nelems = 0;
   if (b->nhashes >= MAX_HASHES) {
      return false;
   }
   return true;
}

bool bloom_contains(const bloom * const b, const size_t elem) {
   size_t i = 0;
   for (i = 0; i < b->nhashes; i++) {
      if (!is_set(b->b, (b->hashes)(elem, i))) {
         return false;
      }
   }
   return true;
}

void bloom_add(bloom * const b, const size_t elem) {
   size_t i = 0;
   b->nelems = b->nelems + 1;
   /*#pragma clang loop unroll(enable)*/
   for (i = 0; i < b->nhashes; i++) {
      set_bit(b->b, (b->hashes)(elem, i));
   }
}

size_t bloom_count(const bloom * const b) {
   return b->nelems;
}

double bloom_fp_rate(const bloom * const b) {
   /* Starobinski Trachtenberg & Agarwal, 2003 */
   const double m = b->b.len*CHAR_BIT;
   const double n = b->nelems;
   const double k = b->nhashes;
   return pow(1.0 - exp(-1.0*k*n/m), k);
}

__attribute__((always_inline))
void bloom_sethashes(bloom * const b, hash_t * const hashes) {
   b->hashes = hashes;
}
