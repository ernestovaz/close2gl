#include "Close2GL.h"

mat4 Close2GL::getModelViewProjection(mat4 model, mat4 view, mat4 projection) {
    return projection * view * model;
}

vector<vec3> Close2GL::transformPositions(vector<vec3> positions, mat4 modelViewProjection) {
    vector<vec3> transformedVector;
    for (vec3 position : positions) {
        vec4 newPosition(position, 1.0f);
        newPosition = modelViewProjection * newPosition;
        newPosition /= newPosition.w;
        transformedVector.push_back(newPosition);
    }
    return transformedVector;
}
