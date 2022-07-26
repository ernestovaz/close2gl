#include "Model.h"

#include <iostream>
#include <cstdlib>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define AI_CONFIG_PP_RVC_FLAGS aiComponent_NORMALS

using std::exit;
using std::cerr;
using std::endl;
using std::fmax;

vector<vec3>            Model::positions;
vector<vec3>            Model::normals;
vector<unsigned int>    Model::indices;

vec3    Model::center;
float   Model::width;
float   Model::length;

void Model::loadFromFile(string filepath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
            filepath, 
            aiProcess_Triangulate | aiProcess_RemoveComponent | aiProcess_GenSmoothNormals);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        cerr << "ERROR LOADING MODEL: " << importer.GetErrorString() << endl;
        exit(EXIT_FAILURE);
    }

    // used for bounding box calculation
    float top, bottom, right, left, begin, end;
    if(scene->mMeshes[0] && scene->mMeshes[0]->mNumVertices) {
        vec3 firstVertex;
        firstVertex.x = scene->mMeshes[0]->mVertices[0].x;
        firstVertex.y = scene->mMeshes[0]->mVertices[0].y;
        firstVertex.z = scene->mMeshes[0]->mVertices[0].z;
        top = firstVertex.y; 
        bottom = firstVertex.y; 
        right = firstVertex.x; 
        left = firstVertex.x; 
        begin = firstVertex.z;
        end = firstVertex.z;
    }

    for(unsigned int i=0; i<scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        // go over each face and store indices
        for(unsigned int j=0; j<mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];
            for(unsigned int k=0; k<face.mNumIndices; k++) {
                indices.push_back(face.mIndices[k]);
            } 
        }

        // go over each vertex and store positions and normals
        for(unsigned int j=0; j<mesh->mNumVertices; j++) {
            vec3 position;
            position.x = mesh->mVertices[j].x;
            position.y = mesh->mVertices[j].y;
            position.z = mesh->mVertices[j].z;
            positions.push_back(position);

            if(mesh->mNormals) {
                vec3 normal;
                normal.x = mesh->mNormals[j].x;
                normal.y = mesh->mNormals[j].y;
                normal.z = mesh->mNormals[j].z;
                normals.push_back(normal);
            }

            // bounding box calculations
            if(position.y > top) top = position.y;
            if(position.y < bottom) bottom = position.y;
            if(position.x > right) right = position.x;
            if(position.x < left) left = position.x;
            if(position.z > end) end = position.z;
            if(position.z < begin) begin = position.z;
        }
    }

    center.y = (right + left)/2.0f;
    center.x = (top + bottom)/2.0f;
    center.z = (end + begin)/2.0f;
    width   = fmax(fabs(right - left), fabs(top - bottom));
    length  = fabs(begin - center.z);
}

ostream& operator<<(ostream& str, const Model& model){
    str << "Vertex positions: " << model.positions.size() << endl;
    str << "Indices: " << model.indices.size() << endl;
    str << "Vertices: " << endl;
    for(auto i: model.positions) {
        str << i.x << " " << i.y << " " << i.z << endl;
    }
    str << "Indices: " << endl;
    for(auto i: model.indices) {
        str << i << " "; 
    }
    str << endl;
    return str;
}
