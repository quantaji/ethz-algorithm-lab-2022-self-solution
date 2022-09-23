#include <iostream>
#include <string>

void solve() {
  std::string a,b;
  std::cin >> a >> b;
  std::cout << a.size() << ' ' << b.size() << std::endl;
  std::string c(a.rbegin(), a.rend()), d(b.rbegin(), b.rend());
  *c.begin() = *(--b.end());
  *d.begin() = *(--a.end());
  std::cout << a + b << '\n' << c << ' ' << d << std::endl;
}

int main() {
  int t;
  std::cin >> t;
  for (int i=0; i<t; i++){
    solve();
  }
}