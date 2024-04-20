#pragma once

#include <vector>
#include <chrono>
#include "snake.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>
#include <lager/effect.hpp>

namespace game {
    
    int board_width = 100, board_height = 100;
    std::vector<std::pair<int,int>> apples;
    //snake movement interval
    std::chrono::milliseconds interval{100};
    int apple_score{1};
    int level_up_score{20};
    struct model {
        //current apple position
        snake_utils::point apple{board_width/2, board_height/2};
        int total_score{0};
        int level_score{0};
        int level{1};
        snake::model snake_model;
        bool over{false};
    };

    struct tick_action {};
    struct apple_eaten_action{};
    struct change_direction_action {
        snake::direction new_direction;
    };

    using action = std::variant<tick_action, apple_eaten_action, change_direction_action>;
    using update_result = std::pair<model, lager::effect<action>>;
    inline update_result update(model c, action action) {
        return std::visit(lager::visitor{
                          [&](tick_action)->update_result { 
                            c.snake_model = snake::take_step(c.snake_model);
                            //check for danger
                            if(snake::breaks_boundary(c.snake_model, board_width, board_height) || snake::has_cycle(c.snake_model)) {
                                c.over = true;
                            }
                            if(snake::head(c.snake_model) == c.apple)
                                return {c, [] (auto&& ctx) {
                                            ctx.dispatch(apple_eaten_action{});
                                        }};
                            return {c, lager::noop};
                            },
                          [&](apple_eaten_action)->update_result { 
                                c.level_score += apple_score;
                                c.total_score += apple_score;
                                //Just push the apple on head. Eventually, this will come to the tail and extend snake.
                                c.snake_model.points = c.snake_model.points.push_front(c.apple);
                                //Update apple position
                                c.apple = {rand()%board_width, rand()%board_height};
                                if(c.level_score == level_up_score) {
                                    c.level++;
                                    c.level_score = 0;
                                    c.snake_model = {};
                                }
                                return {c, lager::noop};
                           },
                          [&](change_direction_action a)->update_result { 
                            c.snake_model.dir = a.new_direction;
                            return {c, lager::noop};
                           },
                      },
                      action);
    }

    //Used in UI and updated by watch(store)
    model g_instance{};

    auto& get_store() {
        static auto store = lager::make_store<game::action>(
        game::model{}, lager::with_manual_event_loop{});
        return store;
    }
}