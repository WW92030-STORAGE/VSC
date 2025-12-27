#ifndef PERLINNOISE_EXT
#define PERLINNOISE_EXT

#include <cmath>
#include <climits>
#include <cstdint>
#include <cfloat>

#include "../Math.h"
#include "Noise.h"

class PerlinNoise2 : public NoiseOctave2 {
    uint64_t xm;
    uint64_t xb;
    uint64_t ym;
    uint64_t yb;
    public:
    uint64_t seed = 0;
    double spacing = 1;

    PerlinNoise2();

    PerlinNoise2(uint64_t sed);

    void init(uint64_t sed);

    Vector2 randvec(int64_t x, int64_t y);

    double smooth(double x);

    double rand(double x, double y);
};

#endif
