#pragma once

#include <vector>
#include "shape.h"

namespace USTC_CG{

class Freehand : public Shape{
  public:
    Freehand() = default;
    Freehand(float start_point_x,
             float start_point_y,
             float end_point_x,
             float end_point_y)
            : start_point_x_(start_point_x),
              start_point_y_(start_point_y),
              end_point_x_(end_point_x),
              end_point_y_(end_point_y){
      x_list_.push_back(start_point_x_);
      y_list_.push_back(start_point_y_);
    }
    virtual ~Freehand() = default;

    void draw(const Config &config) const override;
    void update(float x, float y) override;
    void add_control_point(float x, float y) override;
    std::vector<std::pair<int, int>> get_interior_pixels() const override;

  private:
    std::vector<float> x_list_, y_list_;
    float start_point_x_, start_point_y_;
    float end_point_x_, end_point_y_;
};

}