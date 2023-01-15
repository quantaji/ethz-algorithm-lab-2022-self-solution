///1
// std
#include <iostream>
#include <algorithm>
#include <vector>
// CGAL
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/enum.h>
#include <CGAL/intersections.h>

// boost
#include "boost/variant.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
// typedef K::Triangle_2 T;
typedef K::Line_2 L;

// idea:
// explicitly define a triangle is not efficient, we can achieve this all using predicates
// a point is inside or on edge/vertex of a triangle if for all the edges, the point is not at different side of that triangle.
// then we can use epic

inline bool not_on_different_side(const P& l_p1, const P& l_p2, const P& tri_p, const P& test_p){
  return CGAL::orientation(l_p1, l_p2, test_p) == CGAL::COLLINEAR || CGAL::orientation(l_p1, l_p2, tri_p) == CGAL::orientation(l_p1, l_p2, test_p);
}

void solve(){
  int m, n; std::cin >> m >> n;
  std::vector<P> leg_points;
  for (int i=0; i<m; ++i)
  {
    int x, y; std::cin >> x >> y;
    leg_points.push_back(P(x, y));
  }
  // calculate belong relationship
  std::vector<std::vector<int>> contain_list (n, std::vector<int>{});
  std::vector<bool> inside(m, false);
  
  for (int i=0; i<n; ++i){
    int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
    std::cin >> x1 >> y1; P l1p1=P(x1, y1);
    std::cin >> x2 >> y2; P l1p2=P(x2, y2);
    std::cin >> x3 >> y3; P l2p1=P(x3, y3);
    std::cin >> x4 >> y4; P l2p2=P(x4, y4);
    std::cin >> x5 >> y5; P l3p1=P(x5, y5);
    std::cin >> x6 >> y6; P l3p2=P(x6, y6);

    for (int l=0; l<m; ++l) 
      if (not_on_different_side(l1p1, l1p2, l2p1, leg_points[l]) && not_on_different_side(l2p1, l2p2, l3p1, leg_points[l]) && not_on_different_side(l3p1, l3p2, l1p1, leg_points[l])) 
        inside[l]=true;
      else inside[l]=false;
    for (int l=0; l<m-1; ++l) 
      if (inside[l] && inside[l+1])
        contain_list[i].push_back(l);
  }

  // O(m-1) iteration
  int total_number = 0; // denote the total number of legs inside the current bulk
  std::vector<int> map_on_leg (m-1, 0);
  int b=0, e=0, min_cost = n;
  while (b<n && e<n){
    while (total_number < m-1 && e<n) {
      for (auto it=contain_list[e].begin(); it!=contain_list[e].end(); ++it){
        if (map_on_leg[*it] == 0) ++total_number;
        ++map_on_leg[*it];
      }
      ++e;
    }
    while (total_number == m-1 && b<e) {
      min_cost = std::min(min_cost, e-b);
      for (auto it=contain_list[b].begin(); it!=contain_list[b].end(); ++it){
        if (map_on_leg[*it] == 1) --total_number;
        --map_on_leg[*it];
      }
      ++b; // there are possiblities that when you ++b, the total_number is still m-1, and this can happen for the last point. therefore, we cannot decrease b once a time, we should use while.
    }
  }
  std::cout << min_cost << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int c; std::cin >> c;
  for (int i=0; i<c; ++i) solve();
}