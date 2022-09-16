#include "Shader.h"

#include <glm/glm.hpp>

using glm::dot;
using glm::max;
using glm::pow;
using glm::normalize;
using glm::clamp;

Shader::Shader(vec3 lightPosition, vec3 eyePosition, vec3 objectColor)
: lightPosition(lightPosition), eyePosition(eyePosition), objectColor(objectColor)
{}

vec3 Shader::applyPhongLightingModel(vec3 vertex, vec3 normal) {
    vec3 color = vec3(0.0f);
    vec3 eyeDirection   = normalize(eyePosition - vertex);
    vec3 lightDirection = normalize(lightPosition - vertex);

    color += DIFFUSE_REFLECTANCE * 
        diffuseReflection(LIGHT_COLOR, normal, lightDirection);

    color += AMBIENT_REFLECTANCE * 
        ambientReflection(AMBIENT_LIGHT_COLOR);

    color += SPECULAR_REFLECTANCE * 
        specularReflection(LIGHT_COLOR, normal, lightDirection, eyeDirection, SHININESS);
    
    return clamp(color *  objectColor, 0.0f, 255.0f);
}

vec3 Shader::diffuseReflection(vec3 light, vec3 normal, vec3 lightDirection) {
    return light * glm::max(0.0f, glm::dot(normal, lightDirection));
}

vec3 Shader::ambientReflection(vec3 light) {
    return light;
}

vec3 Shader::specularReflection(vec3 light, vec3 normal, vec3 lightDirection, vec3 eyeDirection, float shininess) {
    vec3 refraction = -lightDirection + 2.0f * normal * dot(normal, lightDirection);
    return light * pow(max(0.0f, dot(refraction, eyeDirection)), shininess);
}