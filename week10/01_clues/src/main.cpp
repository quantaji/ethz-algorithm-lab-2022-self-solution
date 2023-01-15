///4
// STL
#include <iostream>
#include <vector>
// #include <tuple>
#include <algorithm>
// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
// BGL
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/bipartite.hpp>

typedef std::size_t                                             Index;
typedef CGAL::Exact_predicates_inexact_constructions_kernel     K;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>    Vb;
typedef CGAL::Triangulation_face_base_2<K>                      Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>             Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                   Delaunay;
typedef std::pair<K::Point_2,Index>                             IPoint;
typedef Delaunay::Edge_iterator                                 Edge_iterator;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>   graph;
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator               edge_it;

double d2(const K::Point_2 &a, const K::Point_2 &b){
  return (a.x()-b.x())*(a.x()-b.x()) + (a.y()-b.y())*(a.y()-b.y());
}

struct Compare {
  bool operator() (IPoint a, IPoint b) {
    return a.first.x() < b.first.x();
  }
};

void solve(){
  int n, m; double r; std::cin >> n >> m >> r;
  
  std::vector<IPoint> points; points.reserve(n);
  for (int i=0; i<n; ++i){
    int x, y; std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }
  
  // then sort the points along some direction
  Delaunay t; t.insert(points.begin(), points.end());
  
  std::sort(points.begin(), points.end(), Compare());
  
  graph G(n);
  
  int i2max = 1;
  for (int i1=0; i1<n-1; ++i1) {
    while (i2max < n && std::abs(points[i2max].first.x() - points[i1].first.x()) <= r){
      ++i2max;
    }
    
    for (int i2=i1+1; i2<i2max; ++i2){
      double dsq = d2(points[i1].first, points[i2].first);
      if (dsq <= r*r) {
        boost::add_edge(points[i1].second, points[i2].second, G); 
      }
    }
  }
  
  if (! is_bipartite(G)){
    for (int i=0; i<m; ++i) {
      int ax, ay, bx, by; std::cin >> ax >> ay >> bx >> by;; 
      std::cout << 'n';
    }
    std::cout << std::endl;
  } else {
    
    // calculate connected component
    std::vector<int> component_map(n);
    boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 
    
    for (int i=0; i<m; ++i) {
      int ax, ay, bx, by; std::cin >> ax >> ay >> bx >> by;; 
      
      K::Point_2 pa (ax, ay), pb (bx, by);

      auto va = t.nearest_vertex(pa), vb = t.nearest_vertex(pb);
      
      double d2a = d2(pa, va->point());
      double d2b = d2(pb, vb->point());
      double d2ab = d2(pa, pb);
      
      if (d2ab <= r*r || (d2a <= r*r && d2b <= r*r && component_map[va->info()]==component_map[vb->info()])){
        // this also includes a=b, and the case where a and b are adjacent
        std::cout << 'y';
      } else {
        std::cout << 'n';
      }
    }
    std::cout << std::endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}