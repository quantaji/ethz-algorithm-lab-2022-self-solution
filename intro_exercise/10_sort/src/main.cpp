#include <iostream>
#include <vector>
#include <algorithm>

void solve() {
  int n; std::cin >> n;
  std::vector<int> a; int temp;
  for (int i=0; i<n; ++i) {
    std::cin >> temp; a.push_back(temp);
  }
  std::sort(a.begin(), a.end());
  
  int order; std::cin >> order;
  if (order == 0) {
    for (int i=0; i<n; ++i)
    {
      std::cout << a[i] << ' ';
    }
  } else {
    for (int i=n-1; i>=0; --i) {
      std::cout << a[i] << ' ';
    }
  }
  std::cout << std::endl;
}

int main() {
  int t;
  std::cin >> t;
  for (int i=0; i<t; ++i) {
    solve();
  }
}