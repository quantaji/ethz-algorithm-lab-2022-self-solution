#include <iostream>
#include <queue>

void solve() {
  int n, m, v;
  std::cin >> n >> m >> v;
  int a, b;
  int edge[n][n];
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++){
      edge[i][j] = 0;
    }
  }
  for (int i=0; i<m; ++i) {
    std::cin >> a >> b;
    edge[a][b] = 1;
    edge[b][a] = 1;
  }
  std::queue<int> search_queue; 
  // first denote itself, second denote its origin
  int length[n];
  for (int i=0; i<n; i++) {
    length[i] = -1;
  }
  length[v] = 0;
  search_queue.push(v);
  while (!search_queue.empty()) {
    int current = search_queue.front();
    search_queue.pop();
    for (int i=0; i<n; i++) {
      if ((edge[current][i] ==1) &&  (length[i] == -1)) {
        length[i] = length[current] + 1;
        search_queue.push(i);
      }
    }
  }
  for (int i=0; i<n; ++i) {
    std::cout << length[i] << ' ';
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