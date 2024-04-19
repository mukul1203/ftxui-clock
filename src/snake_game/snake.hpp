#pragma once

#include <lager/extra/struct.hpp>
#include <lager/util.hpp>
#include <immer/flex_vector.hpp>
#include "utils.hpp"

#include <variant>

namespace snake {

    enum class direction {
        top=0,
        left,
        bottom,
        right
    };

    direction opposite(direction d) {
        switch (d)
        {
        case direction::top: return direction::bottom;
        case direction::left: return direction::right;
        case direction::bottom: return direction::top;
        case direction::right: return direction::left;
        }
    }

    struct model {
        immer::flex_vector<snake_utils::point> points{{0,0}};
        direction dir{direction::right};
    };

    snake_utils::point head(model s) {
        return s.points.front();
    }

    model take_step(model s) {
        static const std::array<std::pair<int,int>, 4> deltas = {{{0, -1}, {-1, 0}, {0, 1}, {1, 0}}};
        auto [del_x, del_y] = deltas[int(s.dir)];
        auto [x, y] = head(s);
        snake_utils::point next_head{x+del_x, y+del_y};
        //if direction happens to be just opposite of what it used to be, that is not possible
        if(s.points.size() > 1 && next_head == *(s.points.begin()++)) {
            s.dir = opposite(s.dir);
            return s;
        }
        //Add a point to head based on direction of movement
        s.points = s.points.push_front(next_head);
        //Remove the last point
        s.points = s.points.erase(s.points.size()-1);
        return s;
    }

    bool breaks_boundary(model s, int width, int height) {
        auto [x, y] = head(s);
        return x < 0 || x >= width || y < 0 || y >= height;
    }

    bool has_cycle(model s) {
        auto pts = s.points;
        return (std::find(pts.begin()+1, pts.end(), head(s)) != pts.end());
    }
}