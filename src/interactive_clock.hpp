#include "auto_clock.hpp"
#include "ftxui/component/component.hpp"

namespace interactive_clock {
    //state

    //component
    auto get() {
        return auto_clock::get() | ftxui::CatchEvent();
    }
}//namespace interactive_clock