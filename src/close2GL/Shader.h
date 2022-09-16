#pragma once

#include <glm/vec3.hpp>

using glm::vec3;

#define DIFFUSE_REFLECTANCE     vec3(1.0, 1.0, 1.0)
#define AMBIENT_REFLECTANCE     vec3(1.0, 1.0, 1.0)
#define SPECULAR_REFLECTANCE    vec3(1.0, 1.0, 1.0)
#define AMBIENT_LIGHT_COLOR     vec3(0.1, 0.1, 0.1)
#define LIGHT_COLOR             vec3(1.0, 1.0, 1.0)
#define LIGHT_DIRECTION         vec3(0.0, 1.0, 0.0)
#define SHININESS               4.0f

class Shader {
public:
    Shader(vec3 lightPosition, vec3 eyePosition, vec3 objectColor);
    vec3 applyPhongLightingModel(vec3 vertex, vec3 normal);

private:
    vec3 lightPosition;
    vec3 eyePosition;
    vec3 objectColor;
    
    vec3 ambientReflection(vec3 light);
    vec3 diffuseReflection(vec3 light, vec3 normal, vec3 lightDirection);
    vec3 specularReflection(vec3 light, vec3 normal, vec3 lightDirection, vec3 eyePosition, float shininess);
};