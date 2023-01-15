#include <iostream>

void solve(int n){
  int temp, S=0;
  for (int i=0; i<n; i++) {
    std::cin >> temp;
    S += temp;
  }
  std::cout << S << std::endl;
}

int main() {
  int t, n;
  std::cin >> t;
  for (int i=0; i<t; i++) {
    std::cin >> n;
    solve(n);
  }
}