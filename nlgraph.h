#ifndef NLGRAPH_H
#define NLGRAPH_H
// https://www.boost.org/doc/libs/1_82_0/libs/graph/doc/quick_tour.html

#include <fmt/core.h>

#include <boost/graph/directed_graph.hpp>
#include <ranges>

namespace nlgraph {

class Variation {
 public:
  enum Value { INV, DUP, TRA, UNK };

  Variation() = default;

  [[nodiscard]] Value value() const { return value_; }

  [[nodiscard]] std::string to_string() const {
    switch (value_) {
      case INV:
        return "INV";
      case DUP:
        return "DUP";
      case TRA:
        return "TRA";
      case UNK:
        return "UNK";
    }
  }
  [[nodiscard]] bool is_inv() const { return value_ == INV; }
  [[nodiscard]] bool is_dup() const { return value_ == DUP; }
  [[nodiscard]] bool is_tra() const { return value_ == TRA; }
  [[nodiscard]] bool is_unk() const { return value_ == UNK; }

 private:
  Value value_{UNK};
};

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

template <typename T>
  requires std::is_arithmetic_v<T> && std::default_initializable<T>
class Interval {
 public:
  Interval() = default;
  Interval(T start, T end) : start_(start), end_(end) {}
  [[nodiscard]] T start() const { return start_; }
  [[nodiscard]] T end() const { return end_; }
  [[nodiscard]] T length() const { return end_ - start_; }
  [[nodiscard]] bool contains(T pos) const { return pos >= start_ && pos <= end_; }
  [[nodiscard]] bool contains(const Interval &other) const {
    return contains(other.start_) && contains(other.end_);
  }
  [[nodiscard]] bool overlaps(const Interval &other) const {
    return contains(other.start_) || contains(other.end_);
  }

  [[nodiscard]] bool operator==(const Interval &other) const {
    return start_ == other.start_ && end_ == other.end_;
  }
  [[nodiscard]] bool operator!=(const Interval &other) const { return !(*this == other); }
  [[nodiscard]] bool operator<(const Interval &other) const {
    return start_ < other.start_ || (start_ == other.start_ && end_ < other.end_);
  }
  [[nodiscard]] bool operator>(const Interval &other) const {
    return start_ > other.start_ || (start_ == other.start_ && end_ > other.end_);
  }
  [[nodiscard]] bool operator<=(const Interval &other) const { return !(*this > other); }
  [[nodiscard]] bool operator>=(const Interval &other) const { return !(*this < other); }

  [[nodiscard]] std::string to_string() const { return fmt::format("[{}, {}]", start_, end_); }
  [[nodiscard]] Interval intersection(const Interval &other) const {
    if (overlaps(other)) {
      return Interval(std::max(start_, other.start_), std::min(end_, other.end_));
    }
    return Interval();
  }

  [[nodiscard]] Interval union_(const Interval &other) const {
    if (overlaps(other)) {
      return Interval(std::min(start_, other.start_), std::max(end_, other.end_));
    }
    return Interval();
  }

  [[nodiscard]] bool is_valid() const { return start_ <= end_; }

  [[nodiscard]] Interval extend(T amount) const { return Interval(start_ - amount, end_ + amount); }
  [[nodiscard]] Interval extend_start(T amount) const { return Interval(start_ - amount, end_); }
  [[nodiscard]] Interval extend_end(T amount) const { return Interval(start_, end_ + amount); }

 private:
  T start_{};  // inclusive
  T end_{};    // exclusive
};

struct NodeData {
  std::string name{};
  Strand strand{};
  std::string chr{};
  Interval<int> interval{};
  std::vector<Interval<int>> exons{};
};

struct EdgeData {
  std::string name{};
  std::string source{};
  std::string target{};
  Variation variation{};
};

// class nlgraph : boost::directed_graph<> {};
using nlgraph = boost::directed_graph<NodeData, EdgeData>;

void k_means(const nlgraph &g, const std::string &filename, int clusters);

}  // namespace nlgraph
#endif
