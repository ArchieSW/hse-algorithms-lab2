#pragma once
#include "../models/Vec2d.hpp"
#include "../models/Rectangle.hpp"

template <typename T>
concept Solution = requires(T & solution, Rectangles& recs, const Vec2d& query) {
  { solution.prepare_rectangles(recs) } -> std::same_as<void>;
  { solution.get_number_of_crossing(query) } -> std::same_as<int>;
  { solution.get_name() } -> std::same_as<std::string>;
};

