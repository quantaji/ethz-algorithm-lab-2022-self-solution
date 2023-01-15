#include <iostream>
#include <map>
#include <set>

void solve() {
  int q;
  std::cin >> q;
  std::map<std::string, std::set<int>> a;
  int value;
  std::string key;
  for (int i=0; i<q; i++) {
    std::cin >> value;
    std::cin >> key;
    if (value) {
      auto iter = a.find(key);
      if (iter == a.end()) {
        a.insert(std::pair<std::string, std::set<int>>(key, std::set<int>()));
      }
      a[key].insert(value);
    } else {
      a[key].clear();
    }
  }
  std::string queried_key;
  std::cin >> queried_key;
  if (a[queried_key].empty()) {
    std::cout << "Empty" << std::endl;
  } else {
    for (auto iter=a[queried_key].begin(); iter != a[queried_key].end(); ++iter) {
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