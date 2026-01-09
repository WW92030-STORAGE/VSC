#ifndef MATH_BASE_EXT
#define MATH_BASE_EXT

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <climits>
#include <limits>

// Base math methods that only use numerical values

// Defines

#define M_PI 3.14159265358979323846
#define INF FLT_MAX

namespace BASE {

// Is it zero?
static inline bool fzero(float f) {
    if (f == 0.0) return true;
	constexpr float E = std::numeric_limits<float>::epsilon();
	return (-E < f && f < E);
}

// Is a float nan?

static bool fnan(float f) {
    return std::isnan(f);
}

// Are they equal?

inline bool fequal(float f, float g) {
    if (fnan(f) && fnan(g)) return true;
    if (f == g) return true;
    return fzero(f - g);
}

// Rounding

static inline float round(float f) {
    return floorf(f + 0.5);
}

static inline int ifloor(float f) {
    return (int)(floorf(f));
}

static inline int iceil(float f) {
    return (int)(ceilf(f));
}

static inline int iround(float f) {
    return (int)(round(f));
}

static inline float ReLU(float f) {
    if (f > 0) return f;
    return 0;
}

// Mod

static inline float frem(float a, float m) {
    float res = fmod(a, m);
    while (res < 0) res += m;
    return fmod(res, m);
}

static inline int irem(int a, int m) {
    int res = a % m;
    while (res < 0) res += m;
    return res % m;
}

// LERP

static inline float lerp(float f, float g, float t) {
    return (1 - t) * f + t * g;
}

// Exponents

static inline float pow(float f, int g) {
    if (fzero(f)) return 0;
    if (g < 0) return 1.0 / pow(f, -g);
    if (g == 0) return 1.0;

    if (g & 1) return f * pow(f, g - 1);
    float h = pow(f, g>>1);
    return h * h;
}

// Min/max

static inline int min(int a, int b) {
    return a < b ? a : b;
}

static inline int max(int a, int b) {
    return a > b ? a : b;
}

static inline float clamp(float f, float l, float h) {
    if (f < l) return l;
    if (f > h) return h;
    return f;
}

static inline int iclamp(int i, int l, int h) {
    if (i < l) return l;
    if (i > h) return h;
    return i;
}

// Number theory

static inline int64_t gcd(int64_t x, int64_t y) {
    if (x == 0 && y == 0) return 1;
    if (x < 0 || y < 0) return gcd(abs(x), abs(y));
    if (x < y) return gcd(y, x);

    if (y == 0) return x;
    return gcd(x % y, y);
}

static inline int64_t lcm(int64_t x, int64_t y) {
    if (abs(x) > abs(y)) return lcm(y, x);

    return x * (y / gcd(x, y));
}

// Random number (Alternatively there is a Mersenne twister in the ../random folder.)

static inline void randomseed() {
    srand(time(0));
}

static inline void randomseed(uint64_t n) {
    srand(n);
}

static inline float random() {
    return (float)(rand()) / (float(RAND_MAX));
}

// Calc shit
static inline float deriv(float (*f)(float), float x) {
    const float d = 0.0001;
    return (f(x + d) - f(x - d)) / (d + d);
}

static float newton(float (*f)(float)) {
    const float ITER = 256;
    const int BOUND = 1024;
    const float E = 0.0001;
    
    float x = 0;
    float minim = f(0);
    
    for (int i = -BOUND; i < BOUND; i++) {
        if (fnan(f(i))) continue;
        if (fabs(f(i)) < fabs(minim)) {
            minim = f(i);
            x = i;
        }
    }
    
    for (int i = 0; i < ITER; i++) {
        float dd = deriv(f, x);
        if (fzero(dd)) dd += 0.00001;
        x = x - f(x) / dd;
    }
    return x;
}

};

#endif