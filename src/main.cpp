#include <cmath>                   // for sin, cos
#include <ftxui/dom/elements.hpp>  // for canvas, Element, separator, hbox, operator|, border
#include <ftxui/screen/screen.hpp>  // for Pixel
#include <memory>   // for allocator, shared_ptr, __shared_ptr_access
#include <string>   // for string, basic_string
#include <utility>  // for move
#include <vector>   // for vector, __alloc_traits<>::value_type
 
#include "ftxui/component/component.hpp"  // for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/event.hpp"               // for Event
#include "ftxui/component/mouse.hpp"               // for Mouse
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/canvas.hpp"                    // for Canvas
#include "ftxui/screen/color.hpp"  // for Color, Color::Red, Color::Blue, Color::Green, ftxui
#include "ftxui/component/loop.hpp" 

#define PI 3.14159265
int main(void) {
  using namespace ftxui;

  //clock second arm state (angle in radian)
  double radian = (PI / 2.0);

  //clock component
  auto clock_component = Renderer([&] {
    int canvas_w = 101, canvas_h = 101;
    auto c = Canvas(canvas_w, canvas_h);

    // Circle, not filled
    int center_x = 50, center_y = 40, radius = 40;
    c.DrawPointCircle(center_x, center_y, radius);
    c.DrawPointLine(center_x, center_y, center_x + radius*cos(radian), center_y - radius*sin(radian));

    return canvas(std::move(c)) | border;
  });
  
  //interactive screen and a custom loop
  auto screen = ScreenInteractive::FitComponent();
  Loop loop(&screen, clock_component);
 
  while (!loop.HasQuitted()) {
    loop.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //update state
    radian -= PI/30.0;
  }
  return EXIT_SUCCESS;
}