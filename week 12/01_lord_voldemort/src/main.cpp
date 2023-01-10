///3
#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> IV;
// typedef std::pair<int, int> EN; // End-poistion and number of score pair 

void solve(){
  int n, m, k; std::cin >> n >> m >> k;
  IV v (n, 0);
  
  for (int i=0; i<n; ++i) std::cin >> v[i];
  
  // IV end_map (n, -1), start_map (n, -1), end_posi;
  IV start_posi, end_posi, safe_prev_idx;
  
  int left=0, right=0, sum=v[right], biggest_unconfiled_idx=0;
  // at the same time, we also what a quick search for the biggest un-conflicted index
  while(right<n){
    if (sum==k){
      // end_map[left]=right;
      // start_map[right]=left;
      start_posi.push_back(left);
      end_posi.push_back(right);
      
      while(end_posi[biggest_unconfiled_idx]<left) ++biggest_unconfiled_idx;
      safe_prev_idx.push_back(biggest_unconfiled_idx-1); // this vector denotes the biggest index of end_position that is smaller than start
      
      sum -= v[left];
      ++left;
      
    }else if (sum<k){
      ++right;
      sum+=v[right];
    }else{
      // sum > k
      sum -= v[left];
      ++left;
    }
  }
  
  int n_eff=end_posi.size();
  
  // d[m, i] denotes the maximum score using at most m army when only up to i-th ending position can be occupied
  // not at most m, we need to be exact
  IV cur, old; int cur_max_score=-1;
  for (int i=0; i<n_eff; ++i){
    cur_max_score=std::max(cur_max_score, end_posi[i]-start_posi[i]+1);
    cur.push_back(cur_max_score);
  }
  
  for (int j=1; j<m; ++j){
    old.swap(cur); cur.clear();
    
    int cur_max_score=-1;
    for (int i=0; i<n_eff; ++i){
      // if we want i-th position occupied, its safe previous end point must have a non-negative score, denoting that safe_prev_idx[i] have a possibble m-1 solution
      int occupied_score;
      if (safe_prev_idx[i]==-1 or old[safe_prev_idx[i]]==-1) occupied_score=-1;
      else occupied_score=old[safe_prev_idx[i]]+(end_posi[i]-start_posi[i]+1);
      cur_max_score=std::max(cur_max_score, occupied_score);
      cur.push_back(cur_max_score);
      
      
      
      // // if i-th position is occupied
      // int prev_unoccu_max = (save_prev_idx[i]==-1) ? 0: old[save_prev_idx[i]];
      // int occupied_max_score = prev_unoccu_max + (end_posi[i]-start_posi[i]+1);
      // // if not occupied, the maximum value comes from previous d[m, i-1], or we can also choose d[m-1, i-1]
      // int unoccupied_max_score = std::max(cur_max_score, (i>0)? old[i-1] : 0);
      // cur_max_score = std::max(unoccupied_max_score, occupied_max_score);
      // cur.push_back(cur_max_score);
    }
  }
  
  int max_result=-1;
  for (int i=0; i<n_eff; ++i) max_result=std::max(max_result, cur[i]);
  
  if (max_result==-1) std::cout << "fail" << std::endl;
  else std::cout << max_result << std::endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}