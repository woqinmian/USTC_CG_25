#include "controlpoint.h"

#include <imgui.h>

namespace USTC_CG{

void Controlpoint::draw(const Config &config) const{
    ImDrawList *drwa_list = ImGui::GetWindowDrawList();

    drwa_list->AddCircleFilled(
        ImVec2(config.bias[0] + x_, config.bias[1] + y_),
        8.0f,
        IM_COL32(
            config.line_color[0],
            config.line_color[1],
            config.line_color[2],
            config.line_color[3]),
        ImDrawFlags_None);

    return;
}

void Controlpoint::update(float x, float y){
    x_ = x;
    y_ = y;
}

}