#include <iostream>
using namespace std;

void solve(int n) {
  int Even=1, S=0, temp;
  for (int i=0; i<n; i++) {
    cin >> temp;
    S+=temp;
    if (!(S%2)) {
      Even++;
    }
  }
  int Odd = n+1 - Even;
  cout << Odd*(Odd-1)/2 + Even*(Even - 1)/2<<endl;
}

int main() {
  int t,n;
  cin >> t;
  for (int i=0; i<t; i++){
    cin >> n;
    solve(n);
  }
}