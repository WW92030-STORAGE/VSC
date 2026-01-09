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

static Fragment identity(Fragment f) {
    return Fragment(f);
}

static Fragment invert(Fragment f) {
    Fragment res(f);
    
    uint32_t newcolor = f.color;
    res.color = setRGBA(255 - getRed(newcolor), 255 - getGreen(newcolor), 255 - getBlue(newcolor), getAlpha(newcolor));
    return res; // TODO figure this one out
}

static Fragment hue_offset(Fragment f, float offset = 0.5) {
    Fragment res(f);
    Vector3 hsv = rgb2hsv(rgb(f.color));
    hsv.x = BASE::frem(hsv.x + offset, 1);
    res.color = rgb(hsv2rgb(hsv));
    return res;
}

static Fragment rainbow_radial(Fragment f, Vector2 center = Vector2(0.5, 0.5)) {
    Vector2 polar = rect2polar(f.screenUV - center);

    float offset = BASE::frem(polar.y, 2 * M_PI) / (2 * M_PI);

    Vector3 hsv = rgb2hsv(rgb(f.color));
    hsv.x = BASE::frem(hsv.x + offset, 1);

    Fragment res(f);
    res.color = rgb(hsv2rgb(hsv));
    return res;
}

static Fragment rainbow_concentric(Fragment f, Vector2 center = Vector2(0.5, 0.5), float speed = 4) {
    Vector2 polar = rect2polar(f.screenUV - center);

    float offset = BASE::frem(polar.x * speed, 1);

    Vector3 hsv = rgb2hsv(rgb(f.color));
    hsv.x = BASE::frem(hsv.x + offset, 1);

    Fragment res(f);
    res.color = rgb(hsv2rgb(hsv));
    return res;
}

static Fragment rainbow_spiral(Fragment f, Vector2 center = Vector2(0.5, 0.5), float speed = 4) {
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

    FragShader();

    FragShader(FRAG_SHADER shad);

    FragShader(const FragShader& other);

    Fragment operator()(Fragment f);
};

#endif