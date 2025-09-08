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
             float end_point_y, float color[], float thickness)
            : Shape(color, thickness),
              start_point_x_(start_point_x),
              start_point_y_(start_point_y),
              end_point_x_(end_point_x),
              end_point_y_(end_point_y){
      x_list_.push_back(start_point_x_);
      y_list_.push_back(start_point_y_);
      initial_point();
    }
    virtual ~Freehand() = default;

    void draw(const Config &config) const;
    void update(float x, float y);
    void add_control_point(float x, float y);

    float min_distance(float x, float y) override;
    void initial_point() override;
    void update_point() override;
    void update_shape_by_center(float x, float y) override;

  private:
    std::vector<float> x_list_, y_list_;
    float start_point_x_, start_point_y_;
    float end_point_x_, end_point_y_;
    float min_x_, max_x_;
    float min_y_, max_y_;
    float center_x_, center_y_;
};

}