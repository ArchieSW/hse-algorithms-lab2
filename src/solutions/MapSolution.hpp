#pragma once
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

#include "../common.hpp"
#include "../models/Rectangle.hpp"
#include "../models/Vec2d.hpp"
#include "Solution.hpp"

class MapSolution {
 public:
  MapSolution() = default;
  MapSolution(MapSolution&&) = default;
  MapSolution(const MapSolution&) = default;
  auto operator=(MapSolution&&) -> MapSolution& = default;
  auto operator=(const MapSolution&) -> MapSolution& = default;
  ~MapSolution() = default;

  [[nodiscard]] auto get_name() const -> std::string { return "MapSolution"; }

  void prepare_rectangles(const Rectangles& recs) {
    rectangles = recs;
    compressed_x.clear();
    compressed_y.clear();
    compress_rectangles();
    fill_map();
  }

  auto get_number_of_crossing(const Vec2d& query) -> int {
    // std::cout << "call" << std::endl;
    Vec2d compressed = compress_point(query);
    // std::cout << "compressed" << std::endl;
    if (compressed.x < 0 || compressed.x >= static_cast<int>(compressed_x.size()) ||
        compressed.y < 0 || compressed.y >= static_cast<int>(compressed_y.size())) {
      // std::cout << "if statement" << std::endl;
      return 0;
    }
    // std::cout << "after if" << std::endl;
    // std::cout << "solution_map[compressed.x][compressed.y] " << compressed.x << ' ' << compressed.y
    //           << std::endl;
    // std::cout << "get " << solution_map[compressed.x][compressed.y] << std::endl;
    return solution_map[compressed.x][compressed.y];
  }

 private:
  void compress_rectangles() {
    for (const auto& rectangle : rectangles) {
      const auto& lb = rectangle.get_left_bottom_angle();
      const auto& rt = rectangle.get_right_top_angle();
      compressed_x.push_back(lb.x);
      compressed_x.push_back(rt.x);
      compressed_y.push_back(lb.y);
      compressed_y.push_back(rt.y);
    }
    std::set compressed_and_sorted_x(compressed_x.begin(), compressed_x.end());
    std::set compressed_and_sorted_y(compressed_y.begin(), compressed_y.end());
    compressed_y.clear();
    compressed_x.clear();
    compressed_x.insert(compressed_x.begin(), compressed_and_sorted_x.begin(),
                        compressed_and_sorted_x.end());
    compressed_y.insert(compressed_y.begin(), compressed_and_sorted_y.begin(),
                        compressed_and_sorted_y.end());
  }

  auto compress_point_1d(const std::vector<int>& compressed_axis, int point) -> int {
    auto res = std::upper_bound(compressed_axis.begin(), compressed_axis.end(), point);
    return static_cast<int>(res - compressed_axis.begin() - 1);
  }

  auto compress_point(const Vec2d& point) -> Vec2d {
    return {compress_point_1d(compressed_x, point.x), compress_point_1d(compressed_y, point.y)};
  }

  void fill_map() {
    solution_map = std::vector<std::vector<int>>(compressed_x.size(),
                                                 std::vector<int>(compressed_y.size(), 0));
    for (auto& rectangle : rectangles) {
      Vec2d compressed_left_bottom = compress_point(rectangle.get_left_bottom_angle());
      Vec2d compressed_right_top = compress_point(rectangle.get_right_top_angle());
      for (int i = compressed_left_bottom.x; i < compressed_right_top.x; i++) {
        for (int j = compressed_left_bottom.y; j < compressed_right_top.y; j++) {
          solution_map[i][j]++;
        }
      }
    }
  }

 private:
  Rectangles rectangles;
  std::vector<int> compressed_x;
  std::vector<int> compressed_y;
  std::vector<std::vector<int>> solution_map;
};

static_assert(Solution<MapSolution>, "MapSolution is not implement needed methods");
