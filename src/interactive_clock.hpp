#pragma once

#include "basic_clock.hpp"
#include "ftxui/component/component.hpp"
#include "utils.hpp"

namespace interactive_clock {
    //component
    auto get() {
        auto&& [comp, state] = basic_clock::get();
        auto new_comp = comp | ftxui::CatchEvent([=](ftxui::Event event) {
            if(event == ftxui::Event::ArrowLeft) {
                state->radian += PI/30.0;
            }
            else if(event == ftxui::Event::ArrowRight) {
                state->radian -= PI/30.0;
            }
            else return false;
            return true;
        });
        //return only the component, as there is no modifyable state to expose
        return  new_comp;
    }
}//namespace interactive_clock