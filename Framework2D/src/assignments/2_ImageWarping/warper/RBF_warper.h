// HW2_TODO: Implement the RBFWarper class
#pragma once

#include <vector>
#include <cmath>
#include <eigen/Eigen/Dense>
#include "warper.h"
#include "imgui.h"

namespace USTC_CG
{
class RBFWarper : public Warper
{
   public:
    RBFWarper() = default;

    RBFWarper(const std::vector<ImVec2> &start_points, const std::vector<ImVec2> &end_points, float mu = 1.0f)
        : start_points_(start_points), end_points_(end_points), mu_(mu) {
        initialize();
    }

    virtual ~RBFWarper() = default;
    // HW2_TODO: Implement the warp(...) function with RBF interpolation
    std::pair<int, int> warp(int x, int y) override;
    void initialize();
    float g(int i, const ImVec2 &p);

    // HW2_TODO: other functions or variables if you need
private:
    std::vector<ImVec2> start_points_, end_points_;
    std::vector<float> r2_;
    float mu_;
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> alpha_A_b_;
};
}  // namespace USTC_CG