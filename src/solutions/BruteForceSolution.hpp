#pragma once
#include "../models/Rectangle.hpp"
#include "../models/Vec2d.hpp"
#include "../common.hpp"
#include "Solution.hpp"

class BruteForceSolution {
 public:
  void prepare_rectangles(const Rectangles& recs) { rectangles = recs; }

  auto get_number_of_crossing(const Vec2d& query) -> int {
    int out = 0;
    for (auto& rectangle : rectangles) {
      if (rectangle.is_inside_without_left_and_top(query)) {
        out++;
      }
    }
    return out;
  }

  BruteForceSolution() = default;
  BruteForceSolution(BruteForceSolution&&) = default;
  BruteForceSolution(const BruteForceSolution&) = default;
  auto operator=(BruteForceSolution&&) -> BruteForceSolution& = default;
  auto operator=(const BruteForceSolution&) -> BruteForceSolution& = default;
  ~BruteForceSolution() = default;

  [[nodiscard]] auto get_name() const -> std::string { return "BruteForceSolution"; }

 private:
  Rectangles rectangles;
};

static_assert(Solution<BruteForceSolution>, "BruteForceSolution is not implemen needed methods");
