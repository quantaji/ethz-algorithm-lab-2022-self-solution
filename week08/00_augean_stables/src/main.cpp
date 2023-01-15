///4
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
// CGAL
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void solve(){
  int n; std::cin >> n;
  std::vector<int> f(n, -1), c(n, -1), k(n, -1), l(n, -1), m(n, -1);
  for (int i=0;i<n;++i){
    std::cin >> f[i] >> c[i] >> k[i] >> l[i] >> m[i];
  }
  std::vector<int> s_a {0}, s_p {0};
  std::map<int, int> map_a, map_p; // map S to number of sum
  int sum_a=0, sum_p=0;
  
  map_a[0]=0; map_p[0]=0;
  for (int i=0; i<24; ++i){
    int a_i; std::cin >> a_i;
    if (a_i>0) {
      sum_a += a_i;
      s_a.push_back(sum_a);
      map_a[sum_a]=1+i;
    }
  }
  for (int i=0; i<24; ++i){
    int p_i; std::cin >> p_i;
    if (p_i>0) {
      sum_p += p_i;
      s_p.push_back(sum_p);
      map_p[sum_p]=1+i;
    }
  }
  
  // pruning 
  std::vector<bool> support(n, true);
  for (int i=0; i<n-1; ++i) for (int j=i+1; j<n; ++j) if(support[i] && support[j]){
    if(k[j] >= k[i] && l[j] >= l[i] && m[j] >= m[i] && f[j]-c[j] <= f[i]-c[i]){
      support[j]=false;
    } else if (k[j] <= k[i] && l[j] <= l[i] && m[j] <= m[i] && f[j]-c[j] >= f[i]-c[i]){
      support[i]=false;
    }
  }
  // int sum=0; for(int i=0;i<n;++i) if(support[i]) ++sum; 
  // std::cout << sum << '/' << n << std::endl; 
  
  // you can first check if it is impossible or not, it is faster but the code would be ugly...
  // if a_1 > a_2, then p(a_1) <= p(a_2), then a single O(24) algoirhtm suffice
  bool feasible=false; int min_hour=50;
  auto ia=s_a.begin(), sa=s_a.end();
  auto ip=s_p.rbegin(), sp=s_p.rend();
  
  while(ia!=sa && ip!=sp){
    Program lp (CGAL::SMALLER, true, 0, true, 1); int eq_id=0;
    for (int i=0; i<n; ++i){
      int k_i=k[i]+(*ia)*(*ia);
      int l_i=l[i]+(*ip)*(*ip);
      int m_i=m[i]+(*ia)*(*ip);
      lp.set_a(0, eq_id, -k_i); lp.set_a(1, eq_id, -l_i); lp.set_a(2, eq_id, -m_i);
      lp.set_b(eq_id, c[i]-f[i]);
      ++eq_id;
    }
    // CGAL::Quadratic_program_options options; 
    // options.set_pricing_strategy(CGAL::QP_BLAND);
    // Solution s = CGAL::solve_linear_program(lp, ET(), options);
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()){
      // no solution increase a
      ++ia;
    } else {
      // have solution reduce p
      feasible=true;
      min_hour = std::min(min_hour, map_a[(*ia)]+map_p[(*ip)]);
      ++ip; // !!! IMPORTANT not --ip; !!!
    }
  }
  if (!feasible) std::cout << "Impossible!" << std::endl;
  else std::cout << min_hour << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}