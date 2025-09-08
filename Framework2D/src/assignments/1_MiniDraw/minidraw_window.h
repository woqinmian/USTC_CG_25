#pragma once

#include <memory>

#include "common/window.h"
#include "canvas_widget.h"

namespace USTC_CG
{
class MiniDraw : public Window
{
   public:
    explicit MiniDraw(const std::string& window_name);
    ~MiniDraw();

    void draw();

   private:
    void draw_canvas();

    std::shared_ptr<Canvas> p_canvas_ = nullptr;

    bool flag_show_canvas_view_ = true;

    float color[4] = {1, 0, 0, 1};
    float thickness = 2.0f;
};
}  // namespace USTC_CG