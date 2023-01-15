///4
#include <iostream>
#include <vector>
using namespace std;

void solve() {
  int n, m;
  cin >> n >> m;
  int r, b;
  cin >> r >> b;
  int posi_to_key[n];
  for (int i=0; i<n; ++i) {
    posi_to_key[i] = -1;
  }
  vector<vector<int>> connection;
  int u, v, key=0;
  for (int i=0; i<m; ++i){
    cin >> u >> v;
    if (posi_to_key[u-1] == -1) {
      connection.push_back(vector<int> ());
      posi_to_key[u-1] = key;
      ++key;
    } 
    connection[posi_to_key[u-1]].push_back(v-1);
  }
  int min_max[n], max_min[n];
  // min_max is the time for you to win
  // max_min is the largest time for enemy to win, meaning the largest minimum turn for you to win.
  // n+1 denote impossible route
  for(int i=0; i<n-1; ++i){
    min_max[i] = n+1; max_min[i] = n+1; // all points are assumed to be not accessable
  }
  min_max[n-1] = 0; max_min[n-1] = 0;
  int min_max_temp, max_min_temp;
  for (int i=n-2; i>=0; --i) if (posi_to_key[i] != -1) {
    min_max_temp = n+1;
    max_min_temp = 0;
    for(auto it = connection[posi_to_key[i]].begin(); it!=connection[posi_to_key[i]].end(); ++it){
      int j = *it;
      if (max_min[j] <= n && min_max_temp > max_min[j]+1) {
        min_max_temp = max_min[j]+1;
      }
      if (min_max[j] == n+1) {
        max_min_temp = n+1;
      } else if (max_min_temp < min_max[j]+1) {
        max_min_temp = min_max[j]+1;
      }
    }
    min_max[i] = min_max_temp;
    max_min[i] = max_min_temp;
  }
  // cout << n << ' ' << min_max[r-1] << ' ' << min_max[b-1] << ' ';
  if (min_max[r-1] == min_max[b-1]) {
    if (min_max[r-1] % 2 == 0) {
      // odd turn, moriaty wins
      cout << 1 << endl;
    } else {
      cout << 0 << endl;
    }
  } else if (min_max[r-1] < min_max[b-1]) {
    cout << 0 << endl;
  } else {
    cout << 1 << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  for (int i=0; i<t; ++i) {
    solve();
  }
}