#include "Close2GL.h"

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

mat4 Close2GL::projectionMatrix(float FOVx, float FOVy, float nearPlane, float farPlane) {

    float frustumLength = farPlane - nearPlane;
    float farPlaneWidth = 2 * tan(FOVx) * (frustumLength);
    float farPlaneHeight = 2 * tan(FOVy) * (frustumLength);

    mat4 projection = mat4(0.0);

    projection[0][0] = 2 * nearPlane / farPlaneWidth;
    projection[1][1] = 2 * nearPlane / farPlaneHeight;
    projection[2][2] = -(farPlane + nearPlane)/ frustumLength;
    projection[3][2] = 2 * farPlane * nearPlane / frustumLength;
    projection[2][3] = -1;

    return projection;
}
