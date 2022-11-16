///1
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
// typedef std::map<Delaunay::Face_handle, int>                   Indexmap;
typedef std::map<Tds::Face_handle, double>                Dmap;
enum Color {Free=0, Block=1, Border=2};
typedef std::map<Tds::Face_handle, Color>                 Colormap;
typedef std::pair<Tds::Face_handle, double>               FDpair;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

// I have an idea, we should not think in a way that the disk is growing,
// we should think as the disk is shinking. 
// ther is a time for each disk's edge come alive,
// we should keep this
// we keep a list of face that (1) is already free, but have unfreed neighbor
// and further, a priority que that (the biggest the first), containing the d of edges that in neighbor of unfreed neighbor
// each time we reduce the d to the biggest, and there will be some edges turned to be possible,
// then we do a bfs or dfs to mark all the newly freed faces, and record there d to be freed.
// until there is no unfreed face
// Yes, we must use coloring

//maybe we can start from infinite edge, and use the face circulator to get the first set of accessed faces

// we maintain a set(as priority queue), of bordered faces
// each time we free a face, we add its blocked neighbor to this set, and update the neighbor bordered face's min d value

struct Greater {
  inline bool operator() (const FDpair& e1, const FDpair& e2) {
    if (e1.second != e2.second){
      return e1.second > e2.second;
    } else return e1.first > e2.first;
  }
};

double distance2(const Delaunay::Vertex_handle & v1, const Delaunay::Vertex_handle & v2){
  double dx=v1->point().x()-v2->point().x(), dy=v1->point().y()-v2->point().y();
  return dx*dx+dy*dy;
}

void solve(int n){
  std::vector<IPoint> infected; infected.reserve(n);
  for (int i=0; i<n; ++i){
    double x,y; std::cin >> x >> y;
    infected.emplace_back(K::Point_2(x, y), i);
  }
  
  Delaunay t; t.insert(infected.begin(), infected.end());
  
  Colormap cmap; Dmap dmap;
  for (Face_iterator f=t.finite_faces_begin(); f!=t.finite_faces_end(); ++f) cmap[f]=Block;
  
  std::set<FDpair, Greater> border_q;
  
  Delaunay::Vertex_handle infv = t.infinite_vertex(); // start at THE only infinite vertex
  Delaunay::Face_circulator c = t.incident_faces(infv);
  do{
    for (int k=0; k<3; ++k) if (!t.is_infinite(c->neighbor(k))) {
    
      // calculate d
      double dist=distance2(c->vertex((k+1)%3), c->vertex((k+2)%3));
      cmap[c->neighbor(k)]=Border; // add to border
      
      // add to border queue
      border_q.insert(FDpair(c->neighbor(k), dist));
    }
  } while (++c != t.incident_faces(infv));
  
  // now begin searching 
  while (!border_q.empty()){
    double d=border_q.begin()->second; // current reference d to be compared with
    
    while (!border_q.empty() && border_q.begin()->second >= d){
      // first save the face we want to color 
      Delaunay::Face_handle f=border_q.begin()->first;
      // double f_d=border_q.begin()->second; // not sure this will be used
      
      // now delete it from queue;
      border_q.erase(border_q.begin());
      // map the face as freed
      cmap[f]=Free;
      // record the d at that moment that it is freed
      dmap[f]=d;
      
      // now deal with its neighbor
      for (int k=0;k<3;++k) if (!t.is_infinite(f->neighbor(k))){
        
        Delaunay::Face_handle f_n = f->neighbor(k);
        
        if (cmap[f_n] == Free) continue;
        
        else if (cmap[f_n] == Block){// block -> border
        
          cmap[f_n]=Border;
          double dist=distance2(f->vertex((k+1)%3), f->vertex((k+2)%3));
          border_q.insert(FDpair(f_n, dist));
          
        } else{// case of bordered, most complicated, need to update
          double dist=distance2(f->vertex((k+1)%3), f->vertex((k+2)%3)); // record the current dist between f and f_n
          for (int j=0; j<3; ++j){
            double distp = distance2(f_n->vertex((j+1)%3), f_n->vertex((j+2)%3));
            if (dist > distp && border_q.find(FDpair(f_n, distp)) != border_q.end() ){
              // this is the case where current update effect the position of border face
              
              border_q.erase(border_q.find(FDpair(f_n, distp)));
              border_q.insert(FDpair(f_n, dist));
              
            }
          }
        }
      }
    }
  }
  
  // for (Face_iterator f=t.finite_faces_begin(); f!=t.finite_faces_end(); ++f) std::cout << dmap[f] << ' '; 
  
  int m; std::cin >> m;
  for (int i=0; i<m; ++i){
    double x, y, d; std::cin >> x >> y >> d;
    
    Tds::Vertex_handle v = t.nearest_vertex(K::Point_2(x, y));
    
    double dx=x-v->point().x(), dy = y-v->point().y();
    double dist=dx*dx+dy*dy;
    
    if (dist<d) std::cout << 'n';
    else{
      Tds::Face_handle f = t.locate(K::Point_2(x, y));
      if (t.is_infinite(f)) std::cout << 'y'; // already outside
      else {
        if (dmap[f] >= 4*d) std::cout << 'y';
        else std::cout << 'n';
      }
    }
  }
  std::cout << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n) {
    solve(n); std::cin >> n;
  }
}