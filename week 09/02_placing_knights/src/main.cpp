#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, int capacity) {
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

// idea
// we need maximal independent set (MaxIS), we can instead calculate the Min Vertex Cover (MinVC)
// we can transform the problem into Bipartite graph so that the new MinVC is twice the number of original problem

void solve() {
  int n; std::cin >> n;
  int sum = 0;
  std::vector<std::vector<bool>> avail(n, std::vector<bool> (n, false));
  for (int i=0; i<n; ++i) for(int j=0; j<n; ++j){
    int available; std::cin >> available;
    avail[i][j] = (available == 1);
    sum += available;
  }
  
  int pn=8; // placement number
  std::vector<int> di = {-1, -1, 1, 1, -2, -2, 2, 2};
  std::vector<int> dj = {-2, 2, -2, 2, -1, 1, -1, 1};
  
  // add inter edge
  graph G(2*n*n); edge_adder adder(G);
  for (int i=0; i<n; ++i) for (int j=0; j<n; ++j) if (avail[i][j]){
    for (int k=0; k<pn; ++k){
      int ip=i+di[k], jp=j+dj[k];
      if ((ip>=0) && (ip<n) && (jp>=0) && (jp<n) && (avail[ip][jp])){
        adder.add_edge(i*n+j, n*n+ip*n+jp, 1);
      }
    }
  }
  
  // add source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  for (int i=0; i<n; ++i) for (int j=0; j<n; ++j) if (avail[i][j]) {
    adder.add_edge(v_source, i*n+j, 1);
    adder.add_edge(n*n + i*n+j, v_sink, 1);
  }
  
  int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  // flow = max matching = min vertex cover
  std::cout << sum - flow/2 << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}