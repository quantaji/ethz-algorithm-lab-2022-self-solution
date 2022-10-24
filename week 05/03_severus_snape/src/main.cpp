///1
#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
// This problem can be  formulated into a Knapsack problem, and have pseudo-polynomial time solution
// denote A_i = {0, 1} and B_i = {0, 1}, nA and nB as total number of A and B posion, the condition we need is 
// (1) \sum_i A_i * h_i >= H
// (2) \sum_i A_i * p_i >= P + b * nB
// (3) \sum_i B_i * w_i >= W + a * nA

// One possible way is that we enumerate over all possible (nA, nB) choice
// for B, fixing nA is a easy problem, we just need to sort B in descending way, and we can get an upper bound of nA.
// for A, this is a non-typical Knapsack problem
// we have to find choosing nA posion, and sum of happiness no less than H, what is the maximum number of Power, this array, P(nA)

// Denote D(j, l, H) as the maximum power you can get by choosing l from first j items under the constraint of total happiness over H
// D(j, l, H) = max(D(j-1, l, H), D(j-1, l-1, max(H-h[j], 0)) + p[j])
// D(j, >j, H) = -1,
// D(j, 0, H) = 0
// D(j, l, 0) = no constraint result

// The outer loop should be j
//    inner loop should be H, then l
//    we need to store two, old j-1's Hx(j-1) and new j's Hx(j), this is computationally plaussible


void solve(){
  int n, m; std::cin >> n >> m;
  long long a, b; std::cin >> a >> b;
  long long P, W; int H; std::cin >> P >> H >> W;
  std::vector<int> h_list;
  std::vector<long long> p_list, w_list;
  
  for(int i=0; i<n; ++i){
    long long p_i; int h_i; std::cin >> p_i >> h_i;
    p_list.push_back(p_i);
    h_list.push_back(h_i);
  }
  
  for(int j=0; j<m; ++j){
    long long w_j; std::cin >> w_j;
    w_list.push_back(w_j);
  }
  
  // at the begining j=0
  // D(0, 0, H) = 0
  std::vector<std::vector<long long>> old, current; // the first dimension is for l, second for H
  old.push_back(std::vector<long long> (H+1, 0));
  // now start iteration, j=2, old stand for j=1
  for (int j=1; j<=n; ++j){
    current.push_back(std::vector<long long> (H+1, 0));
    current.push_back(std::vector<long long> (H+1, 0));
    for(int h=0; h<=H; ++h){
      for(int l=0; l<=j; ++l){
        // need to rewrite the logic
        if (l==0) continue; // we don't what to deal with it, so complicated, if we meet l==1 later, we deal with it specifically
        else if (l==1){
          if (j==1) current[l][h] = (h_list[j-1] >= h) ? p_list[j-1] : -1;
          else {
            // j>1 != l
            long long option_A = old[l][h];
            long long option_B = (h_list[j-1] >= h) ? p_list[j-1] : -1;
            current[l][h] = std::max(option_A, option_B); 
          }
        } else{
          // l > 1
          long long option_A = (l == j) ? -1  : old[l][h]; // if l==j, we cannot choose option A, since we cannot choose j+1 items from j items
          long long option_B;
          if (old[l-1][std::max(0, h - h_list[j-1])] == -1) option_B = -1; // if previous option is not possible
          else option_B = old[l-1][std::max(0, h - h_list[j-1])] + p_list[j-1]; // if possible then continue
          current[l][h] = std::max(option_A, option_B); 
        }
      }
    }
    old.swap(current);
  }
  // old[l][H] is the thing we want
  // for (int i=0; i<=n; ++i) std::cout << old[i][H] << ' ';

  std::sort(w_list.begin(), w_list.end());
  std::vector<long long> s_w; long long w_sum = 0;
  s_w.push_back(w_sum);
  for (auto iter = w_list.rbegin(); iter!=w_list.rend(); ++iter){
    w_sum += *iter;
    s_w.push_back(w_sum);
  }
  bool impossible = true; int min_num = n+m;
  for (int i=1; i<=n; ++i){
    if (old[i][H] == -1) continue;
    for (int j=1; j<=m; ++j){
      if (old[i][H] >= P + b*j && s_w[j] >= W + a*i){
        impossible = false;
        min_num = std::min(min_num, i + j);
      }
    }
  }
  if (impossible)
    std::cout << -1 << std::endl;
  else
    std::cout << min_num << std::endl;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}