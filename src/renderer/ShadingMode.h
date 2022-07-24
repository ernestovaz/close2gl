#pragma once

#include <string>

class ShadingMode {
public:
    ShadingMode(const char*);
    const char* name;
    unsigned int uniformID;
};
