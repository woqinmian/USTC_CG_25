#include "RBF_warper.h"

namespace USTC_CG{

std::pair<int, int> RBFWarper::warp(int x, int y){
    int n = start_points_.size();

    Eigen::MatrixXf coefficient(1, n + 3);
    for(int i = 0; i < n; i++){
        coefficient(0, i) = g(i, ImVec2(x, y));
    }
    coefficient(0, n + 0) = x;
    coefficient(0, n + 1) = y;
    coefficient(0, n + 2) = 1;
    Eigen::MatrixXf result(1, 2);
    result = coefficient * alpha_A_b_;
    
    return {static_cast<int>(result(0, 0)), static_cast<int>(result(0, 1))};
}

void RBFWarper::initialize(){
    int n = start_points_.size();

    r2_.clear();
    for(int i = 0; i < n; i++){
        float r2 = 1e10;
        for(int j = 0; j < n; j++){
            if(i == j)
                continue;
            float dx = start_points_[i].x - start_points_[j].y;
            float dy = start_points_[i].y - start_points_[j].y;
            float d2 = dx * dx + dy * dy;
            r2 = std::min(r2, d2);
        }
        r2_.push_back(r2);
    }

    alpha_A_b_.resize(n + 3, 2);
    Eigen::MatrixXf coefficient(n + 3, n + 3);
    Eigen::MatrixXf q_0(n + 3, 2);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            coefficient(i, j) = g(j, start_points_[i]);
        }
        coefficient(i, n + 0) = start_points_[i].x;
        coefficient(i, n + 1) = start_points_[i].y;
        coefficient(i, n + 2) = 1;
        coefficient(n + 0, i) = start_points_[i].x;
        coefficient(n + 1, i) = start_points_[i].y;
        coefficient(n + 2, i) = 1;
        q_0(i, 0) = end_points_[i].x;
        q_0(i, 1) = end_points_[i].y;
    }
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            coefficient(n + i, n + j) = 0.0f;
        }
        for(int j = 0; j < 2; j++){
            q_0(n + i, j) = 0.0f;
        }
    }

    alpha_A_b_ = coefficient.colPivHouseholderQr().solve(q_0);
}

float RBFWarper::g(int i, const ImVec2 &p){
    float dx = start_points_[i].x - p.x;
    float dy = start_points_[i].y - p.y;
    float d2 = dx * dx + dy * dy;
    return std::pow(d2 + r2_[i], mu_ / 2.0f);
}

}