#include <fmt/core.h>
#include <fmt/ostream.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

auto load_nlgraph(std::string_view filename) {
  std::ifstream ifs(filename);
  json j = json::parse(ifs);
  return j;
}

int main(int argc, char **argv) {
  auto graph = load_nlgraph(argv[1]);
  fmt::print("graph: {}\n", graph.dump(4));
  return 0;
}
