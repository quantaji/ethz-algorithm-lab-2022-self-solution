///
// std
#include <iostream>
#include <algorithm>
// CGAL
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int round_down(CGAL::Quotient<ET> a){
  // since in this question, maximum amount is 2^22, it is precise to convert from p/q to double;
  double result = int(CGAL::to_double(a));
  while (result <= a) ++result;
  while (result > a) --result;
  return int(result);
}

int round_closest_square(int sq){
  int a = int(std::sqrt(double(sq)));
  while(a*a < sq) ++a;
  while(a*a >= sq) --a; // a*a < sq now, but the next while be bigger or equal
  if (std::abs(a*a-sq) <= std::abs((a+1)*(a+1)-sq)) return a;
  else return a+1;
}

void solve(int n) {
  // key idea: sum_i a_i x_i + r <= b_i, r>= 0, max r
  
  int d; std::cin >> d; // r is denoted as d-th dim, while others 0the ,...,  d-1th, dim
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  lp.set_l(d, true, 0); lp.set_c(d, -1);
  for (int i=0; i<n; ++i){
    int sq_sum = 0;
    for (int j=0; j<d; ++j){
      int a_i; std::cin >> a_i;
      lp.set_a(j, i, a_i);
      sq_sum += a_i*a_i;
    }
    lp.set_a(d, i, round_closest_square(sq_sum));
    int b_i; std::cin >> b_i;
    lp.set_b(i, b_i);
  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  if (!s.is_infeasible()){
    if (s.is_unbounded()){
      std::cout << "inf" << std::endl;
    } else {
      std::cout << round_down(-s.objective_value()) << std::endl;
    }
  } else{
    std::cout << "none" << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n){
    solve(n);
    std::cin >> n;
  }
}