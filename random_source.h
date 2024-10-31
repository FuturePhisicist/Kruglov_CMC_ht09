#pragma once

#include "mtwister.h"

typedef struct RandomSourceOperations
{
    struct RandomSource *(*destroy)(struct RandomSource *src);
    double (*next)(struct RandomSource *src);
} RandomSourceOperations;

typedef struct RandomSource
{
    RandomSourceOperations *ops;
    void *gen; // source-spefic generator structure
} RandomSource;

enum
{
    DEC_RADIX = 10,
};
