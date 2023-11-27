#include "nlgraph.h"

#include <fmt/core.h>
#include <fmt/ostream.h>

#include <boost/graph/directed_graph.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <ranges>

using namespace boost;

template <class Graph>
struct exercise_vertex {
  exercise_vertex(Graph &g_, const char name_[]) : g(g_), name(name_) {}
  using Vertex = typename graph_traits<Graph>::vertex_descriptor;

  void operator()(const Vertex &v) const {
    using namespace boost;
    typename property_map<Graph, vertex_index_t>::type vertex_id = get(vertex_index, g);

    fmt::print("vertex: {}\n", name[get(vertex_id, v)]);

    // Write out the outgoing edges
    fmt::print("\tout-edges: ");

    typename graph_traits<Graph>::out_edge_iterator out_i, out_end;
    typename graph_traits<Graph>::edge_descriptor e;

    for (auto [out_i, out_end] = out_edges(v, g); out_i != out_end; ++out_i) {
      e = *out_i;
      Vertex src = source(e, g), targ = target(e, g);
      fmt::print("({},{}) ", name[get(vertex_id, src)], name[get(vertex_id, targ)]);
    }

    fmt::print("\n");

    // Write out the incoming edges
    fmt::print("\tin-edges: ");

    typename graph_traits<Graph>::in_edge_iterator in_i, in_end;

    for (auto [in_i, in_end] = in_edges(v, g); in_i != in_end; ++in_i) {
      e = *in_i;
      Vertex src = source(e, g), targ = target(e, g);
      fmt::print("({},{}) ", name[get(vertex_id, src)], name[get(vertex_id, targ)]);
    }

    fmt::print("\n");

    // Write out all adjacent vertices
    fmt::print("\tadjacent vertices: ");
    typename graph_traits<Graph>::adjacency_iterator ai, ai_end;
    for (auto [ai, ai_end] = adjacent_vertices(v, g); ai != ai_end; ++ai) {
      fmt::print("{} ", name[get(vertex_id, *ai)]);
    }
    fmt::print("\n");
  }

  Graph &g;
  const char *name;
};

int main(int /*unused*/, char * /*unused*/[]) {
  // create a typedef for the Graph type
  using Graph =
      adjacency_list<vecS, vecS, bidirectionalS, no_property, property<edge_weight_t, float>>;

  // Make convenient labels for the vertices
  enum { A, B, C, D, E, N };
  const int num_vertices = N;
  const char name[] = "ABCDE";

  // writing out the edges in the graph
  using Edge = std::pair<int, int>;
  std::array edge_array{
      Edge(A, B), Edge(A, D), Edge(C, A), Edge(D, C), Edge(C, E), Edge(B, D), Edge(D, E),
  };

  const auto num_edges = edge_array.size();

  // average transmission delay (in milliseconds) for each connection
  std::array transmission_delay{1.2, 4.5, 2.6, 0.4, 5.2, 1.8, 3.3, 9.1};

  // declare a graph object, adding the edges and edge properties
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ can't handle the iterator constructor
  Graph g(num_vertices);
  property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
  for (std::size_t j = 0; j < num_edges; ++j) {
    graph_traits<Graph>::edge_descriptor e;
    bool inserted;
    auto [e, inserted] = add_edge(edge_array[j].first, edge_array[j].second, g);
    weightmap[e] = transmission_delay[j];
  }
#else
  Graph g(edge_array.begin(), edge_array.end(), transmission_delay.begin(), num_vertices);
#endif

  boost::property_map<Graph, vertex_index_t>::type vertex_id = get(vertex_index, g);
  boost::property_map<Graph, edge_weight_t>::type trans_delay = get(edge_weight, g);

  fmt::print("vertices(g) = ");
  typedef graph_traits<Graph>::vertex_iterator vertex_iter;
  std::pair<vertex_iter, vertex_iter> vp;
  for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
    fmt::print("{} ", name[get(vertex_id, *vp.first)]);
  }
  fmt::print("\n");

  fmt::print("edges(g) = ");
  graph_traits<Graph>::edge_iterator ei, ei_end;
  for (auto [ei, ei_end] = edges(g); ei != ei_end; ++ei) {
    fmt::print("({},{}), ", name[get(vertex_id, source(*ei, g))],
               name[get(vertex_id, target(*ei, g))]);
  }

  fmt::print("\n");

  std::for_each(vertices(g).first, vertices(g).second, exercise_vertex<Graph>(g, name));

  std::map<std::string, std::string> graph_attr, vertex_attr, edge_attr;
  graph_attr["size"] = "3,3";
  graph_attr["rankdir"] = "LR";
  graph_attr["ratio"] = "fill";
  vertex_attr["shape"] = "circle";

  boost::write_graphviz(std::cout, g, make_label_writer(name), make_label_writer(trans_delay),
                        make_graph_attributes_writer(graph_attr, vertex_attr, edge_attr));

  return 0;
}

auto load_nlgraph(std::string_view filename) {
  std::ifstream ifs(filename);
  json j = json::parse(ifs);
  return j;
}

int main2(int argc, char **argv) {
  auto graph = load_nlgraph(argv[1]);
  fmt::print("graph: {}\n", graph.dump(4));

  return 0;
}
