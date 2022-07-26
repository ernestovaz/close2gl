#include "Settings.h"

float Settings::verticalFieldOfView             = DEFAULT_FIELD_OF_VIEW;
float Settings::horizontalFieldOfView           = DEFAULT_FIELD_OF_VIEW;
bool Settings::fieldOfViewIsAsymmetric          = false;
float Settings::farPlane                        = DEFAULT_FAR_PLANE;
float Settings::nearPlane                       = DEFAULT_NEAR_PLANE;
int Settings::reverseFaceOrientation            = DEFAULT_FACE_ORIENTATION_REVERSE;
bool Settings::cullingEnabled                   = DEFAULT_CULLING;
vec3 Settings::renderingColor                   = DEFAULT_RENDERING_COLOR;
RenderingPrimitive Settings::renderingPrimitive = DEFAULT_RENDERING_PRIMITIVE;
