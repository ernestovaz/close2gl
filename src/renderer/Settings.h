#pragma once

#include <glm/vec3.hpp>

using glm::vec3;

enum class RenderingPrimitive { POINTS, LINES, TRIANGLES };

#define DEFAULT_RENDERING_PRIMITIVE         RenderingPrimitive::TRIANGLES
#define DEFAULT_RENDERING_COLOR             vec3(0.89f, 0.427f, 0.819f)
#define DEFAULT_BACKGROUND_COLOR            vec3(0.43f, 0.89f, 0.78f)
#define DEFAULT_FIELD_OF_VIEW               45
#define DEFAULT_NEAR_PLANE                  0.01f
#define DEFAULT_FAR_PLANE                   200.0f
#define DEFAULT_FACE_ORIENTATION_REVERSE    false
#define DEFAULT_CULLING                     false

class Settings {
public:
    static RenderingPrimitive renderingPrimitive;
    static vec3 renderingColor;
    static vec3 backgroundColor;
    static float fieldOfView;
    static float nearPlane;
    static float farPlane;
    static bool reverseFaceOrientation;
    static bool cullingEnabled;
};
