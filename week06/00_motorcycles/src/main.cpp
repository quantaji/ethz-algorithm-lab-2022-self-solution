///2
// std
#include <iostream>
#include <vector>
#include <algorithm>
// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Ray_2.h>
#include <CGAL/intersections.h>
// #include <CGAL/parallel.h>
// idea: line with smallest abs(k) always win, we can sort along this directoin
// any futuring line have no intersection with lines of smaller aboule value...
// if |k_A| > |k_B| and A is forever, this means B is stopped before B hit A,
// this means Exists C s.t. |k_C|<=|k_B| and intersect before A, by drawing a line, you can find that
// C and A are definetly going to intersect!
// if we continue this logic, you may think C can also be hide by other line, 
// but this will lead to that there is a biggest D, s.t. D also intersects with A

// another key property is that, when we iterate, a line fails only when it means with lines that have smaller slop,
// at same time, it also lies in the range of y0 of privous slop, so we can keep track of the max and min value, then it is oky, we don't even need to use predicate or ray.

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef std::tuple<int, EK::FT, double, double> Idx_K_Sign; // idx, abs slop, sign of slop, y0

struct Smaller{
  inline bool operator() (const Idx_K_Sign& p1, const Idx_K_Sign& p2){
    // return (std::get<1>(p1) < std::get<1>(p2)) ? true : ((std::get<1>(p1) > std::get<1>(p2)) ? false : (std::get<2>(p1) > 0 && std::get<2>(p2) < 0));
    if (std::get<1>(p1) < std::get<1>(p2)) return true;
    else if (std::get<1>(p1) > std::get<1>(p2)) return false;
    else{
      // abs equal
      if (std::get<2>(p1)*std::get<2>(p2) < 0){
        // abs non-zero, and different sign, return positive one
        if (std::get<2>(p1) > 0) return true;
        else return false;
      } else {
        // abs = 0 or the two k is the same
        // if >=0 return the smaller y0 one, for better sort result
        // if <0 the one with higher y is smaller
        // this ordering is really important!!!!!!!
        if (std::get<2>(p1) >= 0) return std::get<3>(p1) < std::get<3>(p2);
        else return std::get<3>(p1) > std::get<3>(p2);
      }
    }
  }
};

inline EK::FT abs_back(EK::FT abs_k, double sign){
  return (sign >= 0) ? abs_k : - abs_k;
}

void solve(){
  int n; std::cin >> n;
  std::vector<Idx_K_Sign> order_list;
  for(int i=0; i<n; ++i){
    double y0, x1, y1; std::cin >> y0 >> x1 >> y1;
    order_list.push_back(Idx_K_Sign(i, CGAL::abs(EK::FT(y1-y0) / EK::FT(x1)), y1-y0, y0));
  }
  
  std::sort(order_list.begin(), order_list.end(), Smaller());
  
  std::vector<bool> stay(n, true);
  
  // still need some pruning
  double init_y = std::get<3>(order_list[0]);
  double min_y=init_y, max_y=init_y;
  EK::FT init_k = abs_back(std::get<1>(order_list[0]), std::get<2>(order_list[0]));
  EK::FT max_k=init_k, min_k=init_k;
  
  for(int i=1; i<n; ++i){ // outer loop
    int idx = std::get<0>(order_list[i]);
    double sign = std::get<2>(order_list[i]);
    EK::FT k = abs_back(std::get<1>(order_list[i]), sign);
    double y0 = std::get<3>(order_list[i]);
    if (k > max_k){
      if (y0 > max_y) {
        max_k = k; // this forever
      } else {
        stay[idx] = false;
      }
    } else if (k < min_k){
      if (y0 < min_y){
        min_k = k; // forever
      } else {
        stay[idx] = false;
      }
    } // else either = max_k or min_k, do nothing, 
    min_y = std::min(min_y, y0); max_y = std::max(max_y, y0);
    
  }
  
  for (int i=0; i<n; ++i) if (stay[i]) std::cout << i << ' ';
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}