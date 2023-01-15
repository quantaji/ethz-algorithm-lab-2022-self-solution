///1

// idea

// this problem can be transformed into a bipartite tree problem
// first, we make a stat about all shelter and agent position, and we get a matrix of SxA of the shortest path amoung all of this
// time complexity is O(nlog n + m) xa
// then we create a second bipartite graph,
// one side is all shelter 
// the other side is all agent (if two are one same position, create two node)
// if a shelter have 2 capacity, we duplicate this shelter
// this bipartite graph will have at most 2*a*s edge, each edge with weight d+shortest path, 
// if a shelter have a second capacity, on the second shelter, make the weight as 2d+shortest path
// to test the least possible solution, we can make a threshold on the weight, sort all edges according to the wegiht, 
// and if we can make the total flow equal to a using only edges of weight less than or equal to w, then the minimax solution is w
// this can be done in a binary search

// stl
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <limits>
// boost
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
// flow
#include <boost/graph/push_relabel_max_flow.hpp>

typedef std::vector<int> IV;
typedef std::set<int> IS;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor          vertex_desc;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> f_graph; // flow graph
typedef traits::vertex_descriptor f_vertex_desc;
typedef traits::edge_descriptor f_edge_desc;

class edge_adder {
  f_graph &G;

 public:
  explicit edge_adder(f_graph &G) : G(G) {}

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
  int n, m, a, s, c, d; std::cin >> n >> m >> a >> s >> c >> d;
  
  // now construct the graph
  graph G(n); 
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc e;
  for (int i=0; i<m; ++i){
    char w; int x, y, z; std::cin >> w >> x >> y >> z;
    e=boost::add_edge(x, y, G).first; weights[e]=z;
    if (w=='L'){
      e=boost::add_edge(y, x, G).first; weights[e]=z;
    }
  }
  
  IV a_list, s_list; IS a_set, s_set;
  for(int i=0; i<a; ++i){
    int a_i; std::cin >> a_i;
    a_list.push_back(a_i);
    a_set.insert(a_i);
  }
  for(int i=0; i<s; ++i){
    int s_i; std::cin >> s_i;
    s_list.push_back(s_i);
    s_set.insert(s_i);
  }
  
  std::map<int, int> as_sp; // agent to shelter shortest path
  
  for (auto i=a_set.begin(); i!=a_set.end(); ++i){
    IV dist_map(n);
    boost::dijkstra_shortest_paths(G, *i,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
    for (auto j=s_set.begin(); j!=s_set.end(); ++j){
      int id=(*j)*n + (*i);
      as_sp[id]=dist_map[*j];
      // std::cout << dist_map[*j] << ' ' << (dist_map[*j] == std::numeric_limits<int>::max()) << std::endl;
      // it turns out that boost use numeric limits to store the shortest path's infinite
    }
  }
  
  // now we construct the bipartite graph, before constructing, we should first get a ordered list of edges
  // we do this by a multimap, map total time to (a,s) pair
  std::multimap<int, int> tas;
  IS t_set; // a temporary place to store all value of 
  for (int i=0; i<a; ++i){
    for (int j=0; j<s; ++j){
      int weight_id=s_list[j]*n+a_list[i];
      if (as_sp[weight_id]<std::numeric_limits<int>::max()){
        int id1=(j)*a+i;
        tas.insert(std::pair<int, int> (as_sp[weight_id]+d, id1));
        t_set.insert(as_sp[weight_id]+d);
        if (c==2){
          int id2=(s+j)*a+i;
          tas.insert(std::pair<int, int> (as_sp[weight_id]+2*d, id2));
          t_set.insert(as_sp[weight_id]+2*d);
        }
      }
    }
  }

  IV t_list;
  for (auto iter=t_set.rbegin(); iter!=t_set.rend(); ++iter) t_list.push_back(*iter); // we do this reversely to make convenience for binary search
  
  // now begin binary search
  int left=0, right=t_set.size();
  while(left<right){
    int mid=(left+right)/2; // left is always possible while right is always impossible
    
    f_graph fG((c==2)? a+2*s:a+s);
    const f_vertex_desc v_source = boost::add_vertex(fG);
    const f_vertex_desc v_sink = boost::add_vertex(fG);
    edge_adder adder(fG);
    
    for (int i=0; i<a; ++i) adder.add_edge(v_source, i, 1);
    for (int i=0; i<((c==2)?2*s:s); ++i) adder.add_edge(a+i, v_sink, 1);
    
    for (auto iter=tas.begin(); iter!=tas.upper_bound(t_list[mid]); ++iter){
      int agent=(iter->second)%a;
      int shelter=(iter->second)/a;
      adder.add_edge(agent, a+shelter, 1);
    }
    
    int flow = boost::push_relabel_max_flow(fG, v_source, v_sink);
    if (flow == a){
      // means this value is possible
      left=mid;
    } else {
      right=mid;
    }
    
    if (left+1==right) break;
  }
  
  std::cout << t_list[left] << std::endl;
  
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}