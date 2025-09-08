#include "ellipse.h"

#include <imgui.h>

namespace USTC_CG{

void Ellipse::draw(const Config &config) const {
    ImDrawList *drwa_list = ImGui::GetWindowDrawList();

    drwa_list->AddEllipse(
        ImVec2(config.bias[0] + center_x_, config.bias[1] + center_y_),
        ImVec2(a_, b_),
        IM_COL32(line_color[0], line_color[1], line_color[2], line_color[3]),
        theta_,
        ImDrawFlags_None,
        line_thickness
    );

    if(is_selected){
        Controlpoint::Config s = {.bias = {config.bias[0], config.bias[1]}};
        for(const auto &point : point_list_){
            point->draw(s);
        }
    }
}

void Ellipse::update(float x, float y){
    point_x[1] = x;
    point_x[2] = x;
    point_y[2] = y;
    point_y[3] = y;

    center_x_ = (point_x[0] + point_x[2]) / 2;
    center_y_ = (point_y[0] + point_y[2]) / 2;

    float ax = (point_x[1] + point_x[2]) / 2.0f;
    float ay = (point_y[1] + point_y[2]) / 2.0f;
    float bx = (point_x[1] + point_x[0]) / 2.0f;
    float by = (point_y[1] + point_y[0]) / 2.0f;
    a_ = std::sqrt((ax - center_x_) * (ax - center_x_) + (ay - center_y_) * (ay - center_y_));
    b_ = std::sqrt((bx - center_x_) * (bx - center_x_) + (by - center_y_) * (by - center_y_));

    float dx1 = 10.0f;
    float dy1 = 0.0f;
    float dx2 = point_x[1] - point_x[0];
    float dy2 = point_y[1] - point_y[0];
    float cross = dx1 * dy2 - dx2 * dy1;
    float dot = dx1 * dx2 + dy1 * dy2;
    theta_ = std::atan2(cross, dot);
}

void Ellipse::initial_point(){
    current_point_ = std::make_shared<Controlpoint>(0, 0);
    point_list_.push_back(current_point_);

    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::rotate);
    point_list_.push_back(current_point_);

    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::scale);
    point_list_.push_back(current_point_);

    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::vertax, 0);
    point_list_.push_back(current_point_);
    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::vertax, 1);
    point_list_.push_back(current_point_);
    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::vertax, 2);
    point_list_.push_back(current_point_);
    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::vertax, 3);
    point_list_.push_back(current_point_);

    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::edge, 0);
    point_list_.push_back(current_point_);
    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::edge, 1);
    point_list_.push_back(current_point_);
    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::edge, 2);
    point_list_.push_back(current_point_);
    current_point_ = std::make_shared<Controlpoint>(0, 0, Controlpoint::pointType::edge, 3);
    point_list_.push_back(current_point_);

    current_point_.reset();
}

void Ellipse::update_point(){
    center_x_ = (point_x[0] + point_x[2]) / 2;
    center_y_ = (point_y[0] + point_y[2]) / 2;

    float ax = (point_x[1] + point_x[2]) / 2.0f;
    float ay = (point_y[1] + point_y[2]) / 2.0f;
    float bx = (point_x[1] + point_x[0]) / 2.0f;
    float by = (point_y[1] + point_y[0]) / 2.0f;
    a_ = std::sqrt((ax - center_x_) * (ax - center_x_) + (ay - center_y_) * (ay - center_y_));
    b_ = std::sqrt((bx - center_x_) * (bx - center_x_) + (by - center_y_) * (by - center_y_));

    float dx1 = 10.0f;
    float dy1 = 0.0f;
    float dx2 = point_x[1] - point_x[0];
    float dy2 = point_y[1] - point_y[0];
    float cross = dx1 * dy2 - dx2 * dy1;
    float dot = dx1 * dx2 + dy1 * dy2;
    theta_ = std::atan2(cross, dot);

    float rotate_point_x = ((point_x[1] + point_x[2]) / 2.0f + center_x_) / 2.0f;
    float rotate_point_y = ((point_y[1] + point_y[2]) / 2.0f + center_y_) / 2.0f;
    float scale_point_x = (center_x_ + point_x[2]) / 2.0f;
    float scale_point_y = (center_y_ + point_y[2]) / 2.0f;

    std::shared_ptr<Controlpoint> updating;

    updating = point_list_[0];
    updating->update(center_x_, center_y_);

    updating = point_list_[1];
    updating->update(rotate_point_x, rotate_point_y);

    updating = point_list_[2];
    updating->update(scale_point_x, scale_point_y);

    updating = point_list_[3];
    updating->update(point_x[0], point_y[0]);
    updating = point_list_[4];
    updating->update(point_x[1], point_y[1]);
    updating = point_list_[5];
    updating->update(point_x[2], point_y[2]);
    updating = point_list_[6];
    updating->update(point_x[3], point_y[3]);

    updating = point_list_[7];
    updating->update((point_x[0] + point_x[1]) / 2.0f, (point_y[0] + point_y[1]) / 2.0f);
    updating = point_list_[8];
    updating->update((point_x[1] + point_x[2]) / 2.0f, (point_y[1] + point_y[2]) / 2.0f);
    updating = point_list_[9];
    updating->update((point_x[2] + point_x[3]) / 2.0f, (point_y[2] + point_y[3]) / 2.0f);
    updating = point_list_[10];
    updating->update((point_x[3] + point_x[0]) / 2.0f, (point_y[3] + point_y[0]) / 2.0f);
}

void Ellipse::update_shape_by_center(float x, float y){
    float offset_x = x - (point_x[0] + point_x[2]) / 2.0f;
    float offset_y = y - (point_y[0] + point_y[2]) / 2.0f;
    for(int i = 0; i < 4; i++){
        point_x[i] += offset_x;
        point_y[i] += offset_y;
    }
}

void Ellipse::update_shape_by_rotate(float x, float y){
    float center_point_x = (point_x[0] + point_x[2]) / 2.0f;
    float center_point_y = (point_y[0] + point_y[2]) / 2.0f;
    float rotate_point_x = ((point_x[1] + point_x[2]) / 2.0f + center_point_x) / 2.0f;
    float rotate_point_y = ((point_y[1] + point_y[2]) / 2.0f + center_point_y) / 2.0f;

    float dx1 = rotate_point_x - center_point_x;
    float dy1 = rotate_point_y - center_point_y;
    float dx2 = x - center_point_x;
    float dy2 = y - center_point_y;
    float cross = dx1 * dy2 - dx2 * dy1;
    float dot = dx1 * dx2 + dy1 * dy2;
    double theta = std::atan2(cross, dot);

    for(int i = 0; i < 4; i++){
        rotate_by_theta(point_x[i], point_y[i], center_point_x, center_point_y, theta);
    }
}

void Ellipse::update_shape_by_scale(float x, float y){
    float center_point_x = (point_x[0] + point_x[2]) / 2.0f;
    float center_point_y = (point_y[0] + point_y[2]) / 2.0f;
    float scale_point_x = (center_point_x + point_x[2]) / 2.0f;
    float scale_point_y = (center_point_y + point_y[2]) / 2.0f;

    float dx1 = scale_point_x - center_point_x;
    float dy1 = scale_point_y - center_point_y;
    float dx2 = 10.0f;
    float dy2 = 0.0f;
    float cross = dx1 * dy2 - dx2 * dy1;
    float dot = dx1 * dx2 + dy1 * dy2;
    double theta = std::atan2(cross, dot);
    rotate_by_theta(scale_point_x, scale_point_y, center_point_x, center_point_y, theta);
    rotate_by_theta(x, y, center_point_x, center_point_y, theta);
    float times = (x - center_point_x) / (scale_point_x - center_point_x);
    
    for(int i = 0; i < 4; i++){
        scale_by_times(point_x[i], point_y[i], center_point_x, center_point_y, times);
    }
}

void Ellipse::update_shape_by_vertax(float x, float y){
    int order = current_point_->order_;
    float x1 = point_x[order];
    float y1 = point_y[order];
    float x2 = point_x[(order + 1) % 4];
    float y2 = point_y[(order + 1) % 4];

    point_x[order] = x;
    point_y[order] = y;
    if(std::abs(x1 - x2) < 1e-6){
        point_y[(order - 1 + 4) % 4] = y;
        point_x[(order + 1) % 4] = x;
        return;
    }
    if(std::abs(y1 - y2) < 1e-6){
        point_x[(order - 1 + 4) % 4] = x;
        point_y[(order + 1) % 4] = y;
        return;
    }

    float dx = x1 - x2;
    float dy = y1 - y2;
    float tempx = x1 * dy * dy + x * dx * dx - (y1 - y) * dx * dy;
    tempx = tempx / (dx * dx + dy * dy);
    float tempy = dy * (tempx - x1) / dx + y1;

    point_x[(order - 1 + 4) % 4] += tempx - x1;
    point_y[(order - 1 + 4) % 4] += tempy - y1;
    point_x[(order + 1) % 4] += x - tempx;
    point_y[(order + 1) % 4] += y - tempy;
}

void Ellipse::update_shape_by_edge(float x, float y){
    float center_point_x = (point_x[0] + point_x[2]) / 2.0f;
    float center_point_y = (point_y[0] + point_y[2]) / 2.0f;

    int order = current_point_->order_;
    float x1 = (point_x[order] + point_x[(order + 1) % 4]) / 2.0f;
    float y1 = (point_y[order] + point_y[(order + 1) % 4]) / 2.0f;

    float dx1 = x1 - center_point_x;
    float dy1 = y1 - center_point_y;
    float dx2 = 10.0f;
    float dy2 = 0.0f;
    float cross = dx1 * dy2 - dx2 * dy1;
    float dot = dx1 * dx2 + dy1 * dy2;
    double theta = std::atan2(cross, dot);
    rotate_by_theta(x1, y1, center_point_x, center_point_y, theta);
    rotate_by_theta(x, y, center_point_x, center_point_y, theta);
    float times = (x - center_point_x) / (x1 - center_point_x);

    float cx1 = (point_x[(order - 1 + 4) % 4] + point_x[order]) / 2.0f;
    float cy1 = (point_y[(order - 1 + 4) % 4] + point_y[order]) / 2.0f;
    float cx2 = (point_x[(order + 1) % 4] + point_x[(order + 2) % 4]) / 2.0f;
    float cy2 = (point_y[(order + 1) % 4] + point_y[(order + 2) % 4]) / 2.0f;

    scale_by_times(point_x[order], point_y[order], cx1, cy1, times);
    scale_by_times(point_x[(order + 1) % 4], point_y[(order + 1) % 4], cx2, cy2, times);
}

float Ellipse::min_distance(float x, float y){
    float temp_x = x - center_x_;
    float temp_y = y - center_y_;

    rotate_by_theta(temp_x, temp_y, 0, 0, -theta_);

    float a1 = std::max(a_ - 10.0f, 1.0f);
    float a2 = a_ + 10.0f;
    float b1 = std::max(b_ - 10.0f, 1.0f);
    float b2 = b_ + 10.0f;

    if(temp_x * temp_x / (a1 * a1) + temp_y * temp_y / (b1 * b1) > 1.0f){
        if(temp_x * temp_x / (a2 * a2) + temp_y * temp_y / (b2 * b2) < 1.0f){
            return 0.0f;
        }
    }

    return 100.0f;
}

}