#include "tools.h"

namespace USTC_CG{

float line2vertax_min_distance(float x1, float y1, float x2, float y2, float x0, float y0){
    if(y2 < y1){
        float temp = x1;
        x1 = x2; x2 = temp;
        temp = y1;
        y1 = y2; y2 = temp;
    }

    float len = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    float cos_theta = (x2 - x1) / len;
    float theta = std::acos(cos_theta);

    x2 = x1 + len;
    y2 = y1;

    float x0_t = x0 - x1;
    float y0_t = y0 - y1;
    x0 =  x0_t * std::cos(theta) + y0_t * std::sin(theta) + x1;
    y0 = -x0_t * std::sin(theta) + y0_t * std::cos(theta) + y1;

    float min;
    if(x0 < x1){
        min = std::sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    }
    else if(x0 <= x2){
        min = std::abs(y1 - y0);
    }
    else min = std::sqrt((x2 - x0) * (x2 - x0) + (y2 - y0) * (y2 - y0));

    return min;
}

void rotate_by_theta(float &x, float &y, float cx, float cy, double theta){
    float tempx = x - cx;
    float tempy = y - cy;

    x = std::cos(theta) * tempx - std::sin(theta) * tempy + cx;
    y = std::sin(theta) * tempx + std::cos(theta) * tempy + cy;
}

void scale_by_times(float &x, float &y, float cx, float cy, float times){
    float tempx = x - cx;
    float tempy = y - cy;

    x = times * tempx + cx;
    y = times * tempy + cy;
}

}