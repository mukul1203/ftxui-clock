#include <cmath>
#include <memory>
#include "ftxui/component/component.hpp"  // for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/dom/canvas.hpp"                    // for Canvas

namespace auto_clock {

#define PI 3.14159265

struct instance {

    //State
    struct state_t {
        //clock second arm state (angle in radian)
        double radian = (PI/2.0);
    };
    using state_ptr = std::shared_ptr<state_t>;

    //Render
    auto operator()() {
        auto radian = _state->radian;
        int canvas_w = 101, canvas_h = 101;
        auto c = ftxui::Canvas(canvas_w, canvas_h);

        // Circle, not filled
        int center_x = 50, center_y = 40, radius = 40;
        c.DrawBlockCircle(center_x, center_y, radius);
        c.DrawBlockLine(center_x, center_y, center_x + radius*cos(radian), center_y - radius*sin(radian));

        return canvas(std::move(c)) | ftxui::border;
    }
    auto state() { return _state; }

    private:
    //all state is on heap, and shared between all copies of this instance, to allow modifying via one copy and affecting the other copy
    //that may be used internally during rendering. Intentional spooky action.
    state_ptr _state = std::make_shared<state_t>();
};

//Return component and its state pointer
auto get() {
instance ins{};
auto state = ins.state();
return std::make_pair<ftxui::Component, instance::state_ptr>(ftxui::Renderer(std::move(ins)), std::move(state));
}

} //namespace auto_clock