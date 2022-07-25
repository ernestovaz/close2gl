#pragma once

#include <vector>

#include <glm/glm.hpp>

using std::vector;

using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Close2GL {
    mat4 getModelViewProjection(mat4 model, mat4 view, mat4 projection);
    vector<vec3> transformPositions(vector<vec3> positions, mat4 modelViewProjection);
}

