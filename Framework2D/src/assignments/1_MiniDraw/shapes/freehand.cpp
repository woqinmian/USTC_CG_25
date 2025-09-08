#include "freehand.h"

#include <imgui.h>

namespace USTC_CG{

void Freehand::draw(const Config &config) const{
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
                line_color[0],
                line_color[1],
                line_color[2],
                line_color[3]),
            line_thickness
        );
    }

    if(is_selected){
        Controlpoint::Config s = {.bias = {config.bias[0], config.bias[1]}};
        for(const auto &point : point_list_){
            point->draw(s);
        }
    }
}

void Freehand::update(float x, float y){
    end_point_x_ = x;
    end_point_y_ = y;
}

void Freehand::add_control_point(float x, float y){
    x_list_.push_back(x);
    y_list_.push_back(y);
}

void Freehand::initial_point(){
    min_x_ = x_list_[0];
    max_x_ = x_list_[0];
    min_y_ = y_list_[0];
    max_y_ = y_list_[0];
    for(int i = 0; i < x_list_.size(); i++){
        min_x_ = std::min(min_x_, x_list_[i]);
        max_x_ = std::max(max_x_, x_list_[i]);
        min_y_ = std::min(min_y_, y_list_[i]);
        max_y_ = std::max(max_y_, y_list_[i]);
    }
    center_x_ = (min_x_ + max_x_) / 2.0f;
    center_y_ = (min_y_ + max_y_) / 2.0f;

    current_point_ = std::make_shared<Controlpoint>(0, 0);
    point_list_.push_back(current_point_);
    current_point_.reset();
}

void Freehand::update_point(){
    std::shared_ptr<Controlpoint> updating;

    updating = point_list_[0];
    updating->update(center_x_, center_y_);
}

void Freehand::update_shape_by_center(float x, float y){
    float offset_x = x - center_x_;
    float offset_y = y - center_y_;

    for(int i = 0; i < x_list_.size(); i++){
        x_list_[i] += offset_x;
        y_list_[i] += offset_y;
    }

    min_x_ += offset_x;
    max_x_ += offset_x;
    min_y_ += offset_y;
    max_y_ += offset_y;
    center_x_ += offset_x;
    center_y_ += offset_y;
}

float Freehand::min_distance(float x, float y){
    float min = line2vertax_min_distance(x_list_[0], y_list_[0], x_list_[1], y_list_[1], x, y);
    int n = static_cast<int>(x_list_.size());
    for(int i = 1; i < n; i++){
        float temp_min = line2vertax_min_distance(x_list_[i], y_list_[i], x_list_[(i + 1) % n], y_list_[(i + 1) % n], x, y);
        min = std::min(min, temp_min);
    }

    return min;
}

}