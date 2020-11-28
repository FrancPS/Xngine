#include "Application.h"
#include "WindowProperties.h"
#include "ModuleRender.h"// for bakerhouse
#include "ModuleTexture.h"
#include <vector>

WindowProperties::WindowProperties() {
}

bool WindowProperties::Draw() {

    static ResourceModel* model = App->renderer->bakerhouse; //this is hardcoded

    ImGui::Begin("PROPERTIES");

    /*properties window with three sections: transformation, geometry and texture. All should give
read-only information about the current loaded meshes and texture (triangle count, texture size).*/
    if (ImGui::CollapsingHeader("Transformation")) {
        ImGui::TextUnformatted("It's always [0,0,0]");
    }
    if (ImGui::CollapsingHeader("Geometry")) {
        float modelWidth     = App->renderer->bakerhouse->sizeX;
        float modelHeight    = App->renderer->bakerhouse->sizeY;    // why if these are static the size doesnt change?
        float modelDepth     = App->renderer->bakerhouse->sizeZ;
        ImGui::Text("Model BB size:\nWidth: %.2f, Height: %.2f, Depth: %.2f", modelWidth, modelHeight, modelDepth);
        for (unsigned int i = 0; i < model->GetNumMeshes(); ++i) {
            ImGui::Text("Mesh %d Num Vertices: %d", i, model->GetMeshes()[i]->numVertex);
        }
    }
    if (ImGui::CollapsingHeader("Texture")) {
        static unsigned int textureWidth = App->textures->texWidth;
        static unsigned int textureHeight = App->textures->texHeight;
        static ImTextureID texturePreview = (void*)(intptr_t)App->textures->textureID;
        static unsigned int texPrevWidth = 256;
        static unsigned int texPrevHeight = 256;

        ImGui::Text("Width: %d, Height: %d", textureWidth, textureHeight);
        //ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
        ImGui::Image(texturePreview, ImVec2((float)texPrevWidth, (float)texPrevHeight), uv_min, uv_max, tint_col, border_col);
    }


    ImGui::End();

    return true;
}