#pragma once

#include <vector>
#include "shape.h"

namespace USTC_CG{

class Polygon : public Shape{
  public:
    Polygon() = default;
    Polygon(float start_point_x,
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
    }
    virtual ~Polygon() = default;

    void draw(const Config &config) const override;
    void update(float x, float y) override;
    void add_control_point(float x, float y) override;
    void set_sealed();

    float min_distance(float x, float y) override;
    void initial_point() override;
    void update_point() override;
    void update_shape_by_center(float x, float y) override;
    void update_shape_by_vertax(float x, float y) override;

  private:
  std::vector<float> x_list_, y_list_;
  float start_point_x_, start_point_y_;
  float end_point_x_, end_point_y_;
  float min_x_, max_x_;
  float min_y_, max_y_;
  float center_x_, center_y_;
  bool is_sealed = false;
};
}