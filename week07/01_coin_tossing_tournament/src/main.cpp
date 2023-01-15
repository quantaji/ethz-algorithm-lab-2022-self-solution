///3
#include <iostream>
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
  int n, m; std::cin >> n >> m;
  
  std::vector<int> uncertainty (n, 0);
  std::vector<std::pair<int, int>> unk_list;
  
  for (int i=0; i<m; ++i){
    int a, b, c; std::cin >> a >> b >> c;
    if (c==0) unk_list.push_back(std::pair<int, int> (a, b));
    else if (c==1) --uncertainty[a]; 
    else if (c==2) --uncertainty[b]; 
  }
  bool possible = true; int sum=0;
  for (int i=0; i<n; ++i){
    int s_i; std::cin >> s_i;
    // std::cout << uncertainty[i] << " " ;
    uncertainty[i] += s_i; sum+=s_i;
    
    // if there are already wrong from the know game, then it is impossible.
    if (uncertainty[i] < 0) possible=false; 
    // std::cout << uncertainty[i] << " " ;
  }
  
  // check if the total score is the total number of match
  if (sum!=m) possible=false;
  
  unsigned long flow=0;
  if (possible){
     // [0,n) for player, [n~n+unk_list.size()) for games
    graph G(n+unk_list.size()); edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    // add to destination
    for (int i=0; i<n; ++i)if (uncertainty[i]>0) {
      adder.add_edge(i, v_sink, uncertainty[i]); 
    }
    // add sorce to game and game to player
    for (std::size_t i=0; i<unk_list.size(); ++i){
      // add source to game
      adder.add_edge(v_source, n+i, 1);
      // add game to player
      adder.add_edge(n+i, unk_list[i].first, 1);
      adder.add_edge(n+i, unk_list[i].second, 1);
    }
    flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    if (flow != unk_list.size()) possible=false;
  }
  std::cout << ( possible ? "yes" : "no") << std::endl;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}