///3
#include <iostream>
#include <string>
#include <vector>
// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};


void solve(){
  int h, w; std::cin >> w >> h;
  std::vector<std::vector<bool>> tile (h, std::vector<bool> (w, false));
  
  int sum=0; std::string temp;
  for (int i=0; i<h; ++i){
    std::cin >> temp;
    for (int j=0; j<w; ++j){
      if (temp[j]=='.'){
        tile[i][j] = true;
        ++sum;
      } else tile[i][j] = false;
    }
  }
  
  if (sum%2 != 0) std::cout << "no" << std::endl;
  else {
    graph G(2*h*w); edge_adder adder(G); // upper and lower layer are linked by possible tiles
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    for (int i=0; i<h; ++i) for(int j=0; j<w; ++j) if (tile[i][j]){
      // add source and sink
      adder.add_edge(v_source, i*w+j, 1);
      adder.add_edge(h*w+i*w+j, v_sink, 1);
    }
    // check vertical
    for (int i=0; i<h-1; ++i) for(int j=0; j<w; ++j) if (tile[i][j] && tile[i+1][j]){
      adder.add_edge(i*w+j, h*w+(i+1)*w+j, 1);
      adder.add_edge((i+1)*w+j, h*w+i*w+j, 1);
    }
    // check horizontal
    for (int i=0; i<h; ++i) for(int j=0; j<w-1; ++j) if (tile[i][j] && tile[i][j+1]){
      adder.add_edge(i*w+j, h*w+i*w+j+1, 1);
      adder.add_edge(i*w+j+1, h*w+i*w+j, 1);
    }
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    std::cout << ((flow ==long(sum)) ? "yes" : "no") << std::endl;
  }
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  for(int i=0; i<n; ++i) solve();
}