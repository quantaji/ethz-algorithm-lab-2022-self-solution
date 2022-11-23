///5
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
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, int> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef traits::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, int capacity, int cost) {
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
  int s_begin=1; // an offset
  int n, m, s; std::cin >> n >> m >> s;
  int m_begin = s_begin+s;
  int n_begin = m_begin+m;
  int num_v = n_begin+n;
  
  graph G(num_v); edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  // add state
  for (int i=0; i<s; ++i){
    int l; std::cin >> l;
    adder.add_edge(v_source, s_begin+i, l, 0);
  }
  // add site
  for (int i=0; i<m; ++i){
    int s_i; std::cin >> s_i;
    adder.add_edge(s_begin+s_i-1, m_begin+i, 1, 0);
  }
  // add bidding
  for (int i=0; i<n; ++i) for (int j=0; j<m; ++j){
    int bij; std::cin >> bij;
    if (bij>5) adder.add_edge(m_begin+j, n_begin+i, 1, 100-bij);
  }
  // add sink
  for (int i=0; i<n; ++i){
    adder.add_edge(n_begin+i, v_sink, 1, 0);
  }
  
  int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  // boost::cycle_canceling(G);
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost = 100*flow - boost::find_flow_cost(G);
  std::cout << flow << ' ' << cost << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}