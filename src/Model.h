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
    vector<vec3>         positions;
    vector<vec3>         normals;
    vector<unsigned int> indices;

    Model();
    Model(string);
    void loadFromFile(string);

private:


};

ostream& operator<<(ostream&, const Model&);
