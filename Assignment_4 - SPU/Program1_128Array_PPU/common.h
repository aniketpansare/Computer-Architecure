#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <math.h>

#define TIMING
#define ARRAY_SIZE                          128

// Control block, for DMA
typedef struct _CONTROL_BLOCK {  
  unsigned int data;
  unsigned int padding[3];
} CONTROL_BLOCK;

// Some alignment macros
#define QWORD_ALIGNED __attribute__((aligned(16)))
#define CACHE_ALIGNED __attribute__((aligned(128)))

#endif
