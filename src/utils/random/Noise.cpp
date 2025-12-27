#include <cstdint>
#include <cmath>

#include "../../../include/utils/random/Mersenne.h"
#include "../../../include/utils/random/RandomSource.h"



#include "../../../include/utils/random/Noise.h"

    NoiseOctave2::NoiseOctave2() {
        rs = new MersenneTwister();

        init(0);
    }

    NoiseOctave2::NoiseOctave2(uint64_t seed) {
        rs = new MersenneTwister();

        init(seed);
    }

    void NoiseOctave2::init(uint64_t seed) {
        rs->init(seed);
    }

    float NoiseOctave2::rand(float a, float b) {
        return 0;
    }

    NoiseOctave2::~NoiseOctave2() {
        delete rs;
    }