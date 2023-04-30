#include <bits/chrono.h>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <stdexcept>
#include "../solutions/BruteForceSolution.hpp"
#include "../solutions/MapSolution.hpp"
#include "../solutions/PersistentSegmentTreeSolution.hpp"
#include "../solutions/Solution.hpp"
#include "src/benchmarks/TestCaseGenerator.hpp"
#include "src/models/Rectangle.hpp"

template <Solution T>
auto bench_preparation(T solution, const Rectangles& recs) -> std::uint64_t {
  const int COUNT_OF_LAUNCHES = 20;
  std::uint64_t average_time = 0;
  for (int i = 0; i < COUNT_OF_LAUNCHES; i++) {
    auto start = std::chrono::high_resolution_clock::now();
    solution.prepare_rectangles(recs);
    auto end = std::chrono::high_resolution_clock::now();
    auto execution_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    if (average_time + execution_time < average_time) {
      throw std::runtime_error("uint64_t overflow");
    }
    average_time += execution_time;
  }
  average_time /= COUNT_OF_LAUNCHES;
  return average_time;
}

template <Solution T>
auto bench_queries(T solution, const Rectangles& recs, const Points& points) -> std::uint64_t {
  solution.prepare_rectangles(recs);

  const int COUNT_OF_LAUNCHES = 20;
  std::uint64_t average_time = 0;
  for (int i = 0; i < COUNT_OF_LAUNCHES; i++) {
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& query : points) {
      solution.get_number_of_crossing(query);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto execution_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    if (average_time + execution_time < average_time) {
      throw std::runtime_error("uint64_t overflow");
    }
    average_time += execution_time;
  }
  average_time /= COUNT_OF_LAUNCHES;
  return average_time;
}

template <Solution T>
void bench(const Rectangles& recs, const Points& queries, T solution) {
  volatile auto time_prepare = bench_preparation(solution, recs);
  volatile auto time_queries = bench_queries(solution, recs, queries);
  std::cout << solution.get_name() << ";" << recs.size() << ";" << time_prepare
            << ";" << time_queries << std::endl;
}

template <typename... Args>
requires(Solution<Args>&&...) void bench_all(TestCaseGenerator gen, Args... args) {
  auto counts_of_test_data = std::vector<int>{10,  20,  50,  80,  100, 150,  250,  300,  400, 450,
                                              500, 600, 700, 800, 900, 1000, 1300, 1500, 2000};

  std::cout << "Solution name" << ";" << "count of test data" << ";"
            << "preparation time" << ";" << "queries time" << std::endl;

  for (auto& count : counts_of_test_data) {
    auto recs = gen.generate_rectangles(count);
    auto points = gen.generate_points(count);
    (bench(recs, points, args), ...);
  }
}

auto main() -> int {
  TestCaseGenerator gen;
  bench_all(gen, BruteForceSolution(), MapSolution(), PersistentSegmentTreeSolution());
  return 0;
}
