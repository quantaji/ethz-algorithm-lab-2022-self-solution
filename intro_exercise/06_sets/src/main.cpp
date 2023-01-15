#include <iostream>
#include <set>

void solve() {
  int q;
  std::cin >> q;
  std::set<int> a;
  int temp;
  for (int i=0; i<q; i++) {
    std::cin >> temp;
    if (!temp) {
      std::cin >> temp;
      a.insert(temp);
    } else {
      std::cin >> temp;
      a.erase(temp);
    }
  }
  if (a.empty()) {
    std::cout << "Empty" << std::endl;
  } else {
    for (auto iter = a.begin(); iter != a.end(); iter++) {
     std::cout << *iter << ' ';
    }
    std::cout << std::endl;
  }
}

int main() {
  int t;
  std::cin >> t;
  for (int i=0; i<t; i++) {
    solve();
  }
}