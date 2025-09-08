#include "freehand.h"

#include <imgui.h>
#include <algorithm>
#include <iostream>

namespace USTC_CG{

void Freehand::draw(const Config &config) const{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    for(int i = 0; i < x_list_.size(); i++){
        float head_point_x = x_list_[i];
        float head_point_y = y_list_[i];
        float tail_point_x = x_list_[(i + 1) % x_list_.size()];
        float tail_point_y = y_list_[(i + 1) % x_list_.size()];

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
}

void Freehand::update(float x, float y){
    end_point_x_ = x;
    end_point_y_ = y;
}

void Freehand::add_control_point(float x, float y){
    x_list_.push_back(x);
    y_list_.push_back(y);
}

std::vector<std::pair<int, int>> Freehand::get_interior_pixels() const{
    int min_y = 10000, max_y = 0;
    std::vector<int> temp_y;
    std::vector<int> temp_x;
    for(int i = 0; i < y_list_.size(); i++){
        int now_y = static_cast<int>(std::round(y_list_[i]));
        temp_y.push_back(now_y);
        int now_x = static_cast<int>(std::round(x_list_[i]));
        temp_x.push_back(now_x);
        min_y = std::min(min_y, now_y);
        max_y = std::max(max_y, now_y);
    }
    std::vector<std::vector<int>> point(max_y - min_y);

    for(int i = 0; i < x_list_.size(); i++){
        int x1 = temp_x[i];
        int y1 = temp_y[i];
        int x2 = temp_x[(i + 1) % x_list_.size()];
        int y2 = temp_y[(i + 1) % x_list_.size()];

        if(y1 == y2)
            continue;
        if(y1 > y2){
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        for(int y = y1; y < y2; y++){
            if(x1 == x2)
                point[y - min_y].push_back(x1);
            else{
                float x = x1 + (float)(y + 0.1 - y1) * (y2 - y1) / (float)(x2 - x1);
                point[y - min_y].push_back(static_cast<int>(std::round(x)));
            }
        }

    }

    std::vector<std::pair<int, int>> int_pixels;
    for(int y = 0; y < max_y - min_y; y++){
        std::sort(point[y].begin(), point[y].end());
        if(point[y].size() % 2 != 0){
            std::cout << "error" << std::endl;
            continue;
        }
        for(int i = 0; i < point[y].size(); i += 2){
            int x_start = point[y][i];
            int x_end = point[y][i+1];
            for(int x = x_start; x <= x_end; x++){
                int_pixels.push_back(std::make_pair(x, y + min_y));
            }
        }
    }
    return int_pixels;
}


}