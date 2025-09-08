#pragma once

#include <vector>

#include "tools.h"
#include "../controlpoint/controlpoint.h"

namespace USTC_CG
{
class Shape
{
   public:
    // Draw Settings
    struct Config
    {
        // Offset to convert canvas position to screen position
        float bias[2] = { 0.f, 0.f };
        // Line color in RGBA format
        float scale = 1.0f;
    };

   public:
    Shape(float color[], float thickness = 2.0f){
        for(int i = 0; i < 4; i++){
            line_color[i] = static_cast<unsigned char>(color[i] * 255);
        }
        line_thickness = thickness;
    }

    virtual ~Shape() = default;

    /**
     * Draws the shape on the screen.
     * This is a pure virtual function that must be implemented by all derived
     * classes.
     *
     * @param config The configuration settings for drawing, including line
     * color, thickness, and bias.
     *               - line_color defines the color of the shape's outline.
     *               - line_thickness determines how thick the outline will be.
     *               - bias is used to adjust the shape's position on the
     * screen.
     */
    virtual void draw(const Config& config) const = 0;
    /**
     * Updates the state of the shape.
     * This function allows for dynamic modification of the shape, in response
     * to user interactions like dragging.
     *
     * @param x, y Dragging point. e.g. end point of a line.
     */
    virtual void update(float x, float y) = 0;
    /**
     * Adds a control point to the shape.
     * This function is used to add control points to the shape, which can be
     * used to modify the shape's appearance.
     *
     * @param x, y Control point to be added. e.g. vertex of a polygon.
     */
    virtual void add_control_point(float x, float y) {if(x < 0 || y < 0) return;}
    virtual float min_distance(float x, float y) {return x-x+y-y+1000.0f;}
    virtual void initial_point() {}
    virtual void update_point() {}
    virtual void update_shape_by_center(float x, float y) {if(x < 0 || y < 0) return;}
    virtual void update_shape_by_rotate(float x, float y) {if(x < 0 || y < 0) return;}
    virtual void update_shape_by_scale(float x, float y) {if(x < 0 || y < 0) return;}
    virtual void update_shape_by_vertax(float x, float y) {if(x < 0 || y < 0) return;}
    virtual void update_shape_by_edge(float x, float y) {if(x < 0 || y < 0) return;}

    void set_selected() { update_point(); is_selected = true; }
    void cancel_selected() { is_selected = false; }

    void set_linecolor(float color[]){
        for(int i = 0; i < 4; i++){
            line_color[i] = static_cast<unsigned char>(color[i] * 255);
        }
    }

    void set_linethickness(float val){
        line_thickness = val;
    }

    bool select_point(float x, float y){
        for(auto &point : point_list_){
            if(point->distance(x, y) <= 64.0f){
                current_point_ = point;
                update_shape(x, y);
                return true;
            }
        }
        return false;
    }

    void update_shape(float x, float y){
        switch(current_point_->point_type_){
            case Controlpoint::center:{
                update_shape_by_center(x, y);
                break;
            }
            case Controlpoint::rotate:{
                update_shape_by_rotate(x, y);
                break;
            }
            case Controlpoint::scale:{
                update_shape_by_scale(x, y);
                break;
            }
            case Controlpoint::vertax:{
                update_shape_by_vertax(x, y);
                break;
            }
            case Controlpoint::edge:{
                update_shape_by_edge(x, y);
            }
            default: break;
        }
        update_point();
    }

public:
    std::shared_ptr<Controlpoint> current_point_;
    std::vector<std::shared_ptr<Controlpoint>> point_list_;
    bool is_selected = false;
    unsigned char line_color[4];
    float line_thickness;
    //float center_point_x_, center_point_y_;
};
}  // namespace USTC_CG