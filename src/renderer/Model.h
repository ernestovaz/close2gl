#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

using std::string;
using std::vector;
using std::ostream;

using glm::vec2;
using glm::vec3;

class Model {
public:
    static vector<vec3>         positions;
    static vector<vec3>         normals;
    static vector<vec2>         textureCoords;
    static vector<unsigned int> indices;

    static vec3 center;
    static float width;
    static float length;
    
    static bool hasTextureCoordinates;

    static void loadFromFile(string);
};

ostream& operator<<(ostream&, const Model&);
