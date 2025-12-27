#ifndef INERTIA_VSC_PHYS
#define INERTIA_VSC_PHYS

#include <string>
#include <cstdint>
#include <algorithm>

#include "../utils/Math.h"
#include "Constants.h"

namespace Inertia {

static Matrix3 sphere_solid(float mass = 1.0, float radius = 1.0) {
    return Matrix3::eye_scaled(0.4 * mass * radius * radius);
}

static Matrix3 sphere_hollow(float mass = 1.0, float radius = 1.0) {
    return Matrix3::eye_scaled((2.0 / 3.0) * mass * radius * radius);
}

static Matrix3 box(float mass = 1.0, float sx = 1.0, float sy = 1.0, float sz = 1.0) {
    float xsq = sx * sx;
    float ysq = sy * sy;
    float zsq = sz * sz;
    float c = 1.0 / 12.0;
    return Matrix3::diagonal(c * (ysq + zsq), c * (xsq + zsq), c * (ysq + zsq));
}

}

#endif