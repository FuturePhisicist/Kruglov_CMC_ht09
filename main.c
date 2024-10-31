#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "random_source.h"

enum
{
    REQUIRED_ARGUMENTS_NUM = 3 + 1,
    MIN_N = 0,
    MAX_N = 100000,
};

int
main(int argc, char **argv)
{
    if (argc < REQUIRED_ARGUMENTS_NUM) {
        fprintf(stderr, "Usage: %s GENERATOR_TYPE N SEED\n", argv[0]);

        return EXIT_FAILURE;
    }

    void *handle = dlopen(NULL, RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "Error loading neccessary shared object\n");

        return EXIT_FAILURE;
    }

    const char *GENERATOR_TYPE = argv[1];
    RandomSource *(*fn)(const char *);
    if (!strcmp(GENERATOR_TYPE, "linear")) {
        void *sym = dlsym(handle, "random_linear_factory");

        if (sym == NULL) {
            goto emergency_exit;
        }

        fn = (RandomSource * (*) (const char *) ) sym;

    } else if (!strcmp(GENERATOR_TYPE, "mt")) {
        void *sym = dlsym(handle, "random_mt_factory");

        if (sym == NULL) {
            goto emergency_exit;
        }

        fn = (RandomSource * (*) (const char *) ) sym;
    } else {
        fprintf(stderr, "Error getting generator\n");

        goto emergency_exit;
    }

    const char *N = argv[2];
    errno = 0;
    char *eptr = NULL;
    long n = strtol(N, &eptr, DEC_RADIX);
    if (errno || *eptr || eptr == N || (int) n != n || n < MIN_N || n > MAX_N) {
        fprintf(stderr, "Wrong N\n");

        goto emergency_exit;
    }

    const char *SEED = argv[3];
    RandomSource *generator = fn(SEED);
    if (generator == NULL)
    {
        fprintf(stderr, "Error making generator\n");

        goto emergency_exit;
    }


    for (int i = 0; i < n; ++i) {
        printf("%.10g\n", generator->ops->next(generator));
    }

    generator = generator->ops->destroy(generator);

    if (dlclose(handle) != 0)
    {
        fprintf(stderr, "Error closing shared object\n");

        goto emergency_exit;
    }

    return EXIT_SUCCESS;

emergency_exit:
    if (dlclose(handle) != 0)
    {
        fprintf(stderr, "Error closing shared object\n");
    }

    return EXIT_FAILURE;
}
