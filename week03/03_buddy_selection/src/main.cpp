///3
///STL includes
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

//BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

void solve(){
  int n, c; long unsigned int f;
  std::cin >> n >> c >> f;
  std::vector<std::set<int>> hobbies (n, std::set<int> {});
  std::map<std::string, int> dict; // use a map to replace string with int, to accelerate calculation
  int index = 0;
  for (int i=0; i<n; ++i) {
    for (int j=0; j<c; ++j){
      std::string temp; std::cin >> temp;
      if (dict.find(temp) == dict.end()){
        dict.insert(std::pair<std::string, int> (temp, index));
        ++index;
      }
      hobbies[i].insert(dict[temp]);
    }
  }
  graph G(n);
  for (int i=1; i<n; ++i) for (int j=0; j<i; ++j){
    std::set<int> temp {};
    std::set_intersection(hobbies[i].begin(), hobbies[i].end(), hobbies[j].begin(), hobbies[j].end(), std::inserter(temp, temp.begin()));
    // std::cout << int(temp.size()) - int(f) << ' ';
    if (temp.size() > f) boost::add_edge(i, j, G);
  }
  
  std::vector<vertex_desc> mate_map(n);  // exterior property map
  // const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();
  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  
  // std::cout << matching_size << ' ' << n/2 << " --- ";
  if (matching_size < n/2) std::cout << "optimal" << std::endl;
  else std::cout << "not optimal" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i=0; i<t; ++i){
    solve();
  }
}