#include "ShadingMethod.h"

ShadingMethod::ShadingMethod(
        const char* vertexShaderSubroutineName,
        const char* fragmentShaderSubroutineName) {
    this->vertexSubroutine.name = vertexShaderSubroutineName;
    this->fragmentSubroutine.name = fragmentShaderSubroutineName;
}

vector<ShadingMethod> ShadingMethod::methods{
    ShadingMethod("no_shading", "no_shading") //No Shading
    /*
    {"Gouraud (Ambient + Diffuse)", ShadingMethod("ambient_diffuse_shading", "no_shading")}
    {"Gouraud (Ambient + Diffuse + Specular)", ShadingMethod("ambient_diffuse_specular_shading", "no_shading")}
    {"Phong (Ambient + Diffuse + Specular)", ShadingMethod("no_shading", "ambient_diffuse_specular_shading")}
    */
};
