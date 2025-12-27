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
    NoiseOctave2();

    NoiseOctave2(uint64_t seed);

    virtual void init(uint64_t seed);

    virtual float rand(float a, float b);

    virtual ~NoiseOctave2();
};

#endif
