// STL
#include <iostream>
#include <set>
#include <algorithm>
// BGL
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

struct bi_comp_t {
  typedef boost::edge_property_tag kind;
} bi_comp;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, 
  boost::no_property, boost::property<bi_comp_t, std::size_t>> Graph;
// typedef boost::graph_traits<Graph>::vertex_descriptor vert_desc;
typedef boost::graph_traits<Graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<Graph>::edge_iterator    edge_it;
typedef boost::property_map<Graph, bi_comp_t>::type Bi_component_map;

// given a graph G, if we calculate all of its biconnected component (maximal biconnected graph)
// Prop 1, every bi-comp A B does not share more than 1 vertex, or any edge (=two vertices), otherwise, the union of two component would form a bigger bi-con graph
// every edge would either be in a bi-comp, or stay isolated
// if we shrink all vertices in one bi-comp into one single point, the graph then become a new smaller graph
// this graph is actually a tree, otherwise, there will be cycles and then it wold form bigger bi-comp

// therefore, the critical edge is those remain in the the shrinked graph. 
// If we admit a single edge can also be bi-connected, then we are searching for edges connect to two articulation points

void solve() {
  int n, m;
  std::cin >> n >> m;
  Graph G(n);
  for (int i=0; i<m; ++i){
    int e1, e2; std::cin >> e1 >> e2;
    boost::add_edge(e1, e2, G);
  }
  Bi_component_map bicomp = boost::get(bi_comp, G);
  int num_components = boost::biconnected_components(G, bicomp);
  std::vector<std::vector<edge_desc>> component_list(num_components, std::vector<edge_desc> ());
  
  // iterate over all edges and check single edge components
  edge_it iter, end;
  for (boost::tie(iter, end) = boost::edges(G); iter!=end; ++iter){
    component_list[bicomp(*iter)].push_back(*iter);
  }
  std::set<int> results {};
  for (auto it=component_list.begin(); it!=component_list.end(); ++it) if (it->size() == 1){
    edge_desc e = it->front();
    int v1=boost::target(e, G), v2=boost::source(e, G);
    results.insert(std::min(v1*n+v2, v2*n+v1));
  }
  std::cout << results.size() << std::endl;
  if (!results.empty()) for (auto it=results.begin(); it!=results.end(); ++it) {
    int v1 = (*it)/n, v2=(*it)%n;
    std::cout << v1 << ' ' << v2 << std::endl;
  }
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}