# Random Number Generator Project (ht09)

This project implements a random number generation program, `printrandom`, which uses two types of random number generators: a Linear Congruential Generator (LCG) and a Mersenne Twister (MT). The program is designed to be compiled with a `Makefile`, using shared code organization and dynamic linking.

## Table of Contents
- [Overview](#overview)
- [Compilation](#compilation)
- [Usage](#usage)
- [Detailed Description](#detailed-description)
- [Example](#example)

## Overview

`printrandom` generates uniformly distributed random numbers in the range `[0, 1)`, with support for two types of random number generators:
1. **Linear Congruential Generator (LCG)**
2. **Mersenne Twister (MT)**

The program accepts the generator type, count of numbers to generate, and a seed as command-line arguments. It then initializes the specified generator, produces the requested count of random numbers, and outputs them in a formatted manner.

## Compilation

To compile the program, ensure that all files are in the project directory, then run:

```bash
make
```

This will produce an executable called `printrandom`. To clean up object files and the executable, run:

```bash
make clean
```

## Usage

The program requires three command-line arguments:
1. **Generator type** (`mt` or `linear`)
2. **Number of random numbers** to generate (integer between 0 and 100000)
3. **Seed** for initializing the generator

### Example Usage

```bash
./printrandom linear 5 12345
```

This example runs the program with the Linear Congruential Generator, generating 5 random numbers using the seed `12345`.

## Detailed Description

### Random Number Generators

1. **Linear Congruential Generator (LCG)**:
   - Uses multiplier `1103515245`, increment `12345`, and modulus `2^31`.
   - Seed is specified as a positive integer, with only the lower 31 bits used.

2. **Mersenne Twister (MT)**:
   - Uses a 32-bit Mersenne Twister algorithm.
   - Seed is a 32-bit unsigned integer.
   - As stated in the task, its implementation was taken from [here](https://github.com/blackav/mtwister/tree/master)

### `RandomSource` Interface

The `RandomSource` interface allows seamless switching between different generator implementations. Each generator type implements:
- `double next(RandomSource *src)`: Generates the next random number.
- `RandomSource *destroy(RandomSource *src)`: Cleans up and frees resources associated with the generator.

### `random_factory` Functions

Each generator has a specific factory function to initialize it:
- `random_linear_factory`: Initializes an LCG generator.
- `random_mt_factory`: Initializes an MT generator.

The `main.c` file loads the correct factory function using `dlsym` based on the command-line argument.

## Example

To generate 10 random numbers using the Mersenne Twister generator with seed `123456`:

```bash
./printrandom mt 5 123456
```

Expected output:
```plaintext
0.491462737
0.5018269304
0.2805587475
0.2943618261
0.347394317
```

Each line is a new random number formatted to 10 significant figures.
