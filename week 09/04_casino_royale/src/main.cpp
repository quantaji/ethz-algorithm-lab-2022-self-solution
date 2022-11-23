///0
// idea
// the number of l is fixed along the mission
// initially we might think this must be handles by flow capacity
// but instead
// we can alternatively transform this constrain, into the condition that, no l 
// of agents can be together
// or, we can imagine l different lines across the line, and the agent is teleported to next station ant get into the train
// but there will be 2*n + 2 <= 600 vertex
// and the maximum number of edges are (m+n)

// to make sure all edges are non negative, and the solution we get is valid
// we have to make sure, all l path have a initial cost of n*max_c, and each 
// task edge have (ty - tx)*max_c - priority
#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

void solve(){
  int n, m, l; std::cin >> n >> m >> l;
  int offset = 1, C_MAX=1<<11, CAP_MAX=114514;
  graph G(offset+2*n); edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  // add source and sink
  adder.add_edge(v_source, offset, l, 0);
  adder.add_edge(offset+n-1, v_sink, l, 0);
  
  for (int i=0; i<n; ++i){
    // add layer 2 to layer 1
    adder.add_edge(offset+n+i, offset+i, CAP_MAX, 0);
    if (i>0){
      adder.add_edge(offset+i-1, offset+i, CAP_MAX, C_MAX);
    }
  }
  
  for (int i=0; i<m; ++i){
    int x, y, q; std::cin >> x >> y >> q;
    adder.add_edge(offset+x, offset+n+y, 1, (y-x)*C_MAX - q);
  }
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int max_priority = l*(n-1)*C_MAX - boost::find_flow_cost(G);
  std::cout << max_priority << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}