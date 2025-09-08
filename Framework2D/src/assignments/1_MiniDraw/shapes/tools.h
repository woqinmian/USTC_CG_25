#pragma once

#include <cmath>
#include <iostream>


namespace USTC_CG{

float line2vertax_min_distance(float x1, float y1, float x2, float y2, float x0, float y0);

void rotate_by_theta(float &x, float &y, float cx, float cy, double theta);

void scale_by_times(float &x, float &y, float cx, float cy, float times);
}