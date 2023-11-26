#ifndef NLGRAPH_H
#define NLGRAPH_H
// https://www.boost.org/doc/libs/1_82_0/libs/graph/doc/quick_tour.html

#include <fmt/core.h>

#include <boost/graph/directed_graph.hpp>
#include <ranges>

namespace nlgraph {

class Strand {
 public:
  enum Value { Forward, Reverse };

  Strand() = default;
  explicit Strand(Value value) : value_(value) {}

  [[nodiscard]] Value value() const { return value_; }
  [[nodiscard]] std::string to_string() const {
    switch (value_) {
      case Forward:
        return "Forward";
      case Reverse:
        return "Reverse";
    }
  }

  [[nodiscard]] Strand reverse() const { return Strand(value_ == Forward ? Reverse : Forward); }
  [[nodiscard]] bool is_forward() const { return value_ == Forward; }
  [[nodiscard]] bool is_reverse() const { return value_ == Reverse; }

 private:
  Value value_{Forward};
};

struct NodeData {
  std::string name{};
  Strand strand{};
};

struct EdgeData {
  std::string name{};
  std::string source{};
  std::string target{};
};

class nlgraph : boost::directed_graph<> {};

void k_means(const nlgraph &g, const std::string &filename, int clusters);

}  // namespace nlgraph
#endif
