#include <iostream>
#include <cmath>
using namespace std;

void solve(){
  int n,k;
  cin>>n>>k;
  int S[n+1];
  int temp;
  S[0] = 0;
  for (int i=0; i<n; ++i) {
    cin >> temp;
    S[i+1] = S[i] + temp;
  }
  
  int min_r_l[n+1];
  int i=0, j=1;
  while (i<n+1) {
    while ((S[j] - S[i] < k) && (j<n+1)) {
      ++j;
    }
    min_r_l[i] = j;
    ++i;
  }
  int delta = k;
  int b_l=0, b_r=0, right, current_delta;
  for (int left=0; left<n; ++left){
    right = min_r_l[left]-1;
    current_delta = abs(S[right] - S[left] - k);
    if (current_delta < delta) {
      delta = current_delta;
      b_l = left; b_r = right - 1;
    }
    ++right;
    current_delta = abs(S[right] - S[left] - k);
    if (current_delta < delta) {
      delta = current_delta;
      b_l = left; b_r = right - 1;
    }
  }
  cout<<b_l<<' '<<b_r<<endl;
}

int main() {
  int t;
  cin>>t;
  for (int i=0; i<t; ++i) {
    solve();
  }
}