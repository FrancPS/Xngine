#include "Globals.h"
#include "Application.h"
#include "WindowConfig.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "MathGeoLib.h"

#include "GL/glew.h"
#include "GL/wglew.h"
// TODO: Textures is a class with only a static function.
// It doesnt initialise DevIL at start, but when a load is requested!
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"


WindowConfig::WindowConfig() {
    // TODO: Should not go here!
    vIL     = ilGetInteger(IL_VERSION_NUM);
    vILU    = ilGetInteger(ILU_VERSION_NUM);
    vILUT   = ilGetInteger(ILUT_VERSION_NUM);
}


bool WindowConfig::Draw() {

    ImGui::Begin("CONFIGURATION");

    // ----- RESOURCE USAGE ----- //
        // Update FPS buffer
    fpsNow = ImGui::GetIO().Framerate;
    fps.push_back(fpsNow);

    char title[55];
    sprintf_s(title, 55, "Application average %.3f ms/frame (%.1f FPS)", 1000.0 / fpsNow, fpsNow);

        // Remove the 1st element when we have more than 'histNumElements' values in the FPS vector
    if (fps.size() > histNumElements)
        fps.erase(fps.begin());

        // Plot Hist
    ImGui::PlotHistogram("##Histogram", &fps[0], histNumElements, 0, title, 0.0f, 2000, ImVec2(330, 50));
    
        // Memory soncumption
    /*GLint nTotalMemoryInKB = 0;
    glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &nTotalMemoryInKB);
    ImGui::Text("%d Mb", nTotalMemoryInKB);
    GLint nCurAvailMemoryInKB = 0;
    glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &nCurAvailMemoryInKB);
    ImGui::Text("%d Mb", nCurAvailMemoryInKB);*/

    /*forAMD?*GLuint uNoOfGPUs = wglGetGPUIDsAMD(0, 0);
    GLuint* uGPUIDs = new GLuint[uNoOfGPUs];
    wglGetGPUIDsAMD(uNoOfGPUs, uGPUIDs);

    GLuint uTotalMemoryInMBAMD = 0;
    wglGetGPUInfoAMD(uGPUIDs[0], WGL_GPU_RAM_AMD, GL_UNSIGNED_INT, sizeof(GLuint), &uTotalMemoryInMBAMD);

    GLint nCurAvailMemoryInKBAMD = 0;
    glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, &nCurAvailMemoryInKBAMD);

    ImGui::Text("%d Mb", uTotalMemoryInMBAMD);
    ImGui::Text("%d Mb", nCurAvailMemoryInKBAMD);**/


    // ----- WINDOW CONFIG ----- //
    if (ImGui::CollapsingHeader("Window Configuration"))
    {
        if (ImGui::Button("Toggle Fullscreen"))
            App->window->ToggleFullscreen();
        if (ImGui::Button("Toggle Borderless"))
            App->window->ToggleBorderless();
    }
    
    // ----- RENDER CONFIG ----- //
    if (ImGui::CollapsingHeader("Renderer Configuration")) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Select Background Color:");
        ImGui::ColorPicker4("MyColor##1", (float*)&App->renderer->backgroundColor, NULL, false);

        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Select Grid Color:");
        ImGui::ColorPicker4("MyColor##2", (float*)&App->renderer->gridColor, NULL, false);
    }

    // ----- CAMERA CONFIG ----- //
    if (ImGui::CollapsingHeader("Camera Configuration"))
    {
        // Get Camera Values
        vec cameraPos = math::float3(App->camera->frustum.Pos());

        static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Position:");
        ImGui::SameLine(); ImGui::DragFloat("x", &cameraPos.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.2f", flags);
        ImGui::SameLine(); ImGui::DragFloat("y", &cameraPos.y, 0.005f, -FLT_MAX, +FLT_MAX, "%.2f", flags);
        ImGui::SameLine(); ImGui::DragFloat("z", &cameraPos.z, 0.005f, -FLT_MAX, +FLT_MAX, "%.2f", flags);

        App->camera->frustum.SetPos(cameraPos);
    }

    // ----- HARDWARE INFO ----- //
    if (ImGui::CollapsingHeader("Hardware Information"))
    {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "CPU :");
        ImGui::BulletText("Number of Cores: %d", App->cpuCores);
        ImGui::BulletText("L1 Cache size: %d Bytes", App->cpuCacheSize);
        std::string caps = "";
        ImGui::Bullet(); ImGui::TextWrapped("Capacitors: %s", PrintCapacitors(caps));
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "RAM :");
        ImGui::BulletText("System RAM: %.1f MB", App->systemRAM/(float)1024);
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "GPU :");
        ImGui::BulletText("%s", App->gpu);
    }

    // ----- SOFTWARE INFO ----- //
    if (ImGui::CollapsingHeader("Software Information"))
    {
        ImGui::TextUnformatted("Compiled using: ");
        ImGui::BulletText("SDL %s", App->vSDL);
        ImGui::BulletText("OpenGL %s", App->vOpenGL);
        ImGui::BulletText("Glew %s", App->vGlew);
        ImGui::BulletText("DevIL: IL %d, ILU %d, ILUT %d", vIL, vILU, vILUT);
    }

    ImGui::End();

    return true;
}




const char* WindowConfig::PrintCapacitors(std::string &caps) {
    if (App->hasAVX) {
        caps.append(std::string("AVX"));
    }
   if (App->hasAVX2) {
        caps.append(std::string(", AVX2"));
    }
    if (App->hasAltiVec) {
        caps.append(std::string(", AltiVec"));
    }
    if (App->hasMMX) {
        caps.append(std::string(", MMX"));
    }
    if (App->hasRDTSC) {
        caps.append(std::string(", RDTSC"));
    }
    if (App->hasSSE) {
        caps.append(std::string(", SSE"));
    }
    if (App->hasSSE2) {
        caps.append(std::string(", SSE2"));
    }
    if (App->hasSSE3) {
        caps.append(std::string(", SSE3"));
    }
    if (App->hasSSE41) {
        caps.append(std::string(", SSE41"));
    }
    if (App->hasSSE42) {
        caps.append(std::string(", SSE42"));
    }

    return caps.c_str();
}


