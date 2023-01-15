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

int round_up(int p, int q){
  // int sign_p = (p >= 0) ? 1: -1;
  int sign_q = (q >= 0) ? 1: -1;
  int result = p/q;
  while(result * q * sign_q >= p * sign_q){
    --result;
    // finally result < p/q
  }
  while(result * q * sign_q < p * sign_q){
    ++result;
    // finally result >= p/q
  }
  return result;
}

void solve(int p)
{
  int a, b; std::cin >> a >> b;
  Program lp; 
  const int X = 0; 
  const int Y = 1;
  const int Z = 2;
  if (p==1){
    lp = Program(CGAL::SMALLER, true, 0, false, 0);
    lp.set_a(X, 0, 1);  lp.set_a(Y, 0, 1); lp.set_b(0, 4);
    lp.set_a(X, 1, 4);  lp.set_a(Y, 1, 2); lp.set_b(1, a*b);
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);
    lp.set_c(X, a);     lp.set_c(Y, -b);
  } else if (p==2){
    lp = Program(CGAL::SMALLER, false, 0, true, 0);
    lp.set_a(X, 0, -1); lp.set_a(Y, 0, -1);                     lp.set_b(0, 4);
    lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b);
    lp.set_a(X, 2, 1);  lp.set_a(Y, 2, -1);                     lp.set_b(2, 1);
    lp.set_c(X, a);     lp.set_c(Y, b);     lp.set_c(Z, 1);
  } else assert(false);
 
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (!s.is_infeasible()){
    if (s.is_unbounded()) std::cout << "unbounded" << std::endl;
    else {
      int m = int(s.objective_value_numerator().to_double()), n = int(s.objective_value_denominator().to_double());
      // std::cout << m << ' ' << n << std::endl;
      int result = round_up(m, n);
      if (p==1) std::cout << -result << std::endl;
      else std::cout << result << std::endl; 
    }
  } else{
    std::cout << "no" << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int p; std::cin >> p;
  while (p){
    solve(p);
    std::cin >> p;
  }
}