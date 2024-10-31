#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "mt.h"
#include "random_source.h"
#include "mtwister.h"

static double
mt_next(RandomSource *src)
{
    return genRand(src->gen);
}

static RandomSource *
mt_destroy(RandomSource *src)
{
    free(src->ops);
    free(src->gen);
    free(src);

    return NULL;
}

RandomSource *
random_mt_factory(const char *params)
{
    errno = 0;
    char *eptr = NULL;
    long seed = strtol(params, &eptr, DEC_RADIX);
    if (errno || *eptr || eptr == params || (int) seed != seed || seed < 0) {
        fprintf(stderr, "Incorrect seed");

        return NULL;
    }

    seed &= INT_MAX;

    RandomSource *result = calloc(1, sizeof(*result));
    if (result == NULL) {
        fprintf(stderr, "Error allocating RandomSource\n");

        return NULL;
    }

    RandomSourceOperations *ops = calloc(1, sizeof(*ops));
    if (ops == NULL) {
        fprintf(stderr, "Error allocating RandomSourceOperations\n");

        return NULL;
    }

    ops->next = mt_next;
    ops->destroy = mt_destroy;

    result->ops = ops;

    MTRand *gen = calloc(1, sizeof(*gen));
    if (gen == NULL) {
        fprintf(stderr, "Error allocating MTRand\n");

        return NULL;
    }

    *gen = seedRand(seed);

    result->gen = gen;

    return result;
}
