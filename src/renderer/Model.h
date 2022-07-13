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

    vec3 boundingBoxCenter;
    float boundingBoxLargestSide;

    Model();
    Model(string);
    vec3 getCenter();
    float getBoundingBoxSide();

private:
    void loadFromFile(string);
    void calculateBoundingBox();

};

ostream& operator<<(ostream&, const Model&);
