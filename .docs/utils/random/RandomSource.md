# .utils/random/RandomSource

`class RandomSource`

## Description

Base class for random number generators.

## Notes

- I don't know why this exists.

## Constructors

- `RandomSource(int seed = 0)`

## Functions

- `virtual void init(uint64_t seed)` = Initialize with the given seed.
- `virtual uint64_t rand64()` = Generate a random `uint64_t` from the seed and the sequence of past random numbers generated.
- `uint32_t rand32()` = Generate a `uint32_t`.
- `double rand()` = Generate a `double : [0, 1)`.