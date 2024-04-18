#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/component/loop.hpp" 
#include "auto_clock.hpp"

int main(void) {
  //interactive screen and a custom loop
  auto screen = ftxui::ScreenInteractive::FitComponent();
  auto&& [comp, state] = auto_clock::get();
  ftxui::Loop loop(&screen, std::move(comp));
 
  while (!loop.HasQuitted()) {
    loop.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //update state
    state->radian -= PI/30.0;
    screen.PostEvent(ftxui::Event::Custom);
  }
  return EXIT_SUCCESS;
}