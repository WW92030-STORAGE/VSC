#ifndef SHADER_EXT
#define SHADER_EXT

#include "../utils/geometry/Fragment.h"
#include "Color.h"

/*

Functions that can act as shaders

*/

typedef Fragment (*FRAG_SHADER)(Fragment);

namespace FragShader {

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

}


#endif