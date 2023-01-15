#include <iostream>
using namespace std;

void solve(int n) {
  int block_S[n+1][n+1];
  block_S[0][0] = 0;
  for (int i=1; i<=n; i++){
    block_S[0][i] = 0;
    block_S[i][0] = 0;
  }
  int S, temp;
  for (int i=1; i<=n; i++){
    S = 0;
    for (int j=1; j<=n; j++){
      cin>>temp;
      S+=temp;
      S %= 2;
      block_S[i][j] = (block_S[i-1][j] + S) % 2;
    }
  }
  // int Even[n+1][n+1];
  int Result=0, Even, Odd;
  for (int i_2=1; i_2<=n; i_2++){
    for (int i_1=0; i_1<i_2; i_1++){
      Even = 0;
      for (int j=0; j<=n; j++) {
        if (!((block_S[i_1][j] + block_S[i_2][j])%2)) {
          Even++;
        }
      }
      Odd = n+1-Even;
      Result += Even*(Even-1)/2 + Odd*(Odd-1)/2;
    }
  }
  cout<<Result<<endl;
}

int main() {
  int t; cin >> t;
  int n;
  for (int i=0; i<t; i++) {
    cin >> n;
    solve(n);
  }
}