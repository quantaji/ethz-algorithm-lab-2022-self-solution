///3
#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map; // this is one way for definint weight
typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;
typedef std::tuple<int, int, int> cust_edge;

struct Greater {
  inline bool operator() (const cust_edge& e1, const cust_edge& e2) {
    return std::get<2>(e1) > std::get<2>(e2);
  }
};

void solve() {
  int n, e, s, a, b;
  std::cin >> n >> e >> s >> a >> b;
  std::vector<weighted_graph> G(s, weighted_graph(n));
  weighted_graph F(n); // F is the graph used for final calculation
  for (int i=0; i<e; ++i){
    int t1, t2; std::cin >> t1 >> t2;
    boost::add_edge(t1, t2, EdgeWeightProperty(2e6), F);
    for (int j=0; j<s; ++j) {
      int w; std::cin >> w;
      boost::add_edge(t1, t2, EdgeWeightProperty(w), G[j]);
    }
  }
  std::vector<int> h(n);
  for (int i=0; i<s; ++i) std::cin >> h[i];
  
  // std::vector<std::vector<edge_desc>> accessable_list(n, std::vector<edge_desc> ());
  for (int i=0; i<s; ++i) {
    // iterate over each species
    std::priority_queue<cust_edge, std::vector<cust_edge>, Greater> task;
    out_edge_it it, end;
    weight_map weight =  get(boost::edge_weight, G[i]);
    for (boost::tie(it, end) = boost::out_edges(h[i], G[i]); it!=end; ++it){
      int source=boost::source(*it, G[i]), target=boost::target(*it, G[i]), w=weight(*it);
      task.push(cust_edge(source, target, w));
    }
    std::vector<bool> visited(n, false); visited[h[i]]=true;
    while (!task.empty()){
      cust_edge cur = task.top(); task.pop();
      int source=std::get<0>(cur), target=std::get<1>(cur), wweight=std::get<2>(cur);
      if (visited[target]) continue; else {
        // current edge is accessable, then do comparison
        visited[target] = true; // first mark this
        // compare with F
        if (weight(boost::edge(source, target, F).first) > wweight) weight(boost::edge(source, target, F).first) = wweight;
        // add its other edge to priority que
        for (boost::tie(it, end) = boost::out_edges(target, G[i]); it!=end; ++it){
          int n_s=boost::source(*it, G[i]), n_t=boost::target(*it, G[i]), w=weight(*it);
          if (!visited[n_t]) task.push(cust_edge(n_s, n_t, w));
        }
      }
    }
  }
  
  // now perform dijkstra on F
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(F, a, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, F))));
  std::cout << dist_map[b] << std::endl;
  
}

int main() {
  int t;
  std::cin >> t;
  for (int i=0; i<t; ++i) {
    solve();
  }
}