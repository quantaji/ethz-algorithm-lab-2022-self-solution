///4
#include <iostream>
#include <algorithm>
#include <map>
#include <set>

typedef std::pair<long long, int> D_T_pair;
typedef std::multimap<long long, D_T_pair> Comb_Map;
typedef std::set<long long> Value_Set;

void insert_all_possible(long long *d, long long *t, int n, long long acum_t, long long acum_d, int acum_n, Comb_Map &target){
  // done recursively
  if (n==0){
   target.insert(std::pair<long long, D_T_pair> (acum_t, D_T_pair(acum_d, acum_n)));
  } else {
    // if choose current
    insert_all_possible(d+1, t+1, n-1, acum_t+(*t), acum_d+(*d), acum_n+1, target);
    // if not choose current
    insert_all_possible(d+1, t+1, n-1, acum_t, acum_d, acum_n, target);
  } 
}

void solve(){
  int n, m; std::cin >> n >> m;
  long long D, T; std::cin >> D >> T;
  
  long long d[n], t[n];
  for (int i=0; i<n; ++i) {
    std::cin >> d[i] >> t[i];
  }
  
  long long s[m+1];  s[0]=0;
  for (int i=1; i<=m; ++i) std::cin >> s[i];
  
  // splite set, if n>= 2 this is ok, if n=1, left would be 1 and right will be 0. we must guranted left is always not empty
  int left_l = n - n/2, right_l = n/2; // left set, d:d+mid, right, d+mid
  Comb_Map map_left, map_right;
  insert_all_possible(d, t, left_l, 0, 0, 0, map_left);
  insert_all_possible(d+left_l, t+left_l, right_l, 0, 0, 0, map_right);
  
  // find searching range. for left set t_start should the highest value s.t. < T - sum t_right, so would be the element privous to the lower_bound
  long long t_start_left = T - 1; 
  for (int i=left_l; i<n; ++i) t_start_left -= t[i]; 
  t_start_left = std::max(0ll, t_start_left);
  long long t_start_right = T - 1; 
  for (int i=0; i<left_l; ++i) t_start_right -= t[i]; 
  t_start_right = std::max(0ll, t_start_right);
  
  // prepare for binary search
  int left_m = 0, right_m = m;
  bool impossible = true;
  while(right_m >= left_m) {
    bool need_stop = (right_m == left_m) ? true : false; // if r = l, we run one last time
    int mid = (left_m + right_m) / 2;
    
    long long d_max = 0;
    std::map<long long, long long> left_value, right_value; // map t value in the set to the maximum score d we can get
    
    // takes 2^15 operation
    long long d_left=0, d_right=0;
    for (auto iter = map_left.begin(); iter!=map_left.end(); ++iter){
      d_left = std::max(d_left, iter->second.first + s[mid]*iter->second.second);
      left_value[iter->first] = d_left;
      // if (t_start_left >= iter->first) left_value[t_start_left] = d_left;
    }
    // right value takes T - t_r as key, so that key_r - key_l = T - t_l - t_r > 0
    // t_r start with highest value lower than T, so key_r is the smallest value bigger than 0;
    // t_r ends with the last possible value >= t_start_right, so the next value would be t_r < t_start_right, so key_r_end > T - t_start_right
    for (auto iter = map_right.begin(); iter!=map_right.end(); ++iter){
      d_right = std::max(d_right, iter->second.first + s[mid]*iter->second.second);
      right_value[T - iter->first] = d_right;
      // if (t_start_right >= iter->first) right_value[T - t_start_right] = d_right;
    }
    
    // pruning
    auto iter_l = left_value.upper_bound(t_start_left); --iter_l;
    auto end_l = left_value.lower_bound(T);
    auto iter_r = right_value.upper_bound(0);
    auto end_r = right_value.upper_bound(T - t_start_right);
    
    while (iter_l != end_l && iter_r != end_r){
      if (iter_r->first - iter_l->first > 0){
        // this is ok
        d_max = std::max(d_max, iter_l->second + iter_r->second);
        ++iter_l;
      } else {
        ++iter_r;
      }
    }
    

    // std::cout <<mid << ' '<<s[mid] << ' '<< d_max << ' ' << D << ' ' << std::endl; // for debug
    if (d_max < D){ // fails, current m is not enough
      left_m = mid+1;
    } else{
      impossible = false;
      right_m = mid; // suceeds, low the upper boudn
    }
    if (need_stop) break;
  }

  if (impossible) std::cout << "Panoramix captured" << std::endl;
  else std::cout << right_m << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}