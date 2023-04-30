#pragma once
#include <iostream>
#include <vector>

#include "Vec2d.hpp"

class Rectangle {
 public:
  friend auto operator>>(std::istream& in, Rectangle& rec) -> std::istream&;
  friend auto operator<<(std::ostream& out, const Rectangle& rec) -> std::ostream&;

  Rectangle() = default;
  Rectangle(Rectangle&&) = default;
  Rectangle(const Rectangle&) = default;
  auto operator=(Rectangle&&) -> Rectangle& = default;
  auto operator=(const Rectangle&) -> Rectangle& = default;
  ~Rectangle() = default;

  // NOLINTNEXTLINE
  Rectangle(const Vec2d& left_bottom, const Vec2d& right_top)
      : left_bottom_angle(left_bottom), right_top_angle(right_top) {}

  [[nodiscard]] auto is_inside(const Vec2d& point) const -> bool {
    return left_bottom_angle.x <= point.x && left_bottom_angle.y <= point.y &&
           right_top_angle.x >= point.x && right_top_angle.y >= point.y;
  }

  [[nodiscard]] auto is_inside_without_left_and_top(const Vec2d& point) const -> bool {
    return left_bottom_angle.x <= point.x && left_bottom_angle.y <= point.y &&
           right_top_angle.x > point.x && right_top_angle.y > point.y;
  }

  [[nodiscard]] auto get_left_bottom_angle() const -> const Vec2d& { return left_bottom_angle; }

  [[nodiscard]] auto get_right_top_angle() const -> const Vec2d& { return right_top_angle; }

  void set_left_bottom_angle(const Vec2d& to_update) { left_bottom_angle = to_update; }

  void set_right_top_angle(const Vec2d& to_update) { right_top_angle = to_update; }

 private:
  Vec2d left_bottom_angle;
  Vec2d right_top_angle;
};

auto operator>>(std::istream& in, Rectangle& rec) -> std::istream& {
  in >> rec.left_bottom_angle >> rec.right_top_angle;
  return in;
}

using Rectangles = std::vector<Rectangle>;

auto operator<<(std::ostream& out, const Rectangle& rec) -> std::ostream& {
  out << rec.left_bottom_angle << ' ' << rec.right_top_angle;
  return out;
}
