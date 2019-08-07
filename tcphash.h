/* tcphash.h
 * Max Zinkus
 */

#include <stdint.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include "Murmur3.h"

#define FILTER_SIZE 512

/* Jacobson-Karels hysteresis averaging factor
 * higher is less prone to sharp variation
 */
#define JK_FACTOR 0.99f

static double avg = 577.8, dev = 100.0;

__attribute__((always_inline)) uint32_t hash(size_t in, uint32_t seed) {
   uint32_t out;
   double diff = in - avg;
   avg = avg + JK_FACTOR*diff;
   dev = dev + (JK_FACTOR*(fabs(diff)-dev));
   in = (size_t)fabs((avg + (JK_FACTOR*dev))); 
   MurmurHash3_x86_32(&in, sizeof(size_t), seed, &out);
   return out % (FILTER_SIZE * sizeof(uint8_t) * CHAR_BIT);
}

