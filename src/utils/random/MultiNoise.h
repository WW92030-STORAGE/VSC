#ifndef MULTINOISE_EXT
#define MULTINOISE_EXT

#include "Noise.h"
#include "PerlinNoise.h"

inline float RandWithCast2(NoiseOctave2* no, float x, float y) {
    PerlinNoise2* p = dynamic_cast<PerlinNoise2*>(no);
    if (p) return p->rand(x, y);

    return no->rand(x, y);
}

class FractalNoise2 {
    public:
    NoiseOctave2* octave = nullptr;
    int numOctaves = 4;
    float atten = 0.5;
    float scale = 2.0;

    FractalNoise2(NoiseOctave2* NO, int numoc = 4, float att = 0.5, float sc = 2.0) {
        octave = NO;
        numOctaves = numoc;
        atten = att;
        scale = sc;
    }

    inline float rand(float x, float y) {
        float pp = 1.0;
        float nn = 0.0;
        float res = 0.0;

        for (int i = 0; i < numOctaves; i++) {
            nn += pp;
            res += pp * RandWithCast2(octave, x, y);
            pp *= atten;

            x *= scale;
            y *= scale;
        }
        if (BASE::fzero(nn)) return res;
        return res / nn;
    }

    ~FractalNoise2() {
    }
};

#endif