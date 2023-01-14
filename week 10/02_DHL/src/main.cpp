///2
#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <algorithm>
typedef std::vector<int> IV;

// i still need some tricks to to pruning for test 4.

void solve(){
  int n; std::cin >> n;
  std::vector<int> A, B;
  
  for (int i=0; i<n; ++i){
    int a; std::cin >> a;
    A.push_back(a-1);
  }
  for (int i=0; i<n; ++i){
    int b; std::cin >> b;
    B.push_back(b-1);
  }
  
  // int inf=std::numeric_limits<int>::max();
  int inf=100*100*1000*2;
  
  std::vector<IV> d1(n, IV (n, inf)), d2(n, IV (n, inf)), d3(n, IV (n, inf));
  d1[0][0]=A[0]*B[0];
  
  for (int ipj=1; ipj<2*n-1; ++ipj){ //ipj means i+j, ranges from 1 to 2n-2
    for (int i=std::max(0, ipj-(n-1)); i<=std::min(n-1, ipj); ++i){
      int j=ipj-i;
      int block=A[i]*B[j];
      if (i-1>=0 && j-1>=0) {
        d1[i][j]=std::min(d1[i][j], block+d1[i-1][j-1]);
        d1[i][j]=std::min(d1[i][j], block+d2[i-1][j-1]);
        d1[i][j]=std::min(d1[i][j], block+d3[i-1][j-1]);
      }
      if (i-1>=0){
        d2[i][j]=std::min(d2[i][j], block+d1[i-1][j]);
        d2[i][j]=std::min(d2[i][j], block+d2[i-1][j]);
      }
      if (j-1>=0){
        d3[i][j]=std::min(d3[i][j], block+d1[i][j-1]);
        d3[i][j]=std::min(d3[i][j], block+d3[i][j-1]);
      }
    }
  }
  int result=inf;
  result=std::min(result, d1[n-1][n-1]);
  result=std::min(result, d2[n-1][n-1]);
  result=std::min(result, d3[n-1][n-1]);
  std::cout << result << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}