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

    PerlinNoise2() : NoiseOctave2() {
        init(0);
    }

    PerlinNoise2(uint64_t sed) : NoiseOctave2(seed) {
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
        double theta = rs->rand() * M_PI * 2.0;

        Vector2 res(cos(theta), sin(theta));
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
        Vector2 scaledpoint(x / spacing, y / spacing);

        Vector2 TL_ = randvec(xcell, ycell);
        Vector2 TR_ = randvec(xcell + 1, ycell);
        Vector2 BL_ = randvec(xcell, ycell + 1);
        Vector2 BR_ = randvec(xcell + 1, ycell + 1);

        Vector2 grads[4] = {TL_, TR_, BL_, BR_};
        Vector2 corners[4];
        for (int i = 0; i < 4; i++) corners[i] = Vector2(xcell + i % 2, ycell + i / 2);
        float s[4];
        for (int i = 0; i < 4; i++) s[i] = (scaledpoint - corners[i]).dot(grads[i]);

        double u = smooth(scaledpoint.x - xcell);
        double v = smooth(scaledpoint.y - ycell);

        float TOP = BASE::lerp(s[0], s[1], u);
        float BOT = BASE::lerp(s[2], s[3], u);

        // return (1 - v) * ((1 - u) * s[0] + u * s[1]) + v * ((1 - u) * s[2] + u * s[3]);

        return 0.5 * (1 + BASE::lerp(TOP, BOT, v));
    }
};

#endif
