#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "linear.h"
#include "random_source.h"

enum : long long
{
    MULTIPLIER = 1103515245LL,
    INCREMENT = 12345LL,
    MODULUS = 1LL << 31,
};

typedef struct LinearRand
{
    long long prev;
} LinearRand;

static double
linear_next(RandomSource *src)
{
    LinearRand *gen = src->gen;

    long long result = (MULTIPLIER * gen->prev + INCREMENT) % MODULUS;

    gen->prev = result;
    src->gen = gen;

    return (double) result / ((double) INT_MAX + 1);
}

static RandomSource *
linear_destroy(RandomSource *src)
{
    free(src->ops);
    free(src->gen);
    free(src);

    return NULL;
}

RandomSource *
random_linear_factory(const char *params)
{
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

    ops->next = linear_next;
    ops->destroy = linear_destroy;

    result->ops = ops;

    errno = 0;
    char *eptr = NULL;
    long seed = strtol(params, &eptr, DEC_RADIX);
    if (errno || *eptr || eptr == params || (int) seed != seed || seed < 0) {
        fprintf(stderr, "Incorrect seed");

        return NULL;
    }

    seed &= INT_MAX;

    LinearRand *gen = calloc(1, sizeof(*gen));
    if (gen == NULL) {
        fprintf(stderr, "Error allocating LinearRand\n");

        return NULL;
    }

    gen->prev = seed;
    result->gen = gen;

    return result;
}
