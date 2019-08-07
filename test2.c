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

#include "Bloom.h"
#include "Murmur3.h"

#define FP_RATE 1.0f
#define FILTER_SIZE 512
#define TRAIN_INPUTS 2656
#define OK_INPUTS (TRAIN_INPUTS+2656)
#define INPUTS (OK_INPUTS + 25)
#define BAD_INPUTS (INPUTS-OK_INPUTS)

__attribute__((always_inline)) uint32_t hash(size_t in, uint32_t seed) {
   uint32_t out;
   MurmurHash3_x86_32(&in, sizeof(size_t), seed, &out);
   return out % (FILTER_SIZE * sizeof(uint8_t) * CHAR_BIT);
}

void check(bloom *filter) {
   printf("--------------------\n");
   printf("Ones: %zu\n", count_ones(filter->b));
   printf("Count: %zu\n", bloom_count(filter));
   printf("FP: %f\n\n", bloom_fp_rate(filter));
}

#define ROUNDS 1

int main() {
   bloom filter;
   int seeds[ROUNDS];
   uint32_t *in = calloc(INPUTS, sizeof(uint32_t));
   uint8_t *buffer = malloc(FILTER_SIZE);
   if (!buffer || !in) {
      return 1;
   }
   int r, fd;
   int fa = 0, a = 0;
   if ((fd = open("/dev/urandom", O_RDONLY)) < 0) {
      perror("open");
      return 1;
   }
   for (int i = 0; i < ROUNDS; i++) {
      if (read(fd, seeds + i, sizeof(int)) != sizeof(int)) {
         return 1;
      }
   }
   close(fd);
   for (int i = 0; i < ROUNDS; i++) {
      printf("ROUND %d\n", i);
      memset(buffer, 0, FILTER_SIZE);
      r = seeds[i];
      srand(r);
      for (size_t j = 0; j < INPUTS; j++) {
         scanf("%u\n", in+j);
      }
      if (!bloom_init(&filter, FILTER_SIZE,
              (hash_t *)hash, (uint8_t *)buffer, TRAIN_INPUTS, FP_RATE)) {
         printf("Couldn't construct filter, increase size or FP rate.\n");
         return 1;
      }
      printf("nhashes: %zu\n", filter.nhashes);
      for (size_t j = 0; j < INPUTS; j++) {
         if (j < TRAIN_INPUTS) {
            if (j == 0) {
               printf("j == 0\n");
               check(&filter);
            }
            bloom_add(&filter, in[j]);
         }
         else if (j < OK_INPUTS) {
            if (j == TRAIN_INPUTS) {
               printf("j == %d\n", TRAIN_INPUTS);
               check(&filter);
            }
            if (!bloom_contains(&filter, in[j])) {
               fa++;
            }
         }
         else {
            if (j == OK_INPUTS) {
               printf("j == %d\n", OK_INPUTS);
               check(&filter);
            }
            if (!bloom_contains(&filter, in[j])) {
               a++;
            }
         }
      }
      printf("j == %d\n", INPUTS);
      check(&filter);
   }
   free(buffer);
   printf("FA: %f\n", ((double)fa)/((double)(OK_INPUTS - TRAIN_INPUTS)));
   printf("A: %f\n", ((double)a)/((double)BAD_INPUTS));
   return 0;
}
