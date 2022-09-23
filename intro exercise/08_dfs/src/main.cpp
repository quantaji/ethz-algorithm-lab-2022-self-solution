#include <iostream>

void solve() {
  int n, m, v;
  std::cin >> n >> m >> v;
  int edge[n][n], visited[n], t_start[n], t_end[n], prev[n];
  for (int i=0; i<n; ++i) {
    t_start[i] = -1;
    t_end[i] = -1;
    prev[i] = -1;
    visited[i] = 0;
    for (int j=0; j<n; j++) {
      edge[i][j] = 0;
    }
  }
  int a, b;
  for (int i=0; i<m; ++i) {
    std::cin >> a >> b;
    edge[a][b] = 1;
    edge[b][a] = 1;
  }
  int timestep = 0;
  int current_v = v;
  t_start[v] = 0;
  while (current_v != -1) {
    visited[current_v]=1;
    int i=0;
    while (i<n) {
      if ((edge[current_v][i]==1) && (visited[i] == 0)) {
        break;
      } 
      ++i;
    }
    if (i<n) {
      // this not visited
      prev[i] = current_v;
      current_v = i;
      ++timestep;
      t_start[current_v] = timestep;
    } else {
      // all vertices visited
      ++timestep;
      t_end[current_v] = timestep;
      current_v = prev[current_v];
    }
  }

  for (int i=0; i<n; i++) {
    std::cout << t_start[i] << ' ';
  }
  std::cout << std::endl;
  for (int i=0; i<n; i++) {
    std::cout << t_end[i] << ' ';
  }
  std::cout << std::endl;
}

int main () {
  int t;
  std::cin >> t;
  for (int i=0; i<t; ++i) {
    solve();
  }
}