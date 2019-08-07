/* test.c
 * Max Zinkus
 */
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#include "Bloom.h"
#include "Murmur3.h"
#include "tcphash.h"

#define K 100
#define INPUTS 1000
#define TRAIN_INPUTS INPUTS-K

void check(bloom *filter) {
   printf("--------------------\n");
   printf("Count: %zu\n", bloom_count(filter));
   printf("FP: %f\n\n", bloom_fp_rate(filter));
}

#define ROUNDS INPUTS/K

int main() {
   bloom filter;
   uint32_t *in = calloc(INPUTS, sizeof(uint32_t));
   uint8_t *buffer = malloc(FILTER_SIZE);
   if (!buffer || !in) {
      return 1;
   }
   int fa[ROUNDS];
   double fa_avg = 0.0;
   memset(fa, 0, ROUNDS*sizeof(int));
   for (size_t j = 0; j < INPUTS; j++) {
      scanf("%u\n", in+j);
   }
   for (int i = 0; i < ROUNDS; i++) {
      printf("ROUND %d\n--------------------\n", i);
      memset(buffer, 0, FILTER_SIZE);
      if (!bloom_init(&filter, FILTER_SIZE,
              (hash_t *)hash, (uint8_t *)buffer, TRAIN_INPUTS, 1.0)) {
         printf("Couldn't construct filter, increase size or FP rate.\n");
         return 1;
      }
      printf("nhashes: %zu\n", filter.nhashes);
      for (size_t j = 0; j < INPUTS; j++) {
         if (j < i*K || j >= (i+1)*K) {
            bloom_add(&filter, in[j]);
         }
      }
      for (size_t j = i*K; j < (i+1)*K; j++) {
         if (!bloom_contains(&filter, in[j])) {
            (fa[i])++;
         }
      }
      check(&filter);
   }
   free(buffer);
   for (int i = 0; i < ROUNDS; i++) {
      printf("FA(%d): %d (%f)\n", i, fa[i], ((double)fa[i])/((double)K));
      fa_avg += fa[i];
   }
   fa_avg /= ((double)ROUNDS);
   printf("FA: %f\n", fa_avg);
   return 0;
}
