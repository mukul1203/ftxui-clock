#pragma once
#include "ftxui/component/component.hpp"  // for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/dom/canvas.hpp"           // for Canvas
#include "../game.hpp"

namespace game {
struct instance {
    //Render
    auto operator()() {
        auto& model = game::g_instance; 
        int canvas_w = game::board_width, canvas_h = game::board_height;
        auto c = ftxui::Canvas(canvas_w, canvas_h);
        // Draw apple
        c.DrawBlock(model.apple.first, model.apple.second, true, ftxui::Color::Palette16::Red);
        // Draw snake
        for(auto& [x, y]: model.snake_model.points) {
            c.DrawBlock(x, y, true, ftxui::Color::Palette16::Blue);
        }
        return canvas(std::move(c)) | ftxui::border;
    }
};

//Return component and its state pointer
auto get() {
return ftxui::Renderer(instance{}) | ftxui::CatchEvent([](ftxui::Event event) {
    auto& store = game::get_store();
    if(event == ftxui::Event::ArrowLeft)
        store.dispatch(game::change_direction_action{snake::direction::left});
    else if(event == ftxui::Event::ArrowUp)
        store.dispatch(game::change_direction_action{snake::direction::top});
    else if(event == ftxui::Event::ArrowRight)
        store.dispatch(game::change_direction_action{snake::direction::right});
    else if(event == ftxui::Event::ArrowDown)
        store.dispatch(game::change_direction_action{snake::direction::bottom});
    return true;
});
}

}