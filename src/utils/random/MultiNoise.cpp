
#include "../../../include/utils/random/Noise.h"
#include "../../../include/utils/random/PerlinNoise.h"


#include "../../../include/utils/random/MultiNoise.h"


    FractalNoise2::FractalNoise2(NoiseOctave2* NO, int numoc, float att, float sc) {
        octave = NO;
        numOctaves = numoc;
        atten = att;
        scale = sc;
    }

    float FractalNoise2::rand(float x, float y) {
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

    FractalNoise2::~FractalNoise2() {
    }