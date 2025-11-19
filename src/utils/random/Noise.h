#ifndef BASENOISE_H
#define BASENOISE_H

#include <cstdint>
#include <cmath>

#include "Mersenne.h"
#include "RandomSource.h"


class NoiseOctave2 {
    protected:
    RandomSource* rs;

    public:
    NoiseOctave2() {
        rs = new MersenneTwister();

        init(0);
    }

    NoiseOctave2(uint64_t seed) {
        rs = new MersenneTwister();

        init(seed);
    }

    virtual inline void init(uint64_t seed) {
        rs->init(seed);
    }

    virtual inline float rand(float a, float b) {
        return 0;
    }

    virtual ~NoiseOctave2() {
        delete rs;
    }
};

#endif
