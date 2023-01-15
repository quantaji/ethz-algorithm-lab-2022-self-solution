#include <iostream>
#include <vector>

void solve(int n) {
  std::vector<int> v(n);
  for (int i=0; i<n; i++) {
    std::cin >> v[i];
  }
  int erase_index;
  std::cin >> erase_index;
  v.erase(v.begin() + erase_index);
  int a, b;
  std::cin >> a >> b;
  v.erase(v.begin() + a, v.begin() + b + 1);
  for (auto iter=v.begin(); iter != v.end(); iter++){
    std::cout << *iter << ' ';
  }
  std::cout << std::endl;
}

int main() {
  int t, n;
  std::cin >> t;
  for (int i=0; i<t; i++) {
    std::cin >> n;
    solve(n);
  }
}