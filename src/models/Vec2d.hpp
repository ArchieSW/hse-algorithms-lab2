#pragma once
#include <vector>
#include <iostream>

struct Vec2d {
  int x;
  int y;

  // NOLINTNEXTLINE
  Vec2d(int x, int y) : x(x), y(y) {}

  Vec2d() = default;
  Vec2d(const Vec2d&) = default;
  Vec2d(Vec2d&&) = default;
  auto operator=(const Vec2d&) -> Vec2d& = default;
  auto operator=(Vec2d&&) -> Vec2d& = default;
};

using Points = std::vector<Vec2d>;

auto operator>>(std::istream& in, Vec2d& point) -> std::istream& {
  in >> point.x >> point.y;
  return in;
}

auto operator<<(std::ostream& out, const Vec2d& point) -> std::ostream& {
  out << point.x << ' ' << point.y;
  return out;
}
