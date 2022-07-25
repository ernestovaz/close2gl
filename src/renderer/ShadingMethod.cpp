#include "ShadingMethod.h"

ShadingMethod::ShadingMethod(
        const char* vertexShaderSubroutineName,
        const char* fragmentShaderSubroutineName) {
    this->vertexSubroutine.name = vertexShaderSubroutineName;
    this->fragmentSubroutine.name = fragmentShaderSubroutineName;
}

vector<ShadingMethod> ShadingMethod::methods{
    ShadingMethod("no_shading", "no_shading"),      //No Shading
    ShadingMethod("ambient_diffuse_shading", "use_vertex_color"),    //Gouraud AD 
    ShadingMethod("ambient_diffuse_specular_shading", "use_vertex_color")    //Gouraud ADS
};
