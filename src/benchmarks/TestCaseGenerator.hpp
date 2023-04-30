#pragma once

#include "src/models/Rectangle.hpp"
#include "src/models/Vec2d.hpp"

class TestCaseGenerator {
 public:
  auto generate_rectangles(int count) -> Rectangles {
    auto out = Rectangles();
    out.reserve(count);

    for (int i = 0; i < count; i++) {
      Vec2d left_bottom = {10 * i, 10 * i};
      Vec2d right_top = {10 * (2 * count - i), 10 * (2 * count - i)};
      out.emplace_back(left_bottom, right_top);
    }

    return out;
  }

 private:
  // NOLINTNEXTLINE
  auto bin_pow(int base, int exp, int mod) -> int {
    int res = 1;
    while (exp > 0) {
      if (exp % 2 == 1) {
        res = (res * base);
      }
      exp = exp / 2;
      base = (base * base);
    }
    return res % mod;
  }

  auto get_point(int prime, int count, int iteration) {
    return bin_pow(prime * iteration, 31, 20 * count);
  }

 public:
  auto generate_points(int count) -> Points {
    int x_prime = 37;
    int y_prime = 73;
    auto points = Points();
    for (int i = 0; i < count; i++) {
      int x = get_point(x_prime, count, i);
      int y = get_point(y_prime, count, i);
      points.emplace_back(x, y);
    }
    return points;
  }
};
