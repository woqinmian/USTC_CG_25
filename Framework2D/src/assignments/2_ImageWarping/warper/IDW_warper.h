// HW2_TODO: Implement the IDWWarper class
#pragma once

#include <vector>
#include <cmath>
#include "warper.h"
#include "imgui.h"

namespace USTC_CG
{
class IDWWarper : public Warper
{
   public:
    IDWWarper() = default;

    IDWWarper(const std::vector<ImVec2> &start_points, const std::vector<ImVec2> &end_points, float mu = 2.0f)
        : start_points_(start_points), end_points_(end_points), mu_(mu) {
        initialize();
    }

    virtual ~IDWWarper() = default;
    // HW2_TODO: Implement the warp(...) function with IDW interpolation
    std::pair<int, int> warp(int x, int y) override;
    void initialize();
    float sigma(int i, const ImVec2 &p);

    // HW2_TODO: other functions or variables if you need
private:
    std::vector<ImVec2> start_points_, end_points_;
    float mu_;
    std::vector<glm::mat2> T_;
};
}  // namespace USTC_CG