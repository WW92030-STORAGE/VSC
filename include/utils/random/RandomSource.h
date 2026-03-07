#ifndef RANDOMSOURCE_EXT
#define RANDOMSOURCE_EXT

#include <cstdint>
#include <cmath>

class RandomSource {
    public:
    uint64_t state = 0;
    RandomSource();

    RandomSource(uint64_t seed);

    virtual void init(uint64_t seed);

    virtual uint64_t rand64();

    uint32_t rand32();

    double rand();

    virtual double operator()();
};

#endif