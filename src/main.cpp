#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/component/loop.hpp" 
#include "basic_clock.hpp"
#include "interactive_clock.hpp"

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

int main(void) {
  run_auto_clock();
  // run_interactive_clock();
  return EXIT_SUCCESS;
}