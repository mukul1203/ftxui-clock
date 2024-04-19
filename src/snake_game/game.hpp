#pragma once

#include <vector>
#include <chrono>
#include "snake.hpp"
#include "utils.hpp"

namespace game {
    
    int board_width = 400, board_height = 400;
    std::vector<std::pair<int,int>> apples;
    //snake movement interval
    std::chrono::milliseconds interval{500};
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

    inline model update(model c, action action) {
        return std::visit(lager::visitor{
                          [&](tick_action) { 
                            auto c_next = c;
                            c_next.snake_model = snake::take_step(c_next.snake_model);
                            //check for danger
                            if(snake::breaks_boundary(c_next.snake_model, board_width, board_height) || snake::has_cycle(c_next.snake_model)) {
                                c_next.over = true;
                            }
                            return c_next;
                            },
                          [&](apple_eaten_action) { 
                                c.level_score += apple_score;
                                c.total_score += apple_score;
                                if(c.level_score == level_up_score) {
                                    c.level++;
                                    c.level_score = 0;
                                }
                                return c;
                           },
                          [&](change_direction_action a) { 
                            c.snake_model.dir = a.new_direction;
                            return c;
                           },
                      },
                      action);
    }
}