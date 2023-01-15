///1
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>
#include <algorithm>

typedef std::tuple<int ,int, int> idx_t; // idx, t_current, t_min along the tree

inline int parent(int id){
  if (id) return (id % 2==0) ? id/2-1 : (id-1)/2;
  else return 0;
}

struct Bigger{
  inline bool operator ()(const idx_t& a, const idx_t& b){
    return std::get<2>(a) > std::get<2>(b);
  }
};

void solve(){
  int n; std::cin >> n;
  std::vector<bool> deactivated (n, false);
  std::vector<int> t, t_min;
  std::priority_queue<idx_t, std::vector<idx_t>, Bigger> leaf;
  
  int t_i, t_i_min;
  for (int i=0; i<n; ++i){
    std::cin >> t_i;
    
    t.push_back(t_i);
    t_i_min = (i) ? std::min(t_i, t_min[parent(i)]) : t_i; // remember to deal with 0
    t_min.push_back(t_i_min);
    
    if (2*i>n-3){
      // leaf node
      leaf.push(idx_t(i, t_i, t_i_min));
    }
  }
  
  bool success=true; int timer=0;
  int top_id, top_t, parent_idx;
  while(!leaf.empty()){
    // std::cout << timer << ' ' << leaf.top().second << ' ' << leaf.top().first << std::endl;
    top_id = std::get<0>(leaf.top());
    top_t = std::get<1>(leaf.top());
    parent_idx = parent(top_id);
    if (top_id==0){
      // success!
      break;
    }
    if (timer < top_t){
      // ok
      deactivated[top_id]=true; leaf.pop();
      // if all to are diactivated, then add parent to queue
      if (deactivated[2*parent_idx+1] && deactivated[2*parent_idx+2]){
        leaf.push(idx_t(parent_idx, t[parent_idx], t_min[parent_idx]));
        // std::cout << "add "<< parent_idx << std::endl;
      }
     
    } else {
      // not ok, explode!
      success=false;
      break;
    }
    ++timer; 
  }
  // std::cout << timer << ' ';
  if (success) std::cout << "yes" << std::endl;
  else std::cout << "no" << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}