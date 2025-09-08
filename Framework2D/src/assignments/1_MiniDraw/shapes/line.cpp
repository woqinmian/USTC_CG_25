#include "line.h"
#include "tools.h"

#include <imgui.h>

namespace USTC_CG
{

Line::Line(float start_point_x, float start_point_y, float end_point_x, float end_point_y, float color[], float thickness)
     : Shape(color, thickness){
    start_point_x_ = start_point_x;
    start_point_y_ = start_point_y;
    end_point_x_ = end_point_x;
    end_point_y_ = end_point_y;
    initial_point();
}

// Draw the line using ImGui
void Line::draw(const Config& config) const
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    draw_list->AddLine(
        ImVec2(
            config.bias[0] + start_point_x_, config.bias[1] + start_point_y_),
        ImVec2(config.bias[0] + end_point_x_, config.bias[1] + end_point_y_),
        IM_COL32(
            line_color[0],
            line_color[1],
            line_color[2],
            line_color[3]),
        line_thickness);
    
    if(is_selected){
        Controlpoint::Config s = {.bias = {config.bias[0], config.bias[1]}};
        for(const auto &point : point_list_){
            point->draw(s);
        }
    }
}

void Line::update(float x, float y)
{
    end_point_x_ = x;
    end_point_y_ = y;
}

float Line::min_distance(float x, float y){
    return line2vertax_min_distance(start_point_x_, start_point_y_, end_point_x_, end_point_y_, x, y);
}

void Line::initial_point(){
    current_point_ = std::make_shared<Controlpoint>(0, 0);
    point_list_.push_back(current_point_);

    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::vertax, 0);
    point_list_.push_back(current_point_);
    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::vertax, 1);
    point_list_.push_back(current_point_);

    current_point_.reset();
}

void Line::update_point(){
    float center_point_x = (start_point_x_ + end_point_x_) / 2.0f;
    float center_point_y = (start_point_y_ + end_point_y_) / 2.0f;

    std::shared_ptr<Controlpoint> updating;

    updating = point_list_[0];
    updating->update(center_point_x, center_point_y);

    updating = point_list_[1];
    updating->update(start_point_x_,  start_point_y_);
    updating = point_list_[2];
    updating->update(end_point_x_, end_point_y_);
}

void Line::update_shape_by_center(float x, float y){
    float offset_x = x - (start_point_x_ + end_point_x_) / 2.0f;
    float offset_y = y - (start_point_y_ + end_point_y_) / 2.0f;
    start_point_x_ += offset_x;
    start_point_y_ += offset_y;
    end_point_x_ += offset_x;
    end_point_y_ += offset_y;
}

void Line::update_shape_by_vertax(float x, float y){
    int order = current_point_->order_;
    if(order == 0){
        start_point_x_ = x;
        start_point_y_ = y;
    }
    else {
        end_point_x_ = x;
        end_point_y_ = y;
    }
}



}  // namespace USTC_CG-