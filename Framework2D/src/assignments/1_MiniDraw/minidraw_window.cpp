#include "minidraw_window.h"

#include <iostream>

namespace USTC_CG
{
MiniDraw::MiniDraw(const std::string& window_name) : Window(window_name)
{
    p_canvas_ = std::make_shared<Canvas>("Widget.Canvas");
}

MiniDraw::~MiniDraw()
{
}

void MiniDraw::draw()
{
    draw_canvas();
}

void MiniDraw::draw_canvas()
{
    // Set a full screen canvas view
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    if (ImGui::Begin(
            "Canvas",
            &flag_show_canvas_view_,
            ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoBackground))
    {
        // Buttons for shape types
        if (ImGui::Button("Line"))
        {
            std::cout << "Set shape to Line" << std::endl;
            p_canvas_->set_line();
        }
        ImGui::SameLine();
        if (ImGui::Button("Rect"))
        {
            std::cout << "Set shape to Rect" << std::endl;
            p_canvas_->set_rect();
        }
        ImGui::SameLine();
        if (ImGui::Button("Ellipse"))
        {
            std::cout << "Set shape to Ellipse" << std::endl;
            p_canvas_->set_ellipse();
        }
        ImGui::SameLine();
        if (ImGui::Button("Polygon"))
        {
            std::cout << "Set shape to Polygon" << std::endl;
            p_canvas_->set_polygon();
        }
        ImGui::SameLine();
        if (ImGui::Button("Freehand"))
        {
            std::cout << "Set shape to Freehand" << std::endl;
            p_canvas_->set_freehand();
        }

        ImGui::BulletText("Line Color");ImGui::SameLine();
        ImGui::ColorEdit4(
            "Line_Color",
            color,
            ImGuiColorEditFlags_NoLabel |
            ImGuiColorEditFlags_AlphaBar |
            ImGuiColorEditFlags_AlphaPreview |
            ImGuiColorEditFlags_AlphaPreviewHalf |
            ImGuiColorEditFlags_HDR |
            ImGuiColorEditFlags_PickerHueWheel);
        const static float thickness_min = 1.0f, thickness_max = 10.0f;
        ImGui::BulletText("Line Thickness");ImGui::SameLine();
        ImGui::SliderScalar(
            "##",
            ImGuiDataType_Float,
            &thickness,
            &thickness_min,
            &thickness_max);


        if(ImGui::Button("Select")){
            std::cout << "Set act to Select" << std::endl;
            p_canvas_->set_select();
        }

        // HW1_TODO: More primitives
        //    - Ellipse
        //    - Polygon
        //    - Freehand(optional)
        
        // Canvas component
        ImGui::Text("Press left mouse to add shapes.");
        // Set the canvas to fill the rest of the window
        const auto& canvas_min = ImGui::GetCursorScreenPos();
        const auto& canvas_size = ImGui::GetContentRegionAvail();
        //ImVec2 canvas_min(8.0f, 60.0f);
        //ImVec2 canvas_size(800.0f, 600.0f);
        p_canvas_->set_attributes(canvas_min, canvas_size, color, thickness);
        p_canvas_->draw();
    }
    ImGui::End();
}
}  // namespace USTC_CG