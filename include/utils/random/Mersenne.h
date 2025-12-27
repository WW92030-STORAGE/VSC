#ifndef MERSENNE_EXT
#define MERSENNE_EXT

#include <cstdint>
#include <cmath>

#include "RandomSource.h"

// https://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/VERSIONS/C-LANG/mt19937-64.c

class MersenneTwister : public RandomSource {
    private:
    const static uint64_t NN = 312;
    const uint64_t MM = 156;
    const uint64_t MATRIX_A = 0xB5026F5AA96619E9ULL;
    const uint64_t UM = 0xFFFFFFFF80000000ULL;
    const uint64_t LM = 0x7FFFFFFFULL;

    const uint64_t SEED_COEFF = 6364136223846793005ULL;
    const uint64_t SEED_SHR = 62;



    int mti=NN+1; // state index
    uint64_t mt[NN]; // state array

    public:
    MersenneTwister();

    MersenneTwister(uint64_t seed);

    void init(uint64_t seed);

    uint64_t rand64();

    double operator()();
};

#endif