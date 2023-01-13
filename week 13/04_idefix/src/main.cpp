///2
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

// idea: union find in increasing order of edge of bones minimum distance
// each time there is an update on the maximum num of bones in a connected component, we store it into a vector

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Vertex_handle                  VH;
typedef K::Point_2 P;
typedef std::pair<P,Index> IPoint;

typedef std::vector<int> IV;
typedef std::pair<int, double> IDP; // idx and d2 pair, the first is used to store id of edge or id of vertex

struct Smaller{
  inline bool operator () (const IDP &a, const IDP &b){
    return a.second < b.second;
  }
};

int ccid(const IV & uftree, int id){
  int result=id;
  while (uftree[result]!=-1) result=uftree[result];
  return result;
}

void solve(){
  Index n, m; int k; double s; std::cin >> n >> m >> s >> k;
  
  std::vector<IPoint> trees; trees.reserve(n);
  for (Index i=0; i<n; ++i){
    int x, y; std::cin >> x >> y;
    trees.emplace_back(K::Point_2(x, y), i);
  }
  
  Delaunay t; t.insert(trees.begin(), trees.end());
  
  std::vector<IDP> edges;
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    if (i1 > i2) std::swap(i1, i2);
    Index edge_id = i1*n+i2;
    edges.push_back(IDP(edge_id, CGAL::squared_distance(trees[i1].first, trees[i2].first)));
  }
  
  std::sort(edges.begin(), edges.end(), Smaller());
  
  std::vector<IDP> bones;
  for (Index i=0; i<m; ++i){
    int x, y; std::cin >> x >> y;
    P point (x,y);
    VH v=t.nearest_vertex(point);
    bones.push_back(IDP(v->info(), 4*CGAL::squared_distance(point, v->point())));
  }
  
  std::sort(bones.begin(), bones.end(), Smaller());
  
  // std::cout << "-\n";
  // for (auto iter=bones.begin(); iter!=bones.end(); ++iter) std::cout << iter->first << ' ' << iter->second << std::endl;
  
  std::vector<IDP> nb_4r2; // list of pairs of number of bones and 4r^2
  nb_4r2.push_back(IDP(-1, -1)); // since there will be some bones under the tree, so when r=0, the corresponding k may not be zero, so we use (-1, -1) as initial value, instead of (0, 0)
  
  // now begine over union find
  IV uftree(n, -1), nbones(n, 0);
  auto eit=edges.begin();
  auto bit=bones.begin();
  
  double inf=std::numeric_limits<double>::max();
  
  while(eit!=edges.end() || bit!=bones.end()){
    // first finding the considering value of 4*r^2
    double value=std::min(   (eit!=edges.end())? eit->second:inf   ,   (bit!=bones.end())? bit->second:inf   );
    
    // an indicator of maximum number of bones in a CC, if nothing is updated, then we add nothing
    int tmp_max=nb_4r2.rbegin()->first;
    
    // first add all bones into coresponding CC
    while(bit!=bones.end() && bit->second == value){
      int tree_id=ccid(uftree, bit->first);
      ++nbones[tree_id];
      // check if maximum value changed
      tmp_max = std::max(tmp_max, nbones[tree_id]);
      ++bit;
    }
    
    // then connect all the edges together
    while(eit!=edges.end() && eit->second == value){
      int id1=ccid(uftree, (eit->first)/n);
      int id2=ccid(uftree, (eit->first)%n);
      if (id1!=id2){
        uftree[id2]=id1;
        nbones[id1]+=nbones[id2];
        nbones[id2]=0;
        tmp_max = std::max(tmp_max, nbones[id1]);
      }
      ++eit;
    }
    
    // then check if the numebr changed
    if (tmp_max > nb_4r2.rbegin()->first) nb_4r2.push_back(IDP(tmp_max, value));
  }
  
  // std::cout << "+\n";
  // for (auto iter=nb_4r2.begin(); iter!=nb_4r2.end(); ++iter) std::cout << iter->first << ' ' << iter->second << std::endl;
  
  // now we can solve over problem
  
  // given s, we want the a that its corresbonding 4r^2 is the biggest while it is smaller than or equal to s
  // given k, we want the smallest r^2 that is corresbonding a is bigger or equal to k, a>=k
  
  // two time of binary search
  int a;
  int left=0, right=nb_4r2.size(), mid; // left is always possible but right is impossible
  while(left<right){
    mid=(left+right)/2;
    if (nb_4r2[mid].second<=s) left=mid;
    else right=mid;
    if (left+1==right) break;
  }
  a=nb_4r2[left].first;
  
  double q;
  left=0, right=nb_4r2.size()-1; // this time left is impossible while right is possible
  while(left<right){
    mid=(left+right+1)/2;
    if (nb_4r2[mid].first>=k) right=mid;
    else left=mid;
    if (left+1==right) break;
  }
  q=nb_4r2[right].second;
  
  std::cout << a << ' ' << (long long)q << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}