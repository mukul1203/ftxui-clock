#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/component/loop.hpp" 
#include "basic_clock.hpp"
#include "interactive_clock.hpp"

#include "counter.hpp"
#include "snake_game/game.hpp"
#include "snake_game/snake.hpp"

#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>

void run_auto_clock() {
  //interactive screen and a custom loop
  auto screen = ftxui::ScreenInteractive::FitComponent();
  auto&& [comp, state] = basic_clock::get();
  ftxui::Loop loop(&screen, std::move(comp));
 
  while (!loop.HasQuitted()) {
    loop.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //update state
    state->radian -= PI/30.0;
    screen.PostEvent(ftxui::Event::Custom);
  }
}

void run_interactive_clock() {
  //interactive screen and a custom loop
  auto screen = ftxui::ScreenInteractive::FitComponent();
  screen.Loop(interactive_clock::get());
}

void run_counter() {
auto draw = [](counter::model curr)
  {
      std::cout << "current value: " << curr.value << '\n';
  };

auto intent = [](char event)->std::optional<counter::action> 
{
    switch (event) {
    case '+':
        return counter::increment_action{};
    case '-':
        return counter::decrement_action{};
    case '.':
        return counter::reset_action{};
    default:
        return std::nullopt;
    }
};

  auto store = lager::make_store<counter::action>(
        counter::model{}, lager::with_manual_event_loop{});
    watch(store, draw);

    auto event = char{};
    while (std::cin >> event) {
        if (auto act = intent(event))
            store.dispatch(*act);
    }
}

void run_snake_game() {
  auto draw = [](game::model curr)
  {
      // std::cout << "current value: " << curr.value << '\n';
  };

auto intent = [](char event)->std::optional<game::action> 
{
    switch (event) {
    case '+':
        return game::tick_action{};
    case '-':
        return game::apple_eaten_action{};
    case '.':
        return game::change_direction_action{};
    default:
        return std::nullopt;
    }
};

  auto store = lager::make_store<game::action>(
        game::model{}, lager::with_manual_event_loop{});
    watch(store, draw);

    auto event = char{};
    while (std::cin >> event) {
        if (auto act = intent(event))
            store.dispatch(*act);
    }
}

int main(void) {
  // run_auto_clock();
  // run_interactive_clock();
  // run_counter();
  run_snake_game();
  return EXIT_SUCCESS;
}