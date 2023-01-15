///4
#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <algorithm>
typedef std::pair<int, int> Pair;
typedef std::pair<Pair, int> PC; // state pair and its cost

// current complexity is O(n^3)
// i still need some tricks to to pruning for test 4.

// There exists a O(n^2) algorithm
// greedy

void solve(){
  int n; std::cin >> n;
  std::vector<int> Sa={0}, Sb={0}; // this exclude the current sum
  int sum_a = 0, sum_b = 0;
  
  for (int i=0; i<n; ++i){
    int a; std::cin >> a;
    sum_a += a-1;
    Sa.push_back(sum_a);
  }
  for (int i=0; i<n; ++i){
    int b; std::cin >> b;
    sum_b += b-1;
    Sb.push_back(sum_b);
  }
  
  // we record each step as x y coorinate pair, and the track we are finding optimal is a list of this coodinate
  // initially all x is zero, and we iterate over x=1 ... x= n-1
  
  std::vector<Pair> step;
  std::vector<int> sum = {0}; // this stores the sum, excluding current block
  for(int i=0; i<n; ++i){
    step.push_back(Pair(0, i));
    sum.push_back(Sb[1]*Sa[i+1]);
  }
  
  for(int j=1; j<n; ++j){
  
    int opt_idx=-1; // act on the opt_idx-th block, -1 means no operation
    int min_val = std::numeric_limits<int>::max(); // record the current maximum difference.

    std::size_t idx=0; // this records the index of block
    
    for (int i=0; i<n; ++i){
      while(idx+1 < step.size() && step[idx+1].second <= i) ++idx;
    
      int cur_x = step[idx].first, cur_y = step[idx].second;
      int changed = (Sa[cur_y+1]-Sa[cur_y])*(Sb[j]-Sb[cur_x+1]) + (Sa[n-1]-Sa[cur_y+1])*(Sb[j+1]-Sb[j]);
      
      if ( idx==0 || !(step[idx-1].first == cur_x && j-cur_x > 1) || !(step[idx-1].first == cur_x && cur_y==n-1) ){ // if we add colum in x direction, and the previous block and current block have same x, we have to remove current block, else remain
        changed += (Sa[cur_y+1]-Sa[cur_y])*(Sb[cur_x+1]-Sb[cur_x]);
      }
      
      if (min_val > sum[idx] + changed){
        min_val = sum[idx] + changed;
        opt_idx = idx;
      }
      // std::cout <<"Current comparision: "<< cur_x << ' ' << cur_y << ' ' << sum[idx] + changed << std::endl;
    }
    
    
    // there is truely a better option
    // resize the step vector to opt_idx, sum to opt_idx+1, then recalculate the sum
    int cur_x = step[opt_idx].first, cur_y = step[opt_idx].second;
    step.resize(opt_idx); sum.resize(opt_idx+1);
    
    // std::cout << "Changed Position: " << j << ' ' << opt_idx << ' ' << cur_x << ' ' << cur_y << std::endl;
    
    if (opt_idx==0 || !(step[opt_idx-1].first == cur_x && j-cur_x > 1)) {
      // in this case, add the current block
      step.push_back(Pair(cur_x, cur_y));
      sum.push_back(*sum.rbegin() + (Sa[cur_y+1]-Sa[cur_y])*(Sb[cur_x+1]-Sb[cur_x]) );
    }
    
    for (int nx=cur_x+1; nx<j; ++nx){
      step.push_back(Pair(nx, cur_y));
      sum.push_back(*sum.rbegin() + (Sa[cur_y+1]-Sa[cur_y])*(Sb[nx+1]-Sb[nx]) );
    }
    
    for (int ny=cur_y+1; ny<n-1; ++ny){
      step.push_back(Pair(j, ny));
      sum.push_back(*sum.rbegin() + (Sa[ny+1]-Sa[ny])*(Sb[j+1]-Sb[j]) );
    }

    
    // then push back the next x
    step.push_back(Pair(j, n-1));
    sum.push_back(*sum.rbegin() + (Sa[n]-Sa[n-1])*(Sb[j+1]-Sb[j]) );
  
  }
  
  // for (auto it=step.begin(); it!=step.end(); ++it){
  //   int x=it->first, y=it->second;
  //   std::cout << x << ' ' << y << std::endl;
  // }
  
  // std::cout <<"Anwer "<< *sum.rbegin() << std::endl;
  std::cout << *sum.rbegin() << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}