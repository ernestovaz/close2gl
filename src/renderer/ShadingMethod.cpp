#include "ShadingMethod.h"

ShadingMethod::ShadingMethod(
        const char* name,
        const char* vertexShaderSubroutineName,
        const char* fragmentShaderSubroutineName) {
    this->name = name;
    this->vertexSubroutine.name = vertexShaderSubroutineName;
    this->fragmentSubroutine.name = fragmentShaderSubroutineName;
}

vector<ShadingMethod> ShadingMethod::methods{
    ShadingMethod("No Shading", "no_shading", "no_shading"),      
    ShadingMethod("Gouraud AD", "ambient_diffuse_shading", "use_vertex_color"),   
    ShadingMethod("Gouraud ADS", "ambient_diffuse_specular_shading", "use_vertex_color"),
    ShadingMethod("Phong ADS", "no_shading", "ambient_diffuse_specular_shading")
};
