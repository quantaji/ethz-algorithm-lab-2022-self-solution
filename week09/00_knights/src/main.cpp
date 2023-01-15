///3
#include <iostream>
#include <string>
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
  int m,n,k,c; std::cin >> m >> n >> k >> c;
  int offset=4;
  // x + m*y is the in_cell, n*m + m*y+x is the out cell
  
  graph G(offset + 2*n*m); edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  for (int i=0; i<m; ++i) for (int j=0; j<n; ++j){
    if (i<m-1){ // horizontal
      adder.add_edge(offset+n*m+m*j+i, offset+m*j+i+1, 1); // left to right
      adder.add_edge(offset+n*m+m*j+i+1, offset+m*j+i, 1); // right to left
    }
    if (j<n-1){ // vertical
      adder.add_edge(offset+n*m+m*j+i, offset+m*(j+1)+i, 1); // up to down
      adder.add_edge(offset+n*m+m*(j+1)+i, offset+m*j+i, 1); // down to up
    }
    // in to out
    adder.add_edge(offset+m*j+i, offset+n*m+m*j+i, c); // left to right
  }
  
  // add out
  for (int i=0; i<m; ++i){
     adder.add_edge(offset+n*m+m*0+i, v_sink, 1);
     adder.add_edge(offset+n*m+m*(n-1)+i, v_sink, 1);
  }
  for (int j=0; j<n; ++j){
     adder.add_edge(offset+n*m+m*j+0, v_sink, 1);
     adder.add_edge(offset+n*m+m*j+m-1, v_sink, 1);
  }
  
  // add knight
  for (int i=0; i<k; ++i){
    int x, y; std::cin >> x >> y;
    adder.add_edge(v_source, offset+m*y+x, 1);
  }
  if (n*m == 0){
    std::cout << 0 << std::endl;
  } else {
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    std::cout << flow << std::endl; 
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for(int i=0; i<t; ++i) solve();
}