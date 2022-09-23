#include <iostream>
#include <iomanip>
#include <string>

void solve() {
  int a;
  long b;
  std::string c;
  double d;
  std::cin >> a >> b >> c >> d;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << a << ' ' << b << ' ' << c << ' ' << d << std::endl;
}

int main(){
  int t;
  std::cin >> t;
  for (int i=0; i<t; i++) {
    solve();
  }
}