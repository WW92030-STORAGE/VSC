#ifndef PERLINNOISE_EXT
#define PERLINNOISE_EXT

#include <cmath>
#include <climits>
#include <cstdint>
#include <cfloat>

#include "../Math.h"

class PerlinNoise2 : BaseNoise2 {
    uint64_t xm;
    uint64_t xb;
    uint64_t ym;
    uint64_t yb;
    public:
    uint64_t seed = 0;
    float spacing = 1;

    PerlinNoise2() : BaseNoise2() {
        init(0);
    }

    PerlinNoise2(uint64_t sed) : BaseNoise2(seed) {
        init(sed);
    }

    inline void init(uint64_t sed) {
        seed = sed;
        rs->init(sed);
        xm = rs->rand64();
        xb = rs->rand64();
        ym = rs->rand64();
        yb = rs->rand64();
    }

    inline Vector2 randvec(int64_t x, int64_t y) {
        rs->init(x * xm + xb);
        uint64_t xseed = rs->rand32();
        rs->init(y * ym + yb);
        rs->rand32();
        uint64_t yseed = rs->rand32();

        rs->init(yseed + (xseed<<32));
        double dx = rs->rand() - 0.5;
        double dy = rs->rand() - 0.5;

        Vector2 res(dx, dy);
        if (isZero(res)) return Vector2(1, 0);

        return res.normalized();
    }

    inline double smooth(double x) {
        return 0.5 * (1 - cosf(M_PI * x));
    }

    inline double rand(double x, double y) {
        if (spacing < 0) spacing = -spacing;
        if (BASE::fzero(spacing)) spacing = 1;
        int xcell = BASE::ifloor(x / spacing);
        int ycell = BASE::ifloor(y / spacing);

        Vector2 TL = randvec(xcell, ycell);
        Vector2 TR = randvec(xcell + 1, ycell);
        Vector2 BL = randvec(xcell, ycell + 1);
        Vector2 BR = randvec(xcell + 1, ycell + 1);

        Vector2 grads[4] = {TL, TR, BL, BR};
        Vector2 corners[4] = {Vector2(xcell, ycell), Vector2(xcell + 1, ycell), Vector2(xcell, ycell + 1), Vector2(xcell + 1, ycell + 1)};

        Vector2 scaledpoint(x / spacing, y / spacing);
        float s[4];
        for (int i = 0; i < 4; i++) s[i] = (scaledpoint - corners[i]).dot(grads[i]);

        double u = smooth(x / spacing - xcell);
        double v = smooth(y / spacing - ycell);

        return (1 - v) * ((1 - u) * s[0] + u * s[1]) + v * ((1 - u) * s[2] + u * s[3]);
    }
};

#endif