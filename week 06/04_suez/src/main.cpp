///4
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
// CGAL
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// it is efficient to only consider the old that is "closest to overlap"

long long round_up(CGAL::Quotient<ET> a){
  // the un-rounded result is 2*n*(h+w)*r
  double result = (long long)(CGAL::to_double(a));
  while (result>=a) --result;
  while (result<a) ++result;
  return (long long)(result);
}

bool rational_lower(long long p1, long long q1, long long p2, long long q2){
  // if q2 = 0, then p2 > 0 denotes +inf, p2 < 0 denotes -inf
  long long sign_q1 = (q1>=0) ? true : false;
  long long sign_q2 = (q2>=0) ? true : false;
  return p1*q2*sign_q1*sign_q2 < p2*q1*sign_q1*sign_q2;
}

void solve(){
  long long n, m, h, w; std::cin >> n >> m >> h >> w;
  std::vector<long long> x_o, x_n, y_o, y_n, d1, d2;
  for (int i=0; i<n; ++i){
    long long x, y; std::cin >> x >> y;
    x_n.push_back(x);
    y_n.push_back(y);
    d1.push_back(x*h+y*w);
    d2.push_back(x*h-y*w);
  }
  for (int i=0; i<m; ++i){
    long long x, y; std::cin >> x >> y;
    x_o.push_back(x);
    y_o.push_back(y);
  }
  
  std::vector<int> p (n, -1); // record the closest old to new, in form of p and q
  std::vector<int> q (n, -1);
  long long dl, l; 
  for (int i=0; i<n; ++i){
    p[i]=1; q[i]=0; // initially r_i = p_i/q_i is infinity
    // (1+r)*l/2 <= dl r <= 2*dl/l - 1 = (2*dl - l)/l
    for (int j=0; j<m; ++j){
      // old new comparision
      long long dx=std::abs(x_o[j] - x_n[i]), dy=std::abs(y_o[j] - y_n[i]);
      if (rational_lower(dy, h, dx, w)) {
        // constraint is on x direction
        dl=dx; l=w; 
      } else {
        // constraint is on y direction
        dl=dy; l=h;
      }
      if (rational_lower(2*dl-l, l, p[i], q[i])) {
        p[i]=2*dl-l; q[i]=l;
      }
    }
  }
  
  
  // also need some pruning for new new pair, for new point (i, j, k), 
  // if rectangle k lies out side the Parallelogram formed by diagnal line of i and j, then it is ok
  std::vector<std::vector<bool>> possible (n, std::vector<bool> (n, true));
  for (int i=0; i<n-1; ++i){
    // condition for ok is (d1k +(w*h)< min(d1i, dij) or d1k > max(d1i, d1j)) or d2...
    for(int j=i+1; j<n; ++j){
      for (int k=0; k<n; ++k) if (k!=i && k!=j){
        if (!( d1[k] + w*h <= std::min(d1[i], d1[j]) || d1[k] - w*h >= std::max(d1[i], d1[j]) || d2[k] + w*h <= std::min(d2[i], d2[j]) || d2[k] - w*h >= std::max(d2[i], d2[j])  )){
          possible[i][j] = false;
          possible[j][i] = false;
          break;
        }
      }
    }
  }
  
  
  // prepare for LP
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  int eq_id=0;
  for(int i=0; i<n; ++i){
    // set minimization constant
    lp.set_c(i, -2*(h+w));
    // set lower bound
    lp.set_l(i, true, 1);
    // the equation for old and new
    // p*r_i <= q
    lp.set_a(i, eq_id, q[i]); lp.set_b(eq_id, p[i]); ++eq_id;
  }
  for (int i=0; i<n; ++i){
    for (int j=i+1; j<n; ++j) if (possible[i][j]) {
      // constrain is (r_i + r_j)*l/2 < dl
      long long dx=std::abs(x_n[j] - x_n[i]), dy=std::abs(y_n[j] - y_n[i]);
      if (rational_lower(dy, h, dx, w)) {
        dl=dx; l=w;
      } else {
        dl=dy; l=h;
      }
      // set constraints
      lp.set_a(i, eq_id, l); lp.set_a(j, eq_id, l); lp.set_b(eq_id, 2*dl); ++eq_id;
    }
  }
  CGAL::Quadratic_program_options options; 
  options.set_pricing_strategy(CGAL::QP_BLAND);
  Solution s = CGAL::solve_linear_program(lp, ET(), options);
  // Solution s = CGAL::solve_linear_program(lp, ET());
  std::cout << round_up(-s.objective_value()) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}