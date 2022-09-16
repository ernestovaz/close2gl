#include "Menu.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"

#include "../renderer/Camera.h"
#include "../renderer/Renderer.h"


Menu::Menu(GLFWwindow* window) {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ItemSpacing = ImVec2(4,8);
    style.IndentSpacing = 100.0f;
    ImVec4 primaryColor = ImVec4(0.40, 0.26, 0.47, 1.0);
    ImVec4 secondaryColor = ImVec4(0.25, 0.21, 0.34, 1.0);
    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.43f, 0.42f, 0.47f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.43f, 0.28f, 0.56f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.00f, 0.00f, 0.00f, 0.67f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.23f, 0.21f, 0.27f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.23f, 0.21f, 0.27f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.43f, 0.42f, 0.47f, 0.54f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.94f, 0.76f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.58f, 0.44f, 0.61f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.29f, 0.43f, 0.60f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.47f, 0.27f, 0.52f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.43f, 0.42f, 0.47f, 0.54f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.47f, 0.27f, 0.52f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.62f, 0.26f, 0.71f, 0.40f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.50f, 0.31f, 0.50f, 1.00f);

    ImGui::SetNextWindowCollapsed(true);
    ImGui::SetNextWindowCollapsed(true);
}

void Menu::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::Begin("Menu", NULL, ImGuiWindowFlags_AlwaysAutoResize);                     

    ImGui::Text("Framerate: %7.1f FPS", ImGui::GetIO().Framerate);

    static int *currentAPI = reinterpret_cast<int*>(&Renderer::currentAPI);

    if(ImGui::CollapsingHeader("Rendering")){
        ImGui::Indent(10.0f);

        const char* APIs[] = {"OpenGL", "Close2GL" }; 
        ImGui::Combo("Rendering API", currentAPI, APIs, IM_ARRAYSIZE(APIs));


        /*
        if(*currentAPI == 1) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            Renderer::currentShadingMethod = 0;
        }
        */
        const char* openGLShadingMethods[] = { 
            "No Shading", 
            "Gouraud (Ambient + Diffuse)",
            "Gouraud (Ambient + Diffuse + Specular)",
            "Phong (Ambient + Diffuse + Specular)"
        };
        const char* close2GLshadingMethods[] = { 
            "No Shading", 
            "Gouraud (Ambient + Diffuse + Specular)",
        };
        ImGui::PushItemWidth(180);
        if(*currentAPI == 0) {
            ImGui::Combo("Shading Method", &Renderer::currentShadingMethod, 
                    openGLShadingMethods, IM_ARRAYSIZE(openGLShadingMethods));
        } else {
            if(Renderer::currentShadingMethod > 1) Renderer::currentShadingMethod = 1;
            ImGui::Combo("Shading Method", &Renderer::currentShadingMethod, 
                    close2GLshadingMethods, IM_ARRAYSIZE(close2GLshadingMethods));
        }

        ImGui::PushItemWidth(180);
        ImGui::ColorEdit3("Model Color", glm::value_ptr(Renderer::renderingColor));

        ImGui::PushItemWidth(90);
        const char* renderingPrimitives[] = { "Points", "Lines", "Triangles" };
        static int *currentPrimitive = reinterpret_cast<int*>(
                &Renderer::renderingPrimitive);
        ImGui::Combo("Rendering Primitive", currentPrimitive, 
                renderingPrimitives, IM_ARRAYSIZE(renderingPrimitives));

        ImGui::PushItemWidth(45);
        static int* currentFaceOrientation = &Renderer::reverseFaceOrientation;
        ImGui::Combo("Triangle Face Orientation", currentFaceOrientation, "CCW\0CW\0");

        ImGui::PushItemWidth(60);
        ImGui::DragFloat("Near Plane", &Renderer::nearPlane, 0.05);
        ImGui::SameLine(160);
        ImGui::PushItemWidth(60);
        ImGui::DragFloat("Far Plane", &Renderer::farPlane, 0.05);

        ImGui::Checkbox("Backface Culling", &Renderer::cullingEnabled);

        if(!Model::hasTextureCoordinates) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }
        ImGui::SameLine(160);
        ImGui::Checkbox("Texture", &Renderer::shouldUseTexture);
        if(!Model::hasTextureCoordinates) {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        if(!Renderer::shouldUseTexture) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }
        const char* textureFilteringMethods[] = { 
            "Nearest Neighbour", 
            "Bilinear",
            "Trilinear",
        };
        ImGui::PushItemWidth(180);
        static int *currentTextureFiltering = reinterpret_cast<int*>(&Renderer::textureFiltering);
        ImGui::Combo("Texture Filtering", currentTextureFiltering, 
                textureFilteringMethods, IM_ARRAYSIZE(textureFilteringMethods));
        if(!Renderer::shouldUseTexture) {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        ImGui::Unindent(10.0f);
    }

    if(ImGui::CollapsingHeader("Camera")){
        ImGui::Indent(10.0f);
        ImGui::PushItemWidth(150);
        ImGui::SliderFloat("Vertical field Of View", &Renderer::verticalFieldOfView, 1.0f, 179.0f);
        if(*currentAPI == 1) {
            ImGui::Checkbox("Asymmetric Field Of View", &Renderer::fieldOfViewIsAsymmetric);
            if(!Renderer::fieldOfViewIsAsymmetric) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }
            ImGui::SliderFloat("Horizontal Field Of View", &Renderer::horizontalFieldOfView, 1.0f, 179.0f);
            if(!Renderer::fieldOfViewIsAsymmetric) {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }
        }

        if(ImGui::ArrowButton("left", 0)) Camera::moveLeft();
        ImGui::SameLine();
        if(ImGui::ArrowButton("front", 2)) Camera::moveFront();
        ImGui::SameLine();
        if(ImGui::ArrowButton("back", 3)) Camera::moveBack();
        ImGui::SameLine();
        if(ImGui::ArrowButton("right", 1)) Camera::moveRight();
        ImGui::SameLine();
        if(ImGui::Button("Up")) Camera::moveUp();
        ImGui::SameLine();
        if(ImGui::Button("Down")) Camera::moveDown();
        ImGui::SameLine();
        ImGui::Text("Move Camera");

        ImGui::PushItemWidth(100);
        ImGui::InputFloat("Movement Units", &Camera::movementUnits, 0.5);
        static float cameraRoll = 0.0f;
        static float lastCameraRoll = cameraRoll;
        ImGui::SliderFloat("Camera Roll",&cameraRoll,-360.0f,360.0f);
        if(lastCameraRoll != cameraRoll) {
            Camera::setRoll(cameraRoll);
            lastCameraRoll = cameraRoll;
        }

        static bool cameraIsCentered = false;
        if(ImGui::Checkbox("Center Camera", &cameraIsCentered)) {
            Camera::center();
        }
        if(ImGui::Button("Reset Camera Position")) {
            Camera::reset();
        }

        ImGui::Unindent(10.0f);
    }
    if (ImGui::Button("Quit")) quitButtonClicked = true;

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Menu::~Menu() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool Menu::isHandlingMouse() {
    return ImGui::GetIO().WantCaptureMouse;
}

