///0
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
  int h, w; std::cin >> h >> w;
  
  std::vector<int> char_stat(26, 0);
  std::string n; std::cin >> n; 
  
  std::map<int, int> char_map;
  
  int id=1; // very strange, I set id=0, then the anser is wrong, 
  // there must be some leakage w.r.t. the indexing.
  
  for (auto iter=n.begin(); iter!=n.end(); ++iter){
    if (char_stat[int(*iter - 'A')]==0){
      char_map[int(*iter - 'A')]=id; ++id; // how many index of char required.
    } 
    ++char_stat[int(*iter - 'A')];
  }
  
  std::vector<std::vector<int>> pair_stat(26, std::vector<int> (26, 0));
  std::vector<std::vector<char>> front(h, std::vector<char> (w, 'a'));
  std::vector<std::vector<char>> back(h, std::vector<char> (w, 'a'));

  for (int i=0; i<h; ++i) for (int j=0; j<w; ++j) std::cin >> front[i][j];
  for (int i=0; i<h; ++i) for (int j=0; j<w; ++j) std::cin >> back[i][w-j-1];
  
  std::map<int, int> pair_map;
  
  for (int i=0; i<h; ++i) for (int j=0; j<w; ++j) {
    int f=int(front[i][j] - 'A'), b=int(back[i][j] - 'A');
    if ((pair_stat[f][b]==0 && pair_stat[b][f]==0) && (char_stat[f]>0 || char_stat[b]>0)){
      // if f or b in the char set, add a map to id, 
      // we set id to be i*26+j, with i>=j
      int key=(f>=b) ? 26*f+b:26*b+f;
      pair_map[key]=id; ++id;
    }
    ++pair_stat[f][b];
    if (b!=f) ++pair_stat[b][f];
  }
  
  graph G(id); edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  // int sum=0;
  for (auto iter=char_map.begin(); iter!=char_map.end(); ++iter){
    // add source --> char vertex 
    adder.add_edge(v_source, iter->second, char_stat[iter->first]); 
  }
  
  for (auto iter=pair_map.begin(); iter!=pair_map.end(); ++iter){
    int c1=(iter->first)/26, c2=(iter->first)%26;
    adder.add_edge(char_map[c1], iter->second, pair_stat[c1][c2]); 
    if (c2!=c1) adder.add_edge(char_map[c2], iter->second, pair_stat[c1][c2]); 
    // add edge to sink
    adder.add_edge(iter->second, v_sink, pair_stat[c1][c2]);
  }
 
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << ((flow==long(n.length())) ? "Yes" : "No") << std::endl;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for(int i=0; i<t; ++i) solve();
}