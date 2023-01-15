///1
#include <iostream>
#include <algorithm>
#include <vector>

struct Compare
{
  bool operator() (std::pair<int, int> a, std::pair<int, int> b) // (r, l) format
  {
    return a.first < b.first;  
  }
};

void solve(){
  int n; std::cin >> n;
  std::vector<std::pair<int, int>> ring_list;
  
  for (int i=0; i<n; ++i){
    int l, p; std::cin >> l >> p;
    ring_list.push_back(std::pair<int, int> (p, l));
  }
  
  std::sort(ring_list.begin(), ring_list.end(), Compare());
  
  int current_r=0, num=0, current_posi=-1e8; // current posi stands for the end of current last boat
  
  while(current_r<n){
    
    int next=-1, next_posi=1e8, test_next=0; // 1e7 is a save maximum
   
    while(test_next + current_r < n){

      
      int t_r = ring_list[test_next + current_r].first, t_l = ring_list[test_next + current_r].second;
      
      if (current_posi - t_r > 0){ // curent testing ring is not availabe any way
        ++test_next;
        continue;
      } else {
        // find the earliest stopping time
        // ir current > t_r - t_l,  current - t_r - t_l > 0, there might be overlapping, then choose current + t_l
        int test_next_posi = t_r + std::max(0, current_posi - t_r + t_l);
      
        if (next_posi > test_next_posi){
          next = test_next;
          next_posi = test_next_posi;
        }
        
        if (current_posi - t_r + t_l <= 0) break; // in this case, the afterwards is guaranteed to be larger than t_r
        ++test_next;
      }
    }
    
    if (next == -1){
      // no choise avaliaabe
      break;
    } else {
       // already choose one
      current_r += next+1;
      current_posi = next_posi;
      ++num; 
    }
  }
  
  std::cout << num << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t; 
  for (int i=0; i<t; ++i) solve();
}