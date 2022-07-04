#include "Model.h"

#include <iostream>
#include <cstdlib>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using std::exit;
using std::cerr;
using std::endl;

// Default model falls back to a single triangle
Model::Model(){
    indices = {0, 1, 2}; 
    positions = {
        vec3(-0.5f, -0.5f, 0.0f),
        vec3( 0.5f, -0.5f, 0.0f),
        vec3( 0.0f,  0.5f, 0.0f)
    };
}

Model::Model(string filepath) {
    loadFromFile(filepath);
}

void Model::loadFromFile(string filepath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
            filepath, 
            aiProcess_Triangulate);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        cerr << "ERROR LOADING MODEL: " << importer.GetErrorString() << endl;
        exit(EXIT_FAILURE);
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
                position.x = mesh->mNormals[j].x;
                position.y = mesh->mNormals[j].y;
                position.z = mesh->mNormals[j].z;
                normals.push_back(normal);
            }
        }
    }

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
