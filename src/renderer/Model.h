#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <glm/vec3.hpp>

using std::string;
using std::vector;
using std::ostream;

using glm::vec3;

class Model {
public:
    static vector<vec3>         positions;
    static vector<vec3>         normals;
    static vector<unsigned int> indices;

    static vec3 center;
    static float width;
    static float length;

    static void loadFromFile(string);
};

ostream& operator<<(ostream&, const Model&);
