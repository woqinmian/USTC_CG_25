#include "IDW_warper.h"

namespace USTC_CG{

std::pair<int, int> IDWWarper::warp(int x, int y){
    int n = start_points_.size();
    std::vector<float> w;
    float Sigma_p = 0.0f;
    for(int i = 0; i < n; i++){
        w.push_back(sigma(i, ImVec2(x, y)));
        Sigma_p += w[i];
    }
    for(int i = 0; i < n; i++){
        w[i] /= Sigma_p;
    }

    float new_x = 0.0f;
    float new_y = 0.0f;
    for(int i = 0; i < n; i++){
        new_x += w[i] * (end_points_[i].x + T_[i][0][0] * (x - start_points_[i].x) + T_[i][0][1] * (y - start_points_[i].y));
        new_y += w[i] * (end_points_[i].y + T_[i][1][0] * (x - start_points_[i].x) + T_[i][1][1] * (y - start_points_[i].y));
    }

    return {static_cast<int>(new_x), static_cast<int>(new_y)};
}

void IDWWarper::initialize(){
    T_.clear();

    int n = start_points_.size();
    for(int i = 0; i < n; i++){
        glm::mat2 A(0.0f), B(0.0f);
        for(int j = 0; j < n; j++){
            if(i == j)
                continue;

            glm::vec2 dp(start_points_[j].x - start_points_[i].x, start_points_[j].y - start_points_[i].y);
            glm::vec2 dq(end_points_[j].x - end_points_[i].x, end_points_[j].y - end_points_[i].y);
            float sigmaij = sigma(i, start_points_[j]);

            A[0][0] += sigmaij * dp.x * dp.x;
            A[0][1] += sigmaij * dp.x * dp.y;
            A[1][0] += sigmaij * dp.y * dp.x;
            A[1][1] += sigmaij * dp.y * dp.y;
            B[0][0] += sigmaij * dq.x * dp.x;
            B[0][1] += sigmaij * dq.x * dp.y;
            B[1][0] += sigmaij * dq.y * dp.x;
            B[1][1] += sigmaij * dq.y * dp.y; 
        }
        if(glm::determinant(A) == 0.0f){
            T_.push_back(glm::mat2(0.0f));
            continue;
        }
        glm::mat2 Ti = B * glm::inverse(A);
        T_.push_back(Ti);
    }
}

float IDWWarper::sigma(int i, const ImVec2 &p){
    float dx = start_points_[i].x - p.x;
    float dy = end_points_[i].y - p.y;
    float d = std::sqrt(dx * dx + dy * dy);
    return 1.0f / std::pow(std::max(d, 1e-6f), mu_);
}

}