#ifndef SHADER_EXT
#define SHADER_EXT

#include "../utils/geometry/Fragment.h"
#include "Color.h"
#include "../utils/Math.h"

#include <vector>
#include <any>

/*

Functions that can act as shaders

*/

typedef Fragment (*FRAG_SHADER)(Fragment);

namespace FragShaders {

Fragment identity(Fragment f) {
    return Fragment(f);
}

Fragment invert(Fragment f) {
    Fragment res(f);
    Vector3 color = rgb(f.color);
    color.x = 1 - color.x;
    color.y = 1 - color.y;
    color.z = 1 - color.z;

    res.color = rgb(color);
    return res;
}

Fragment hue_offset(Fragment f, float offset = 0.5) {
    Fragment res(f);
    Vector3 hsv = rgb2hsv(rgb(f.color));
    hsv.x = BASE::frem(hsv.x + offset, 1);
    res.color = rgb(hsv2rgb(hsv));
    return res;
}

Fragment rainbow_radial(Fragment f, Vector2 center = Vector2(0.5, 0.5)) {
    Vector2 polar = rect2polar(f.screenUV - center);

    float offset = BASE::frem(polar.y, 2 * M_PI) / (2 * M_PI);

    Vector3 hsv = rgb2hsv(rgb(f.color));
    hsv.x = BASE::frem(hsv.x + offset, 1);

    Fragment res(f);
    res.color = rgb(hsv2rgb(hsv));
    return res;
}

Fragment rainbow_concentric(Fragment f, Vector2 center = Vector2(0.5, 0.5), float speed = 4) {
    Vector2 polar = rect2polar(f.screenUV - center);

    float offset = BASE::frem(polar.x * speed, 1);

    Vector3 hsv = rgb2hsv(rgb(f.color));
    hsv.x = BASE::frem(hsv.x + offset, 1);

    Fragment res(f);
    res.color = rgb(hsv2rgb(hsv));
    return res;
}

Fragment rainbow_spiral(Fragment f, Vector2 center = Vector2(0.5, 0.5), float speed = 4) {
    Vector2 polar = rect2polar(f.screenUV - center);

    float offset = BASE::frem(polar.y - speed * polar.x, 2 * M_PI) / (2 * M_PI);

    Vector3 hsv = rgb2hsv(rgb(f.color));
    hsv.x = BASE::frem(hsv.x + offset, 1);

    Fragment res(f);
    res.color = rgb(hsv2rgb(hsv));
    return res;
}

}


struct FragShader {
    FRAG_SHADER shader;

    FragShader() {
        shader = FragShaders::identity;
    }

    FragShader(FRAG_SHADER shad) {
        shader = shad;
    }

    FragShader(const FragShader& other) {
        shader = other.shader;
    }

    Fragment operator()(Fragment f) {
        return shader(f);
    }
};

#endif