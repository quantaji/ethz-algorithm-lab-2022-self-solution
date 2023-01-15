///4
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>

// we use a single int to denote all state
// s = (k+1)^(2m), diff
// the digit 0 denote empty

int power(int n, int m){
  int result=1;
  for (int i=0; i<m; ++i) result *= n;
  return result;
}

// denote 0 as north and 1 as south, and diff as north - south
// then the updated diff is +1 if act=0

void update_win(int & n_ws, int &gain, int ws, int cur_k, int k, int m, int kpom){
  // update the state of window, and get the gain
  // kpom is (k+1)^m for computational convinience
  
  // first calculate gain
  // note: using vector or set will be out of time, using array is faster
  int a[4]={0,0,0,0};
  int temp_ws=ws;
  a[cur_k]=1;
  for (int i=0;i<m-1; ++i){
    int digit=temp_ws%(k+1);
    if (digit>0) a[digit-1]=1;
    temp_ws /= k+1;
  }
  gain = (a[0]+a[1]+a[2]+a[3])*1000;
  
  // get new state
  n_ws = (ws*(k+1))%kpom + (cur_k+1);
}

void update(int & n_s_n, int & excitement_n, int & n_s_s, int & excitement_s, int o_s, int cur_k, int k, int m, int MaxD, int kpom){
  // decode
  int diff=o_s%(2*MaxD)-MaxD;
  int o_ws_s = (o_s/(2*MaxD))%kpom; // window state south
  int o_ws_n = (o_s/(2*MaxD*kpom))%kpom;
  
  // update
  int n_diff_n=diff+1, n_diff_s=diff-1;
  int n_ws_n, n_ws_s, gain_n, gain_s;
  
  update_win(n_ws_n, gain_n, o_ws_n, cur_k, k, m, kpom);
  update_win(n_ws_s, gain_s, o_ws_s, cur_k, k, m, kpom);
  
  excitement_n = gain_n - (1<<(std::abs(n_diff_n)));
  excitement_s = gain_s - (1<<(std::abs(n_diff_s)));
 
  // encode
  n_s_n = (2*MaxD*kpom)*n_ws_n + (2*MaxD)*o_ws_s + (n_diff_n+MaxD);
  n_s_s = (2*MaxD*kpom)*o_ws_n + (2*MaxD)*n_ws_s + (n_diff_s+MaxD);
}

void solve(){
  int n, k, m; std::cin >> n >> k >> m;
  
  // some constant needed
  int kpom = power(k+1, m);
  int MaxD = 15; // we know 11 is the most possible, but for safety, we set 15
  
  // initial state
  int init_s = MaxD; // initially, empty, ws_n = ws_s = diff = 0;
  std::map<int, int> cur_m, prev_m;
  cur_m[init_s] = 0;
  
  for (int i=0; i<n; ++i){
    
    // first update two map
    prev_m.swap(cur_m); cur_m.clear();
    
    // input
    int cur_k; std::cin >> cur_k;
    
    for (auto iter=prev_m.begin(); iter!=prev_m.end(); ++iter){
      int o_s = iter->first, o_max_sum=iter->second;
      
      int n_s_n, n_s_s, excitement_n, excitement_s;
      update(n_s_n, excitement_n, n_s_s, excitement_s, o_s, cur_k, k, m, MaxD, kpom);
      
        if (excitement_n >= 0){
          // no negative excitement
          
          if (cur_m.find(n_s_n)==cur_m.end()){
            cur_m[n_s_n] = o_max_sum + excitement_n;
          } else {
            cur_m[n_s_n] = std::max(cur_m[n_s_n], o_max_sum + excitement_n);
          }
        }
        
        if (excitement_s >= 0){
          // no negative excitement
          
          if (cur_m.find(n_s_s)==cur_m.end()){
            cur_m[n_s_s] = o_max_sum + excitement_s;
          } else {
            cur_m[n_s_s] = std::max(cur_m[n_s_s], o_max_sum + excitement_s);
          }
        }
        
    }
  }
  
  int max_result = 0;
  for (auto iter=cur_m.begin(); iter!=cur_m.end(); ++iter){
    max_result = std::max(max_result, iter->second);
  }
  
  std::cout << max_result << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}