#ifndef BASENOISE_H
#define BASENOISE_H

#include <cstdint>
#include <cmath>

class BaseNoise2 {
    protected:
    RandomSource* rs;

    public:
    BaseNoise2() {
        rs = new MersenneTwister();

        init(0);
    }

    BaseNoise2(uint64_t seed) {
        rs = new MersenneTwister();

        init(seed);
    }

    virtual inline void init(uint64_t seed) {
        rs->init(seed);
    }

    virtual inline float rand(float a, float b) {
        return 0;
    }

    ~BaseNoise2() {
        delete rs;
    }
};

#endif