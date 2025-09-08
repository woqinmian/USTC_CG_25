#include "polygon.h"

#include <imgui.h>
#include <iostream>

namespace USTC_CG{

void Polygon::draw(const Config &config) const {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    for(int i = 0; i < x_list_.size() - 1; i++){
        float head_point_x = x_list_[i];
        float head_point_y = y_list_[i];
        float tail_point_x = x_list_[i + 1];
        float tail_point_y = y_list_[i + 1];

        draw_list->AddLine(
            ImVec2(config.bias[0] + head_point_x, config.bias[1] + head_point_y),
            ImVec2(config.bias[0] + tail_point_x, config.bias[1] + tail_point_y),
            IM_COL32(
                config.line_color[0],
                config.line_color[1],
                config.line_color[2],
                config.line_color[3]),
            config.line_thickness
        );
    }

    float tail_point_x, tail_point_y;
    if(is_sealed){
        tail_point_x = x_list_[0];
        tail_point_y = y_list_[0];
    }
    else{
        tail_point_x = end_point_x_;
        tail_point_y = end_point_y_;
    }

    draw_list->AddLine(
        ImVec2(config.bias[0] + x_list_[x_list_.size() - 1], config.bias[1] + y_list_[x_list_.size() - 1]),
        ImVec2(config.bias[0] + tail_point_x, config.bias[1] + tail_point_y),
        IM_COL32(
            config.line_color[0],
            config.line_color[1],
            config.line_color[2],
            config.line_color[3]),
        config.line_thickness
    );
}


void Polygon::update(float x, float y){
    end_point_x_ = x;
    end_point_y_ = y;
}

void Polygon::add_control_point(float x, float y){
    x_list_.push_back(x);
    y_list_.push_back(y);
}

void Polygon::set_sealed(){
    is_sealed = true;
}

}