#include <iostream>
#include <vector>
#include <algorithm>
// cgal
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

void solve(int n){
  std::vector<IPoint> old_p;
  old_p.reserve(n);
  for (int i=0; i<n; ++i){
    double x, y; std::cin >> x >> y;
    old_p.emplace_back(K::Point_2(x, y), i);
  }
  
  Delaunay t;
  t.insert(old_p.begin(), old_p.end());
  
  int m; std::cin >> m;
  // double min_dist = double(1LL<<62);
  
  for (int i=0; i<m; ++i){
    double x, y; std::cin >> x >> y;
    Tds::Vertex_handle v = t.nearest_vertex(K::Point_2(x, y));
    double dist = (x-v->point().x())*(x-v->point().x()) + (y-v->point().y())*(y-v->point().y());
    // min_dist = std::min(dist, min_dist);
    std::cout << (long long)dist << std::endl;
  }
  
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n>0){
    solve(n);
    std::cin >> n;
  }
}