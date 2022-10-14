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

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK; 
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef IK::Point_2 IP;
typedef EK::Point_2 EP;
typedef IK::Ray_2 IR;
typedef EK::Ray_2 ER;
typedef IK::Segment_2 IS;
typedef EK::Segment_2 ES;
// for int < 2^53, they can be stored as double, down to precision 1. so IK is ok for do_intersection()

long my_round_to_double(const EK::FT& x) {
  long a = long(std::floor(CGAL::to_double(x))); 
  while (EK::FT(a) > x) a -= 1; // no comparison for long and EK::FT
  while (EK::FT(a+1) <= x) a += 1;
  return a;
}

void solve(int n) {
  long x, y, a, b; std::cin >> x >> y >> a >> b;
  EP source_e = EP(x, y);
  ER ray_e = ER(source_e, EP(a, b));
  EK::FT min_d_square = EK::FT(2L<<54) * EK::FT(2L<<54); // 2^105 is bigger than any distance
  int index = -1;
  std::vector<IS> segs;
  EK::FT temp_dsq;
  
  long r, s, t, u;
  for (int i=0; i<n; ++i) {
    std::cin >> r >> s >> t >> u; 
    segs.push_back(IS(IP(r, s), IP(t, u)));
  }
  // no need to do intersection...
  std::random_shuffle(segs.begin(), segs.end());
  
  for (int i=0; i<n; ++i) {
    if (min_d_square < EK::FT(1)) break; // do some pruning
    ES seg_e = ES(EP(segs[i].source().x(), segs[i].source().y()), EP(segs[i].target().x(), segs[i].target().y()));
    if (min_d_square < CGAL::squared_distance(source_e, seg_e)) continue; // this can reduce the time by half/
    auto result = CGAL::intersection(ray_e, seg_e);
    if (result){
      if (const EP* op = boost::get<EP>(&*result)) {
        temp_dsq = CGAL::squared_distance(source_e, *op);
      } else if (const ES* os = boost::get<ES>(&*result)) {
        temp_dsq = CGAL::squared_distance(source_e, *os);
      }
      if (temp_dsq < min_d_square){
        index = i;
        min_d_square = temp_dsq;
      }
    }
  }
    
  if (index == -1) {
    std::cout << "no" << std::endl; 
  } else {
    ES seg_e = ES(EP(segs[index].source().x(), segs[index].source().y()), EP(segs[index].target().x(), segs[index].target().y()));
    auto result = CGAL::intersection(ray_e, seg_e);
    EP intersection_point;
    if (const EP* op = boost::get<EP>(&*result)) {
      intersection_point = *op;
    } else if (const ES* os = boost::get<ES>(&*result)) {
      intersection_point = (CGAL::squared_distance(source_e, os->source()) < CGAL::squared_distance(source_e, os->target())) ? os->source() : os->target();
    }
    std::cout << my_round_to_double(intersection_point.x()) << ' ' << my_round_to_double(intersection_point.y()) << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n) {
    solve(n);
    std::cin >> n;
  }
}