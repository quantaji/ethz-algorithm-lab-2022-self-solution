///3
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <numeric>

typedef std::pair<long long, long long> LN;
typedef std::pair<int, LN> IdxLN;
typedef std::vector<int> IV;
typedef std::vector<long long> LV;

struct Smaller{
  inline bool operator () (const IdxLN & a, const IdxLN & b) {
    return a.second.first * b.second.second < b.second.first * a.second.second;
    // return a.second.first < b.second.first ;
    // return b.second.second < a.second.second;
  }
};

// typedef std::set<IdxLN, Smaller> ChildSet;
typedef std::vector<IdxLN> OrderedChildV;

void solve(){
  int n; std::cin >> n;
  
  IV n_c (n+1, 0), n_c_total (n+1, 0), parent(n+1, -1);
  LV g={0}, l(n+1, 0), l_c_total (n+1, 0);
  // std::vector<ChildSet> c_set (n+1, ChildSet ());
  std::vector<OrderedChildV> c_o_vec(n+1, OrderedChildV());
  std::vector<IV> c_vec (n+1, IV ());
  
  // get input and make some stats on data
  for (int i=0; i<n; ++i){
    long long g_i; std::cin >> g_i; g.push_back(g_i);
  }
  
  for (int i=0; i<n; ++i){
    int u, v; long long l_i; std::cin >> u >> v >> l_i;
    l[v]=l_i;
    c_vec[u].push_back(v); 
    ++n_c[u];
    parent[v]=u;
  }
  

  // using a queue to do bfs from bottom to get the n_total
  std::queue<int> q;
  for (int i=0; i<n+1; ++i) if (n_c[i]==0) q.push(i);
  
  while(!q.empty()){
    int node=q.front(); q.pop();
    // number of child including node and get the total lenth
    long long temp_l = l[node], temp_n=1;
    for (auto iter=c_vec[node].begin(); iter!=c_vec[node].end(); ++iter) {
      temp_l += l_c_total[*iter];
      temp_n += n_c_total[*iter];
    }

    l_c_total[node] = temp_l;
    n_c_total[node] = temp_n;
    // get prepared for its parent node
    if (parent[node]!=-1){
      --n_c[parent[node]];
      if (n_c[parent[node]]==0) q.push(parent[node]);
    }
  }
  
  // std::cout << (n_c_total[0] == n+1) << ' ' <<  n_c_total[0] << ' ' <<  n << ' ' << (l_c_total[0] == (long long)std::accumulate(l.begin(), l.end(), 0)) << std::endl;
  
  // for each node get its children nodes' order of visiting
  for (int i=0; i<n+1; ++i){
    for (auto iter=c_vec[i].begin(); iter!=c_vec[i].end(); ++iter){
      int c=*iter;
      // c_set[i].insert(IdxLN(c, LN (l_c_total[c], n_c_total[c])));
      c_o_vec[i].push_back(IdxLN(c, LN (l_c_total[c], n_c_total[c])));
    }
    std::sort(c_o_vec[i].begin(), c_o_vec[i].end(), Smaller());
  }
  
  // debug
  // for (int i=0; i<n+1; ++i) std::cout << i << ' ' << l_c_total[i] << ' ' << n_c_total[i] << std::endl;
  
  LV visit_time (n+1, 0); // defines the first time of visiting
  // use the same queue since it is already cleared
  long long result = 0;
  int root=0; q.push(root);
  while(!q.empty()){
    int node=q.front(); q.pop();
    // first get result
    result += g[node] - visit_time[node];
    // if (node != root && g[node] - visit_time[node] <= 0) std::cout << '!' << std::endl;
    // for each node
    long long temp_time=visit_time[node];
    for (auto iter=c_o_vec[node].begin(); iter!=c_o_vec[node].end(); ++iter){
      // std::cout <<iter->second.first << '/' << iter->second.second << '='<< double(iter->second.first)/ double(iter->second.second) << ' ';
      // if(n_c_total[node] > 32) std::cout << double(iter->second.first)/ double(iter->second.second) << ' ';
      int c=iter->first;
      // get the accumulated time
      visit_time[c] = temp_time + l[c];
      temp_time += 2*l_c_total[c];
      // add the child into queue
      q.push(c);
    }
    // if(n_c_total[node] > 32) std::cout << std::endl;
  }
  std::cout << result << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}