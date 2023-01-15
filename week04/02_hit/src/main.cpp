///1
// std
#include <iostream>
#include <vector>
#include <algorithm>
// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h> 
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/intersections.h>
#include <CGAL/squared_distance_2.h>
// Boost
#include "boost/variant.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K; 
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;
// for int < 2^53, they can be stored as double, down to precision 1. so IK is ok for do_intersection()
// long takes less time than double.... for cin


void solve(int n) {
  long x, y, a, b; std::cin >> x >> y >> a >> b;
  R ray = R(P(x, y), P(a, b));
  bool do_hit = false;
  
  long r, s, t, u;
  for (int i=0; i<n; ++i) {
    std::cin >> r >> s >> t >> u; 
    if (do_hit) continue;
    if (CGAL::do_intersect(ray, S(P(r, s), P(t, u)))) do_hit = true;
  }

  
  if (do_hit) std::cout << "yes" << std::endl;
  else std::cout << "no" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n) {
    solve(n);
    std::cin >> n;
  }
}