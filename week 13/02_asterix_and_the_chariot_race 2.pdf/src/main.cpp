///4
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <limits>

typedef std::vector<int> IV;
typedef std::vector<long long> LV;

// we are solving a problem of undirected graph in a directed way
// we have to carefully think about the state score we are using

// score_1, the sum of subtree[i] if we repair node[i]
// score_2, if we don't need to repair node[i], also we don't need the parent of i to be repaired, this means we repair at least one of the child of i
// score_3, if we don't repair i or child of i, we need to repair parent of i;

// score_1 = c[i] + sum min(score {1,2,3}) of child
// score_2 = 0 + sum min(score {1,2}) of child + min(delta change of repair to not repair), if i have no child, this value is infinity
// score_3 = 0 + sum score 2 of child, if one of the child have infinity of s2, this must also be infinity

// the result if the min of root's score 1 and 2

void solve(){
  int n; std::cin >> n;
  
  IV parent(n, -1), n_child (n, 0), c (n, 0);
  
  std::vector<IV> child_list (n, IV ());
  
  for (int k=1; k<n; ++k){
    int i, j; std::cin >> i >> j;
    ++n_child[i];
    parent[j] = i;
    child_list[i].push_back(j);
  }
  
  for (int i=0; i<n; ++i) std::cin >> c[i];
  
  LV s1 (n, -1), s2 (n, -1), s3 (n, -1);
  
  long long inf=std::numeric_limits<long long>::max();
  
  std::queue<int> q;
  for (int i=0; i<n; ++i) if (n_child[i]==0) q.push(i);
  
  while (!q.empty()){
    int node=q.front(); q.pop();
    
    // calculate the score of itself
    
    // init value
    s1[node]=c[node];
    s2[node]=0;
    s3[node]=0;
    
    long long delta_min=inf;
    
    for (std::size_t i=0; i<child_list[node].size(); ++i){
      int c=child_list[node][i];
      
      // s1
      s1[node] += std::min(s1[c], std::min(s2[c], s3[c]));
      
      // s2
      s2[node] += std::min(s1[c], s2[c]);
      delta_min = std::min(delta_min, s1[c] - std::min(s1[c], s2[c]));
      
      // s3
      if (s3[node]<inf && s2[c]<inf) s3[node] += s2[c];
      else s3[node]=inf;
    }
    
    s2[node]+=delta_min; // if there is no child node, s2 is infinity, this is correct with our logic
    
    // prepare for its parent
    
    if (parent[node]!=-1){
      int p = parent[node];
      --n_child[p];
      if (n_child[p] == 0) q.push(p);
    }
  }
  
  std::cout << std::min(s1[0], s2[0]) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}