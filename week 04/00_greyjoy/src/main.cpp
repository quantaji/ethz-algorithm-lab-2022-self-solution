///1
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
using namespace std;

void solve() {
  int n, k, w; cin >> n >> k >> w;
  vector<int> c(n); for (int i=0; i<n; ++i) cin >> c[i];
  vector<vector<long>> S(w, vector<long> ());
  vector<int> l(w);
  // first int is the id of water way, second is length
  set<long> value_set;
  multimap<long, pair<int, int>> value_map;
  for (int i=0; i<w; ++i)
  {
    cin >> l[i];
    long sum=0; S[i].push_back(sum);
    int r;
    for (int j=0; j<l[i]; ++j){
      cin >> r;
      sum += c[r];
      S[i].push_back(sum);
      value_map.insert(pair<long, pair<int, int>> (sum, pair<int, int>(i, j+1)));
      if (j>0) value_set.insert(sum);
    }
  }
  int max_islands = 0;
  // first test if there is possible within single way.
  // no data needs to be stored
  for (int i=0; i<w; ++i) {
    int j=0, m=1;
    while (j<l[i]+1) {
      while ((S[i][m] - S[i][j] < k) && (m<l[i]+1)) ++m;
      if ((S[i][m] - S[i][j] == k) && (max_islands < m - j)) max_islands = m - j;
      ++j;
    }
  }
  for (auto it=value_set.begin(); it!=value_set.end() && *(it) <= (k + c[0] + 1)/2; ++it) if (value_set.find(k + c[0] - *(it)) != value_set.end()) {
    long a = *(it), b = k + c[0] - *(it);
    auto range_a = value_map.equal_range(a);
    auto range_b = value_map.equal_range(b);
    int top_a = 0, second_a = 0, top_b = 0, second_b = 0; // denote the top length of given S
    int id_top_a = -1, id_top_b = -1, id_second_a = -1, id_second_b = -1; // denote the id of water way, -1 denote not assigned
    for (auto it_a = range_a.first; it_a!=range_a.second; ++it_a) if (it_a->second.second > top_a) {
      if (second_a < top_a) {
        second_a = top_a; id_second_a = id_top_a;
      }
      top_a = it_a->second.second; id_top_a = it_a->second.first;
    }
    for (auto it_b = range_b.first; it_b!=range_b.second; ++it_b) if (it_b->second.second > top_b) {
      if (second_b < top_b) {
        second_b = top_b; id_second_b = id_top_b;
      }
      top_b = it_b->second.second; id_top_b = it_b->second.first;
    }
    int l = 0;
    if (id_top_a!=id_top_b) l = top_a + top_b - 1;
    else {
      if (id_second_a != -1) l = max(l, second_a + top_b);
      if (id_second_b != -1) l = max(l, second_b + top_a);
    }
    if (l > max_islands) max_islands = l;
  }
  
  cout << max_islands << endl;
}

int main(){
  int t;
  cin >> t;
  for (int i=0; i<t; ++i) solve();
}