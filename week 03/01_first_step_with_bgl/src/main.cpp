#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, EdgeWeightProperty> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

void solve() {
  int n, m;
  std::cin >> n >> m;
  weighted_graph G(n);
  
  for (int i=0; i<m; ++i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    boost::add_edge(u, v, EdgeWeightProperty(w), G);
  }
  // store tree relation in mst
  std::vector<edge_desc> mst;
 // calculating the MST 
  boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map = get(boost::edge_weight, G);
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  int total_weight = 0;
  for (auto it=mst.begin();  it!= mst.end(); ++it){
    total_weight += weight_map(*it); // this is the third way
  }
  
  // calculating distance
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, 0, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));
  int max_dis = 0;
  for (int i=0; i<n; ++i) if (max_dis < dist_map[i]) max_dis = dist_map[i];
  std::cout << total_weight << ' ' << max_dis << std::endl;
}

int main(){
  int t;
  std::cin >> t;
  for (int i=0; i<t; ++i) {
    solve();
  }
}