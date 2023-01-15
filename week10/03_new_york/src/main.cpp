///4
#include <iostream>
#include <limits>
#include <vector>
#include <set>
#include <deque>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::edge_iterator edge_it;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;
typedef std::vector<int> IntV;
typedef std::pair<int, int> IntP; // temperature-index pair

struct Smaller{
  bool operator () (const IntP &a, const IntP &b) {
    if (a.first != b.first){
      return a.first < b.first;
    } else {
      return a.second < b.second;
    }
  }
};

// I saw the hint, only two words, sliding window.
// I got an idea, using multimap, to keep all the h_i in order
// this can reduce complexity from O(n*m) to O(n*log m)
// we have to use set and queue so we can have index and h at the same time

// we let the sliding window eithe move forward to leaf direction or move back to root direction, depending on the front of the sliding window,
// if this node have unvisited leaf node -> foward to this node
// else backward
// so we have to make a list of iterator

// also it would be nice to have a parent list

void solve(){
  
  int n, m, k; std::cin >> n >> m >> k;

  IntV h;
  for (int i=0; i<n; ++i){
    int h_i; std::cin >> h_i;
    h.push_back(h_i);
  }
  
  graph G(n); IntV p (n, -1); // parent
  for (int i=1; i<n; ++i){
    int u, v; std::cin >> u >> v;
    boost::add_edge(u, v, G);
    p[v] = u;
  }
  
  std::vector<out_edge_it> outit, outend;
  for (int i=0; i<n; ++i){
    out_edge_it iter, end;
    boost::tie(iter, end)=boost::out_edges(i, G);
    outit.push_back(iter);
    outend.push_back(end);
  }
  
  std::set<IntP> hidx; // temperature-index map
  std::deque<int> slide;
  
  // insert the root element
  slide.push_front(0); hidx.insert(IntP(h[0], 0)); int len_slide = 1;
  
  std::vector<bool> secure (n, false);
  
  while (!slide.empty()){
    // first we check whether to move forward or backward
    int f=slide.front(), e=slide.back();
    if (outit[f]!=outend[f]){
      // front node have unvisited child node
      
      // add new node
      int next=boost::target(*outit[f], G);
      slide.push_front(next); hidx.insert(IntP(h[next], next)); ++len_slide;
      
      // if the sliding window's length is larger than m, remove last;
      if (len_slide > m){
        slide.pop_back(); hidx.erase(IntP(h[e], e)); --len_slide;
      }
      
      // don't forget to update the iterator
      ++outit[f];
      
      // after forwarding, check if this point is secure
      if (len_slide == m && std::abs(hidx.begin()->first - hidx.rbegin()->first) <= k){
        int new_e=slide.back(); secure[new_e]=true;
      }
    } else {
      // backward
      if (p[e]!=-1){
        // first add backward node
        int parent=p[e];
        slide.push_back(parent); hidx.insert(IntP(h[parent], parent)); ++len_slide;
      }
      // then remove the front
      slide.pop_front(); hidx.erase(IntP(h[f], f)); --len_slide;
    }
  }
  
  bool fail=true;
  for (int i=0; i<n; ++i){
    if (secure[i] == true) {
      std::cout << i << ' ';
      fail=false;
    }
  }
  
  if (fail) std::cout << "Abort mission";
  std::cout << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}