#include "Close2GL.h"

vector<vec3> Close2GL::transformPositions(vector<vec3> positions, mat4 modelViewProjection) {
    int vertexCount = positions.size();
    vector<vec3> transformedVector(vertexCount);
    for (int i = 0; i < vertexCount; i++) {
        vec4 newPosition(positions[i], 1.0f);
        newPosition = modelViewProjection * newPosition;
        newPosition.x /= newPosition.w;
        newPosition.y /= newPosition.w;
        newPosition.z /= newPosition.w;
        transformedVector[i] = newPosition;
    }
    return transformedVector;
}

mat4 Close2GL::projectionMatrix(float FOVx, float FOVy, float nearPlane, float farPlane) {

    float nearPlaneWidth = 2 * tan(FOVx/2) * (nearPlane);
    float nearPlaneHeight = 2 * tan(FOVy/2) * (nearPlane);
    float frustumLength = farPlane - nearPlane;

    mat4 projection = mat4(0.0);

    projection[0][0] = 2 * nearPlane / nearPlaneWidth;
    projection[1][1] = 2 * nearPlane / nearPlaneHeight;
    projection[2][2] = -(farPlane + nearPlane)/ frustumLength;
    projection[3][2] = - 2 * farPlane * nearPlane / frustumLength;
    projection[2][3] = -1;

    return projection;
}

mat4 Close2GL::viewMatrix(vec3 cameraPosition, vec3 cameraDirection, vec3 cameraUp) {
    vec3 n = normalize(-cameraDirection);
    vec3 u = cross(normalize(cameraUp), n);
    vec3 v = cross(n,u);
    mat4 projection(0.0);

    projection[0][0] = u.x;
    projection[1][0] = u.y;
    projection[2][0] = u.z;
    projection[3][0] = dot(-cameraPosition, u);

    projection[0][1] = v.x;
    projection[1][1] = v.y;
    projection[2][1] = v.z;
    projection[3][1] = dot(-cameraPosition, v);

    projection[0][2] = n.x;
    projection[1][2] = n.y;
    projection[2][2] = n.z;
    projection[3][2] = dot(-cameraPosition, n);

    projection[3][3] = 1;

    return projection;
}

float Close2GL::horizontalFieldOfView(float FOVy, float screenWidth, float screenHeight) {
    return 2.0f * atan(tan(FOVy/2.0f)/screenHeight*screenWidth);
}
