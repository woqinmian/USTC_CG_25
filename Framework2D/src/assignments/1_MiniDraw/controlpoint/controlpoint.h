#pragma once

namespace USTC_CG{

class Controlpoint{
public:
    struct Config{
        float bias[2] = {0.0f, 0.0f};
        unsigned char line_color[4] = {255, 0, 0, 255};
        float line_thickness = 2.0f;
        float scale = 1.0f;
    };
    enum pointType{
        center = 0,
        rotate = 1,
        scale = 2,
        vertax = 3,
        edge = 4,
    };


public:
    Controlpoint(float x, float y, pointType point_type = center, int order = 0): x_(x), y_(y), point_type_(point_type), order_(order) {}
    ~Controlpoint() = default;
    void draw(const Config &config) const;
    void update(float x, float y);
    float distance(float x, float y) {return (x_ - x) * (x_ - x) + (y_ - y) * (y_ - y); }
    float x_, y_;
    pointType point_type_;
    int order_;

};


}