#pragma once
#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>

#include "../models/Rectangle.hpp"
#include "../models/Vec2d.hpp"
#include "Solution.hpp"

class PersistentSegmentTree {
 public:
  struct Node {
   public:
    friend class PersistentSegmentTree;

   public:
    using NodePtr = std::shared_ptr<Node>;
    using ToBuildType = std::vector<int>;

   public:
    NodePtr left = nullptr;
    NodePtr right = nullptr;
    int sum = 0;
    int left_bound_inclusive;
    int right_bound_exclusive;

    static auto new_node(int left_bound_inclusive, int right_bound_exclusive) -> NodePtr {
      return std::make_shared<Node>(left_bound_inclusive, right_bound_exclusive);
    }

    static auto new_node(const ToBuildType& to_build, int left_bound_inclusive,
                         int right_bound_exclusive) -> NodePtr {
      return std::make_shared<Node>(to_build, left_bound_inclusive, right_bound_exclusive);
    }

    static auto new_node(const NodePtr& node) -> NodePtr { return std::make_shared<Node>(*node); }

    // NOLINTNEXTLINE
    Node(int left_bound_inclusive, int right_bound_exclusive)
        : left_bound_inclusive(left_bound_inclusive),
          right_bound_exclusive(right_bound_exclusive) {}

    // NOLINTNEXTLINE
    Node(const ToBuildType& to_build, int left_bound_inclusive, int right_bound_exclusive)
        : left_bound_inclusive(left_bound_inclusive), right_bound_exclusive(right_bound_exclusive) {
      if (right_bound_exclusive - left_bound_inclusive == 1) {
        sum = to_build[left_bound_inclusive];
        return;
      }
      if (right_bound_exclusive - left_bound_inclusive == 0) {
        sum = 0;
        return;
      }
      int middle_bound = (left_bound_inclusive + right_bound_exclusive) / 2;
      left = new_node(to_build, left_bound_inclusive, middle_bound);
      right = new_node(to_build, middle_bound, right_bound_exclusive);
    }

    Node(const Node&) = default;
    Node(Node&&) = default;
    auto operator=(const Node&) -> Node& = default;
    auto operator=(Node&&) -> Node& = default;
  };

  using NodePtr = Node::NodePtr;
  using Versions = std::vector<NodePtr>;
  using ToBuildType = Node::ToBuildType;

 public:
  explicit PersistentSegmentTree(int count_versions, const ToBuildType& to_build)
      : count_of_elements(static_cast<int>(to_build.size())) {
    // versions.reserve(count_versions + 10);
    auto new_tree_root = Node::new_node(to_build, 0, static_cast<int>(to_build.size()));
    versions.push_back(new_tree_root);
  }

  PersistentSegmentTree() = default;

  auto is_half_interval_not_inside_node(const NodePtr& node_from, int left_bound_inclusive,
                                        int right_bound_exclusive) -> bool {
    return (right_bound_exclusive <= node_from->left_bound_inclusive) ||
           (left_bound_inclusive >= node_from->right_bound_exclusive);
  }

  auto is_half_interval_inside_node(const NodePtr& node, int left_bound_inclusive,
                                    int right_bound_exclusive) {
    return node->left_bound_inclusive >= left_bound_inclusive &&
           node->right_bound_exclusive >= right_bound_exclusive;
  }

 private:
  // NOLINTNEXTLINE
  auto add_recursive(int left_bound_inclusive, int right_bound_exclusive, int value,
                     const NodePtr& current_node) -> NodePtr {
    auto node_left_bound = current_node->left_bound_inclusive;
    auto node_right_bound = current_node->right_bound_exclusive;
    if (right_bound_exclusive <= node_left_bound || left_bound_inclusive >= node_right_bound) {
      return current_node;
    }
    if (right_bound_exclusive >= node_right_bound && left_bound_inclusive <= node_left_bound) {
      auto copy = Node::new_node(current_node);
      copy->sum += value;
      return copy;
    }
    auto left_copied_node =
        add_recursive(left_bound_inclusive, right_bound_exclusive, value, current_node->left);
    auto right_copied_node =
        add_recursive(left_bound_inclusive, right_bound_exclusive, value, current_node->right);

    auto root_copy = Node::new_node(current_node);
    root_copy->left = left_copied_node;
    root_copy->right = right_copied_node;
    return root_copy;
  }

 public:
  // NOLINTNEXTLINE
  void add(int left_bound_inclusive, int right_bound_exclusive, int value) {
    auto node_from = versions[versions.size() - 1];
    auto added_node = add_recursive(left_bound_inclusive, right_bound_exclusive, value, node_from);
    versions.push_back(added_node);
  }

 private:
  [[nodiscard]] auto at_recursive(int index, const NodePtr& current_node, int accumulator) const
      -> int {
    if (current_node->left == nullptr && current_node->right == nullptr) {
      return accumulator + current_node->sum;
    }
    int middle_bound =
        (current_node->left_bound_inclusive + current_node->right_bound_exclusive) / 2;
    if (index >= middle_bound) {
      return at_recursive(index, current_node->right, accumulator + current_node->sum);
    }
    return at_recursive(index, current_node->left, accumulator + current_node->sum);
  }

 public:
  [[nodiscard]] auto at(int index, int version) const -> int {
    if (index >= count_of_elements) {
      throw std::runtime_error("[PersistentSegmentTree] index out of bound");
    }
    if (version >= versions.size()) {
      throw std::runtime_error("[PersistentSegmentTree] index out of versions bound");
    }
    return at_recursive(index, versions[version], 0);
  }

  [[nodiscard]] auto at_last_version(int index) const -> int {
    return at(index, static_cast<int>(versions.size()) - 1);
  }

  [[nodiscard]] auto size() const -> int { return count_of_elements; }

  [[nodiscard]] auto get_last_version() const -> int {
    return static_cast<int>(versions.size()) - 1;
  }

 private:
  void printBT(const std::string& prefix, const NodePtr& node, bool is_left) {
    if (node != nullptr) {
      std::cout << prefix;
      std::cout << (is_left ? "├──" : "└──");
      std::cout << node->sum << std::endl;
      printBT(prefix + (is_left ? "│   " : "    "), node->left, true);
      printBT(prefix + (is_left ? "│   " : "    "), node->right, false);
    }
  }

 public:
  void print() {
    for (int current_version = 0; const auto& x : versions) {
      std::cout << "/////////////// VERSION " << current_version << "/////////////////////"
                << std::endl;
      for (int i = 0; i < count_of_elements; i++) {
        std::cout << at(i, current_version) << ' ';
      }
      std::cout << std::endl;
      std::cout << "//////////////////// TREE ////////////////////////" << std::endl;
      printBT("", x, false);
      current_version++;
    }
  }

 private:
  Versions versions;
  int count_of_elements;
};

class PersistentSegmentTreeSolution {
 public:
  void prepare_rectangles(const Rectangles& recs) {
    rectangles = recs;
    compress_rectangles();
    for (auto& rec : rectangles) {
      auto compressed_right_top = compress_point(rec.get_right_top_angle());
      auto compressed_left_bottom = compress_point(rec.get_left_bottom_angle());
      is_end[compressed_left_bottom.x].push_back(
          {false, compressed_left_bottom.y, compressed_right_top.y});
      is_end[compressed_right_top.x].push_back(
          {true, compressed_left_bottom.y, compressed_right_top.y});
      rec.set_left_bottom_angle(compressed_left_bottom);
      rec.set_right_top_angle(compressed_right_top);
    }
    std::sort(rectangles.begin(), rectangles.end(), [](const auto& lhs, const auto& rhs) {
      return lhs.get_left_bottom_angle().x < rhs.get_left_bottom_angle().x;
    });
    int max_x = static_cast<int>(compressed_x.size()) - 1;
    int max_y = static_cast<int>(compressed_y.size());
    tree = PersistentSegmentTree(max_x * 2, std::vector<int>(max_y, 0));
    for (const auto& [x, triples] : is_end) {
      for (const auto& [end, y1, y2] : triples) {
        if (end) {
          tree.add(y1, y2, -1);
          get_version_from_x[x] = tree.get_last_version();
          continue;
        }
        tree.add(y1, y2, 1);
        get_version_from_x[x] = tree.get_last_version();
      }
    }
#ifdef DEBUG
    tree.print();
#endif  // DEBUG
  }

  auto get_number_of_crossing(const Vec2d& query) -> int {
    auto query_compressed = compress_point(query);
#ifdef DEBUG
    std::cout << "get query: " << query << std::endl;
    std::cout << "compression performed: " << query_compressed << std::endl;
    std::cout << "tree size: " << tree.size() << std::endl;
    std::cout << "compressed_x: " << compressed_x.size() << ' ';
    for (const auto& x : compressed_x) {
      std::cout << x << ' ';
    }
    std::cout << std::endl;
#endif  // DEBUG
    return tree.at(query_compressed.y, get_version_from_x[query_compressed.x]);
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

 public:
  PersistentSegmentTreeSolution() = default;
  PersistentSegmentTreeSolution(const PersistentSegmentTreeSolution&) = default;
  PersistentSegmentTreeSolution(PersistentSegmentTreeSolution&&) = default;
  auto operator=(const PersistentSegmentTreeSolution&) -> PersistentSegmentTreeSolution& = default;
  auto operator=(PersistentSegmentTreeSolution&&) -> PersistentSegmentTreeSolution& = default;

  [[nodiscard]] auto get_name() -> std::string { return "PersistentSegmentTreeSolution"; }

 private:
  Rectangles rectangles;
  std::vector<int> compressed_x;
  std::vector<int> compressed_y;
  PersistentSegmentTree tree;
  std::map<int, std::vector<std::tuple<bool, int, int>>> is_end;
  std::unordered_map<int, int> get_version_from_x;
};

static_assert(Solution<PersistentSegmentTreeSolution>,
              "PersistentSegmentTreeSolution is not implement needed methods");
