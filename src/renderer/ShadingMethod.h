#pragma once

#include <vector>

using std::vector;

struct ShaderSubroutine {
    const char* name;
    unsigned int index;
};

class ShadingMethod {
public:
    ShadingMethod(
            const char* vertexSubroutineName, 
            const char* fragmentSubroutineName
    );
    struct ShaderSubroutine vertexSubroutine;
    struct ShaderSubroutine fragmentSubroutine;

    static vector<ShadingMethod> methods;
};
