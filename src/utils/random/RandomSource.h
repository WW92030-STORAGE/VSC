#ifndef RANDOMSOURCE_EXT
#define RANDOMSOURCE_EXT

#include <cstdint>
#include <cmath>

class RandomSource {
    public:
    RandomSource() {
        init(0);
    }

    RandomSource(uint64_t seed) {
        init(seed);
    }

    virtual inline void init(uint64_t seed) {
        
    }

    virtual inline uint64_t rand64() {
        return 0;
    }

    inline uint32_t rand32() {
        return rand64() % (1ULL<<32);
    }

    inline double rand() {
        return (rand64() >> 11) * (1.0/9007199254740992.0);
    }

    virtual inline double operator()() {
        return 0;
    }
};

#endif