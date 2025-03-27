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
    MersenneTwister() {
        init(0);
    }

    MersenneTwister(uint64_t seed) {
        init(seed);
    }

    inline void init(uint64_t seed) {
        mt[0] = seed;
        for (mti = 1; mti < NN; mti++) 
        mt[mti] = (SEED_COEFF * (mt[mti-1] ^ (mt[mti-1] >> SEED_SHR)) + mti);
    }

    inline uint64_t rand64() {
        int i;
        unsigned long long x;
        static unsigned long long mag01[2]={0ULL, MATRIX_A};

        if (mti >= NN) { /* generate NN words at one time */

        /* if init_genrand64() has not been called, */
        /* a default initial seed is used     */
            if (mti == NN+1) 
            init(5489ULL); 

            for (i=0;i<NN-MM;i++) {
                x = (mt[i]&UM)|(mt[i+1]&LM);
                mt[i] = mt[i+MM] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
            }
            for (;i<NN-1;i++) {
                x = (mt[i]&UM)|(mt[i+1]&LM);
                mt[i] = mt[i+(MM-NN)] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
            }
            x = (mt[NN-1]&UM)|(mt[0]&LM);
            mt[NN-1] = mt[MM-1] ^ (x>>1) ^ mag01[(int)(x&1ULL)];

            mti = 0;
        }
  
        x = mt[mti++];

        x ^= (x >> 29) & 0x5555555555555555ULL;
        x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
        x ^= (x << 37) & 0xFFF7EEE000000000ULL;
        x ^= (x >> 43);

        return x;
    }

    inline double operator()() {
        return rand();
    }
};

#endif