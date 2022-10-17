///4
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::property_map<Graph, boost::edge_weight_t>::type weight_map; // this is one way for definint weight
typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::graph_traits<Graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<Graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<Graph>::out_edge_iterator         out_edge_it;

// Shame, hinted by others!!! SHAME!!!

void solve() {
  int n, m, k, x, y; std::cin >> n >> m >> k >> x >> y;
  int K = 15; // reducing this can reduce the amount of time
  Graph G(n*(K+1));
  for (int i=0; i<m; ++i){
    int a, b, c, d; std::cin >> a >> b >> c >> d;
    if (d == 1) for (int i=0; i<=K; ++i){
      if (i<K){
        boost::add_edge((K+1)*a+i, (K+1)*b+i+1, EdgeWeightProperty(c), G);
        boost::add_edge((K+1)*b+i, (K+1)*a+i+1, EdgeWeightProperty(c), G);
        // and additional edges to jump back
        boost::add_edge((K+1)*b+i+1, (K+1)*a+i,  EdgeWeightProperty(c), G);
        boost::add_edge((K+1)*a+i+1, (K+1)*b+i, EdgeWeightProperty(c), G);
      }
      // and additional edges to jump back
      boost::add_edge((K+1)*a+i, (K+1)*b+i, EdgeWeightProperty(c), G);
      boost::add_edge((K+1)*b+i, (K+1)*a+i, EdgeWeightProperty(c), G);
    }
    else for (int i=0; i<=K; ++i){
      boost::add_edge((K+1)*a+i, (K+1)*b+i, EdgeWeightProperty(c), G);
      boost::add_edge((K+1)*b+i, (K+1)*a+i, EdgeWeightProperty(c), G);
    }
    // this might be right or wrong
    // if (d==1) {
    //   for (int i=0; i<K; ++ i) for (int j=0; j<K; ++k){
    //     if (i > j){
    //       boost::add_edge(K*a + i, K*b+j,EdgeWeightProperty(c), G);
    //       if (i == j + 1) boost::add_edge(K*b+j, K*a + i, EdgeWeightProperty(c), G);
    //     }
    //     if (i < j){
    //       boost::add_edge(K*b+j, K*a + i,EdgeWeightProperty(c), G);
    //       if (j == i + 1) boost::add_edge(K*a+i, K*b+j,EdgeWeightProperty(c), G);
    //     }
    //   }
    // } else {
    //   for (int i=0; i<K; ++ i) boost::add_edge(K*a+i, K*b+i,EdgeWeightProperty(c), G);
    // }
  }
  std::vector<int> dist_map(n*(K+1));
  boost::dijkstra_shortest_paths(G, x*(K+1), boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));
  std::cout << dist_map[y*(K+1)+k] << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}