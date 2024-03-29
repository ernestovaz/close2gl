#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "buffer/ColorBuffer.h"
#include "buffer/DepthBuffer.h"
#include "texture/MipmapLevel.h"
#include "rasterization/Rasterizer.h"

using std::vector;
using glm::vec2;
using glm::vec3;
using glm::mat4;

class Close2GL {
public:
    enum Primitive{POINTS, LINES, TRIANGLES};
    enum Shading{NO_SHADING, GOURAUD};
    enum Orientation{CW, CCW};
    enum TextureFilter{NEAREST, BILINEAR, TRILINEAR};

    //options
    static Primitive primitive;
    static Shading shading;
    static Orientation frontFace;
    static TextureFilter filter;
    static vec3 color;

    static unsigned char* texture;
    static int textureWidth;
    static int textureHeight;

    static bool useTexture;
    static bool useHorizontalFOV;
    static bool performBackfaceCulling;

    //view
    static vec3 cameraPosition;
    static vec3 cameraDirection;
    static vec3 cameraUp;

    //projection
    static float verticalFOV;
    static float horizontalFOV;
    static float near;
    static float far;

    static float calculateHorizontalFOV();
    static void generateMipmap();
    static void clear(vec3 color);
    static void updateViewport(int left, int top, int right, int bottom);
    static void draw(vector<unsigned int> ids, vector<vec3> pos, vector<vec3> norms, vector<vec2> uvs);
    static unsigned char* getRenderedData();
    
private:
    static ColorBuffer colorBuffer;
    static DepthBuffer depthBuffer;
    static vector<MipmapLevel> mipmap;
    
    static mat4 model;
    static mat4 view;
    static mat4 projection;
    static mat4 viewport;
    
    static float width;
    static float height;
    
    static vector<float> WValues;
    
    static vector<vec3> transform(vector<vec3> array, mat4 transformation, bool perspectiveDivide);
    static vector<unsigned int> viewFrustumCull(vector<unsigned int> ids, vector<vec3> pos);
    static vector<unsigned int> backfaceCull(vector<unsigned int> ids, vector<vec3> pos);
    static void updateView();
    static void updateProjection();
    static void drawNoShading(vector<unsigned int> ids, vector<vec3> pos, Rasterizer* rasterizer);
    static void drawNoShading(vector<unsigned int> ids, vector<vec3> pos, vector<vec2> uvs, Rasterizer* rasterizer);
    static void drawGouraud(vector<unsigned int> ids, vector<vec3> pos, vector<vec3> worldPos, vector<vec3> norms, Rasterizer* rasterizer);
    static void drawGouraud(vector<unsigned int> ids, vector<vec3> pos, vector<vec3> worldPos, vector<vec3> norms, vector<vec2> uvs, Rasterizer* rasterizer);
};