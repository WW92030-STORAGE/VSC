# utils/math/BASE

`namespace BASE`

## Description

Mathematical methods that only use native C++ and not custom data types in VSC.

## Notes

- Some things resemble methods in `std::...`.

## Functions

- `bool fzero(float f)` = Is the float close enough to zero (within a small threshold)?
- `bool fequal(float f, float g)` = Are the two floats close enough to each other?
- `float round(float f)`
- `int ifloor(float f)`
- `int iceil(float f)`
- `int iround(float f)`
- `float lerp(float f, float g, float t)` = Linear interpolate between `f (t = 0)` and `g (t = 1)` using the time `t`.
- `float pow(float f, int g)`
- `int min(int a, int b)`
- `int max(int a, int b)`
- `float clamp(float f, float l, float h)` = Clamps `f` between `l` and `h`.
- `int iclamp(int i, int l, int h)`
- `int64_t gcd(int64_t x, int64_t y)`
- `int64_t lcm(int64_t x, int64_t y)`
- `void randomseed()` = Seeds using the time.
- `float random()` = Random float between 0 and 1.
- `float deriv(float (*f)(float), float x)` = Approximates the derivative of a function `f(float x)` at a point `x`.
- `float newton(float (*f)(float))` = Uses Newton's Method to approximate a root of `f(float x)` within a bound on `x` values.

# Static Instances

- `#define M_PI 3.14159265358979323846`
- `#define INF FLT_MAX` = `std::FLT_MAX` from `<climits>` and `<cfloat>`