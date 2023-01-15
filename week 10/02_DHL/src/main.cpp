///4
#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <algorithm>
typedef std::pair<int, int> Pair;
typedef std::pair<Pair, int> PC; // state pair and its cost

// i still need some tricks to to pruning for test 4.

void solve(){
  int n; std::cin >> n;
  std::vector<int> Sa, Sb;
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
  
  std::vector<std::vector<int>> min_c (n, std::vector<int> (n, std::numeric_limits<int>::max()));
  
  for (int i=0; i<n; ++i) for (int j=0; j<n; ++j){
    // case 1: i or j is zero, then no need to calculate the previous sum
    if (i==0 || j==0){
      min_c[i][j] = Sa[i]*Sb[j];
    } else {
      for (int ip=0; ip<i; ++ip){
        min_c[i][j] = std::min( min_c[i][j], min_c[ip][j-1] + (Sa[i]-Sa[ip])*(Sb[j]-Sb[j-1]) );
      }
      for (int jp=0; jp<j; ++jp){
        min_c[i][j] = std::min( min_c[i][j], min_c[i-1][jp] + (Sa[i]-Sa[i-1])*(Sb[j]-Sb[jp]) );
      }
    }
  }
  
  std::cout << min_c[n-1][n-1] << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}