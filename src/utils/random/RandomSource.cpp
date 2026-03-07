#include <cstdint>
#include <cmath>



#include "../../../include/utils/random/RandomSource.h"

    RandomSource::RandomSource() {
        init(0);
    }

    RandomSource::RandomSource(uint64_t seed) {
        init(seed);
    }

    void RandomSource::init(uint64_t seed) {
        state = seed;
    }

    uint64_t RandomSource::rand64() {
	    state ^= state << 13;
	    state ^= state >> 7;
	    state ^= state << 17;

        return state;
    }

    uint32_t RandomSource::rand32() {
        return rand64() % (1ULL<<32);
    }

    double RandomSource::rand() {
        return (rand64() >> 11) * (1.0/9007199254740992.0);
    }

    double RandomSource::operator()() {
        return 0;
    }