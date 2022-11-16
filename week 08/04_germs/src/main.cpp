///0
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <map>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Finite_faces_iterator                        Face_iterator;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

double distance2(const Delaunay::Vertex_handle & v1, const Delaunay::Vertex_handle & v2){
  double dx=v1->point().x()-v2->point().x(), dy=v1->point().y()-v2->point().y();
  return dx*dx+dy*dy;
}

double round_up_solve(double d){
  // ((2*t^2+1)^2 = d)
  double t = int(std::sqrt(std::min(1.0, (std::sqrt(d)-1))/2));
  while(t>=1.0 && (2*t*t+1)*(2*t*t+1)>=d) --t;
  while((2*t*t+1)*(2*t*t+1)<d) ++t;
  return t;
}


void solve(int n){
  
  int l,b,r,t; 
  std::cin >> l >> b >> r >> t;
  
  std::vector<IPoint> bect;
  std::vector<double> d2;
  
  for (int i=0; i<n; ++i){
    double x, y; std::cin >> x >> y;
    bect.push_back(IPoint(K::Point_2(x, y), i));
  }
  
  Delaunay T; T.insert(bect.begin(), bect.end());
  
  for (Delaunay::Finite_vertices_iterator v=T.finite_vertices_begin(); v!=T.finite_vertices_end(); ++v){
    double x=v->point().x(), y=v->point().y();
    double bound=std::min(std::min(x-l, r-x), std::min(y-b, t-y));
    double dd = 4*bound*bound;
    Delaunay::Vertex_circulator v_n = T.incident_vertices(v); 
    // there is a case where there is only one vertex, 
    // so in this case, it is isolated, we have to first check it first
    if (v_n != 0) do{
      if (!T.is_infinite(v_n)){
        dd = std::min(dd, distance2(v, v_n));
      }
    } while(++v_n!=T.incident_vertices(v));
    d2.push_back(dd);
  }
  
  std::sort(d2.begin(), d2.end());

  double f, m, ll;
  f=*d2.begin(); m=*(d2.begin() + (d2.size())/2 ); ll=*d2.rbegin();
  std::cout << (int)round_up_solve(f) << ' ' << (int)round_up_solve(m) << ' ' << (int)round_up_solve(ll) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n) {
    solve(n); std::cin >> n;
  }
}