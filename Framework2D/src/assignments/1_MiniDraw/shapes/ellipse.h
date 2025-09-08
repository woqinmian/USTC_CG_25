#pragma once

#include "shape.h"

namespace USTC_CG{

class Ellipse : public Shape{
  public:
    Ellipse() = default;
    Ellipse(float start_point_x, 
            float start_point_y, 
            float end_point_x, 
            float end_point_y, float color[], float thickness)
            : Shape(color, thickness),
              start_point_x(start_point_x), 
              start_point_y(start_point_y), 
              end_point_x(end_point_x),
              end_point_y(end_point_y){
      point_x[0] = start_point_x;
      point_y[0] = start_point_y;
      point_x[1] = end_point_x;
      point_y[1] = start_point_y;
      point_x[2] = end_point_x;
      point_y[2] = end_point_y;
      point_x[3] = start_point_x;
      point_y[3] = end_point_y;
      initial_point();
    }

    virtual ~Ellipse() = default;

    void draw(const Config &config) const override;

    void update(float x, float y) override;
    float min_distance(float x, float y) override;
    void initial_point() override;
    void update_point() override;
    void update_shape_by_center(float x, float y) override;
    void update_shape_by_rotate(float x, float y) override;
    void update_shape_by_scale(float x, float y) override;
    void update_shape_by_vertax(float x, float y) override;
    void update_shape_by_edge(float x, float y) override;

  private:
    float start_point_x, start_point_y;
    float end_point_x, end_point_y;
    float point_x[4];
    float point_y[4];
    float center_x_, center_y_;
    float a_, b_;
    float theta_;
};
}