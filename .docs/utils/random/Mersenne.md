# .utils/random/Mersenne

`class MersenneTwister : public RandomSource`

## Description

Mersenne Twister

## Fields

- `mti: int` = State index
- `mt: uint64_t[]` = State array
- `NN, MM, MATRIX_A, UM, LM, SEED_COEFF, SEED_SHR: uint64_t` = Constants.

## Constructors

- `MersenneTwister(int seed = 0)`

