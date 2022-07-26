#include "ShadingMethod.h"

ShadingMethod::ShadingMethod(
        const char* vertexShaderSubroutineName,
        const char* fragmentShaderSubroutineName) {
    this->vertexSubroutine.name = vertexShaderSubroutineName;
    this->fragmentSubroutine.name = fragmentShaderSubroutineName;
}

vector<ShadingMethod> ShadingMethod::methods{
    //No Shading
    ShadingMethod("no_shading", "no_shading"),      

    //Gouraud AD 
    ShadingMethod("ambient_diffuse_shading", "use_vertex_color"),   

    //Gouraud ADS
    ShadingMethod("ambient_diffuse_specular_shading", "use_vertex_color"),

    //Phong ADS
    ShadingMethod("no_shading", "ambient_diffuse_specular_shading")
};
