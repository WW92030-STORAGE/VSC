#include "../../include/utils/geometry/Fragment.h"
#include "../../include/scene/Color.h"
#include "../../include/utils/Math.h"

#include <vector>
#include <any>



#include "../../include/scene/Shader.h"

/*

Functions that can act as shaders

*/

    FragShader::FragShader() {
        shader = FragShaders::identity;
    }

    FragShader::FragShader(FRAG_SHADER shad) {
        shader = shad;
    }

    FragShader::FragShader(const FragShader& other) {
        shader = other.shader;
    }

    Fragment FragShader::operator()(Fragment f) {
        return shader(f);
    }