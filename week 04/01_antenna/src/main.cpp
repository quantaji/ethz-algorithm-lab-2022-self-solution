#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h> 
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>
#include <algorithm>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef CGAL::Min_circle_2_traits_2<K> Traits; 
typedef CGAL::Min_circle_2<Traits> Min_circle;

long my_sqrt_floor(const K::FT& x) {
  long a = long(std::floor(std::sqrt(CGAL::to_double(x)))); // cannot take CGAL::sqrt to EPEC::FT
  while (K::FT(a)*K::FT(a) > x) a -= 1; // long * long may exceed long
  while (K::FT(a)*K::FT(a) < x) a+=1;
  return a;
}

void solve(int n){
  std::vector<Point_2> points;
  for (int i=0; i<n; ++i) {
    long x, y; std::cin >> x >> y;
    points.push_back(Point_2(x, y));
  }
  Min_circle mc(points.begin(), points.end(), true);
  Traits::Circle c = mc.circle();
  K::FT r_square = c.squared_radius();
  std::cout << my_sqrt_floor(r_square) <<std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int n; std:: cin >> n;
  while (n) {
    solve(n);
    std:: cin >> n;
  }
}



// #include <iostream>
// #include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
// #include <CGAL/Min_circle_2.h> 
// #include <CGAL/Min_circle_2_traits_2.h>
// #include <vector>

// typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
// typedef K::Point_2 Point_2;
// typedef CGAL::Min_circle_2_traits_2<K> Traits; 
// typedef CGAL::Min_circle_2<Traits> Min_circle;

// long my_floor(const K::FT& x) {
//   long a = long(std::floor(CGAL::to_double(x)));
//   while (a>x) a -= 1;
//   while (a<x) a+=1;
//   return a;
// }

// void solve(int n){
//   std::vector<Point_2> points;
//   for (int i=0; i<n; ++i) {
//     long x, y; std::cin >> x >> y;
//     points.push_back(Point_2(x, y));
//   }
//   Min_circle mc(points.begin(), points.end(), true);
//   Traits::Circle c = mc.circle();
//   K::FT r = CGAL::sqrt(c.squared_radius());
//   std::cout << my_floor(r) <<std::endl;
// }

// int main(){
//   std::ios_base::sync_with_stdio(false);
//   int n; std:: cin >> n;
//   while (n) {
//     solve(n);
//     std:: cin >> n;
//   }
// }