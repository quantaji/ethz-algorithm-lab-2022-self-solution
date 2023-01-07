///4
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef std::vector<int> IV;
// typedef std::vector<double> DV;
typedef K::Point_2 P;
typedef Delaunay::Vertex_handle Vertex_handle;
typedef Delaunay::Vertex_circulator Vertex_circulator;

int cc_id(const IV & idcc, int idx){
  int result=idx;
  while( idcc[result] != -1) result=idcc[result];
  return result;
}

double d2(const P& a, const P& b){
  return (a.x()-b.x())*(a.x()-b.x()) + (a.y()-b.y())*(a.y()-b.y());
}

void solve(){
  
  int n; double r; std::cin >> n >> r; // cannot use int to store r, if so, r*r will be negative, out of range
  
  double r2 = r*r;
  
  IV x (n, 0), y (n, 0);
  for (int i=0; i<n; ++i) std::cin >> x[n-1-i] >> y[n-1-i];
  
  if (n==2) std::cout << 1 << std::endl;
  else{
    Delaunay t; t.insert(P(x[0], y[0]));
    IV ncc (n, 1), idcc (n, -1); // idcc denotes its predicessor
    IV max_ncc (n, 1);
    std::map<Vertex_handle, int> idx_map;
    
    for (int i=1; i<n; ++i){
      Vertex_handle v = t.insert(P(x[i], y[i]));
      idx_map[v]=i;
      
      Vertex_circulator u=t.incident_vertices(v);
      do{
        if (!t.is_infinite(u) && d2(u->point(), v->point()) <= r2){
          // v is reachable from u
          int j = idx_map[u];
          // std::cout << i << ' ' << j << std::endl;
          
          // why there are some cases that the connected component not increasing???
          // connect i and j and add this connected component
          
          int a=cc_id(idcc, i), b=cc_id(idcc, j);
          if (a != b){
            // std::cout << a << ' ' << b << ' ' << ncc[a] << ' ' << ncc[b] << std::endl;
            idcc[b]=a;
            ncc[a] += ncc[b]; ncc[b]=0;
          }
        }
      } while(++u!=t.incident_vertices(v));
      
    
      int ccid_i = cc_id(idcc, i);
      max_ncc[i]=std::max(max_ncc[i-1], ncc[ccid_i]); // only this value is updated.
      
      // currently we are exploring point n-1, n-1-1, ... , n-1-i, totally i+1 point,
      // at the same time the other part is occupying 0, 1, ..., i, this totally i point
      // when the empire takse 0, 1, ..., i, totally (i+1) place, the remaining place is from id n-1 to id i+1
      // and we have the at most max_ncc[n-2-i] this alias, but we can only take i+1 points, so its min(i+1, max_ncc[n-2-i])
      
      // is this enough to only consider the added vertex?
      if(max_ncc[i] > n-1-i) break; // to reduce unnecessary computation
    }
    
    // for (int i=0; i<n; ++i) std::cout << max_ncc[i] << ' ';
    // std::cout << std::endl;
    
    int result = 1;
    for (int i=1; i<1+((n%2==1)? (n-3)/2:(n-2)/2); ++i) result=std::max(result, std::min(i+1, max_ncc[n-2-i]));
    std::cout << result << std::endl; 
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}