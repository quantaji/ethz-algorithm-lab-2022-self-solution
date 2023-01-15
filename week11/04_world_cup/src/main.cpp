///4
// stl
#include <iostream>
#include <algorithm>
// triangulation
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
// lp
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Delaunay;
typedef Delaunay::Edge_iterator  Edge_iterator;
typedef Delaunay::Vertex_handle VH;
typedef K::Point_2 P;
typedef std::vector<P> PV;
typedef std::vector<int> IV;
typedef std::vector<bool> BV;
typedef std::vector<double> DV;

double d2(const P&a, const P&b){
  return (a.x()-b.x())*(a.x()-b.x()) + (a.y()-b.y())*(a.y()-b.y());
}

int hamming_d(const BV &h1, const BV&h2){
  int result=0;
  for (int i=0; i<h1.size(); ++i){
    if (h1[i]!=h2[i]) ++result;
  }
  return result;
}

long long round_down(CGAL::Quotient<ET> a){
  double result = (long long)(CGAL::to_double(a));
  while (result <= a) ++result;
  while (result > a) --result;
  return (long long)(result);
}

void solve(){
  int n, m, c; std::cin >> n >> m >> c;
  PV ws_points, cc_points;
  IV s, a, d, u; DV cr2;
  for (int i=0; i<n;++i){
    int x, y, s_i, a_i; std::cin >> x >> y >> s_i >> a_i;
    ws_points.push_back(P(x, y));
    s.push_back(s_i);
    a.push_back(a_i);
  }
  for (int i=0; i<m; ++i){
    int x, y, d_i, u_i; std::cin >> x >> y >> d_i >> u_i;
    ws_points.push_back(P(x, y));
    d.push_back(d_i);
    u.push_back(u_i);
  }
  std::vector<IV> rev(n, IV (0));
  for (int i=0; i<n; ++i){
    for (int j=0; j<m; ++j){
      int r_ij; std::cin >> r_ij;
      rev[i].push_back(r_ij);
    }
  }
  
  Delaunay t; t.insert(ws_points.begin(), ws_points.end());
  for (int i=0; i<c; ++i){
    int x, y; double r; std::cin >> x >> y >> r;
    P cc (x, y);
    if (d2(cc, t.nearest_vertex(cc)->point())<=r*r){
      cc_points.push_back(cc);
      cr2.push_back(r*r);
    }
  }
  
  std::vector<BV> hamming_list (ws_points.size(), BV(0));
  
  for (int i=0; i<ws_points.size(); ++i){
    for (int j=0; j<cr2.size(); ++j){
      hamming_list[i].push_back(d2(ws_points[i], cc_points[j]) <= cr2[j]);
    }
  }
  
  Program lp(CGAL::SMALLER, true, 0, false, 0);
  // there are three kind of constrains
  // demand exact constrain id 0 ~ 2m-1
  // alchohol upper constrain id 2m ~ 3m
  // supply upper constrain 3m ~ 3m+n
  for (int i=0; i<n; ++i){
    for (int j=0; j<m; ++j){
      int var=i*m+j;
      lp.set_a(var, j, 1); // demand constraint 1
      lp.set_a(var, m+j, -1); // demand constraint 2
      lp.set_a(var, 2*m+j, a[i]); // alchohol constraint
      lp.set_a(var, 3*m+i, 1); // supply constraint
      int actual_reveneu = 100*rev[i][j]-hamming_d(hamming_list[i],hamming_list[n+j]);
      lp.set_c(var, -actual_reveneu);
    }
  }
  lp.set_c0(0);
  
  for (int j=0; j<m; ++j){
    lp.set_b(j, d[j]);
    lp.set_b(m+j, -d[j]);
    lp.set_b(2*m+j, 100*u[j]);
  }
  
  for (int i=0; i<n; ++i){
    lp.set_b(3*m+i, s[i]);
  }
  
  Solution sol = CGAL::solve_linear_program(lp, ET());
  if(sol.is_infeasible()){
    std::cout << "RIOT!" << std::endl;
  } else {
    std::cout << round_down(-sol.objective_value()/100) << std::endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}