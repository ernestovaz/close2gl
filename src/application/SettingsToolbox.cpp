#include "SettingsToolbox.h"

#include "../renderer/Settings.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

SettingsToolbox::SettingsToolbox(GLFWwindow* window, Camera* camera) {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
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
    colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.40f, 0.56f, 0.31f);
    colors[ImGuiCol_Button]                 = ImVec4(0.62f, 0.26f, 0.71f, 0.40f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.50f, 0.31f, 0.50f, 1.00f);


}

void SettingsToolbox::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("Settings Toolbox");                     

    ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
    ImGui::ColorEdit3("Model Color", glm::value_ptr(Settings::renderingColor));
    if (ImGui::Button("Quit")) quitButtonClicked = true;

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

SettingsToolbox::~SettingsToolbox() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool SettingsToolbox::isHandlingMouse() {
    return ImGui::GetIO().WantCaptureMouse;
}
