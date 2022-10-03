///3
#include <iostream>
#include <vector>
#include <map>

using namespace std;

void solve() {
  int n, m;
  long x;
  int k;
  cin >> n >> m >> x >> k;
  map<int, long> temp_connection = {};
  int u, v; long p;
  bool no_out[n]; 
  for (int i=0; i<n; ++i) {
    no_out[i] = true;
  }
  for (int i=0; i<m; ++i) {
    cin >> u >> v >> p;
    no_out[u] = false;
    // if (u==0) {
    //   zero_out[v] = true;
    // }
    if (temp_connection.find(v*n+u) != temp_connection.end()) {
      if (temp_connection[v*n+u] < p){
        temp_connection[v*n+u] = p;
      }
    } else {
      temp_connection.insert(pair<int, long> (v*n+u, p));
    }
  }
  
  // if there is no out for v, delete it and redirect it to the out of 0;
  map<int, long> connection = {}; //official connection
  for (auto it=temp_connection.begin(); it!=temp_connection.end(); ++it){
    v = it->first / n;
    u = it->first % n;
    p = it->second;
    if (no_out[v]) {
      // no_out_source[u] = true;
      if (connection.find(u) != connection.end()){
        // already exists
        if (connection[u] < p) {
          connection[u] = p;
        }
      } else {
        connection.insert(pair<int, long> (u, p));
      }
    } else {
      if (connection.find(v*n+u) != connection.end()){
        // already exists
        if (connection[v*n+u] < p) {
          connection[v*n+u] = p;
        }
      } else {
        connection.insert(pair<int, long> (v*n+u, p));
      }
    }
  }

  vector<long> old_s, new_s;
  for (int i=0; i<n; ++i) {
    old_s.push_back(-1);
    new_s.push_back(-1);
  }
  old_s[0] = 0;
  int minimum_step = -1;
  for (int iter=0; iter<k; ++iter){
    for (auto it=connection.begin(); it!=connection.end(); ++it){
      v = it->first / n;
      u = it->first % n;
      p = it->second;
      if (old_s[u]!= -1 && new_s[v] < old_s[u] + p){
        new_s[v] = old_s[u] + p;
      }
    }
    old_s.clear();
    for (int i=0; i<n; ++i) {
      old_s.push_back(-1);
      if (new_s[i] >= x) {
        minimum_step = iter+1;
        break;
      }
    }
    if (minimum_step != -1) {
      break;
    }
    old_s.swap(new_s);
  }
  if (minimum_step != -1) {
    cout << minimum_step << endl;
  } else {
    cout << "Impossible" << endl; 
  }
}

int main() {
  int t;
  cin >> t;
  for (int i=0; i<t; ++i) {
    solve();
  }
}