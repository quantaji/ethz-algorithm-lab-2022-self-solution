///1
// std
#include <iostream>
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

void solve(int n, int m) {
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  for (int i=0; i<n; ++i){
    int min_i, max_i; std::cin >> min_i >> max_i;
    // set upper and lower bound for nutrient
    lp.set_b(2*i, -min_i);
    lp.set_b(2*i+1, max_i);
  }
  for (int j=0; j<m; ++j){
    int p_j; std::cin >> p_j;
    lp.set_c(j, p_j);
    for (int i=0; i<n; ++i){
      int C_ji; std::cin >> C_ji;
      lp.set_a(j, 2*i, -C_ji);
      lp.set_a(j, 2*i+1, C_ji);
    }
  }
 
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (!s.is_infeasible()){
    std::cout << round_down(s.objective_value()) << std::endl;
  } else{
    std::cout << "No such diet." << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n, m; std::cin >> n >> m;
  while (n){
    solve(n, m);
    std::cin >> n >> m;
  }
}