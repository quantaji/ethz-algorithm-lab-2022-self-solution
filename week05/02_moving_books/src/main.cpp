///2
#include <iostream>
#include <map>
#include <cmath>
#include <algorithm>

// the problem can be quantified as
// given two list {s_1, ...., s_n} and {w_1, ..., s_m} (sorted)
// we replicate each s_i with l_i times (l_i can be 0), such that, \sum l_i = m and 
// the extended sorted {s'_1, ... , s'_m} such that {s'} >= {w} in dictionary sense.
// in this sense, given a solution, we can always sort the replicated set. (need to prove, but easy)
// this turns the problem into a geometric one
// given a sequence {w1, ..., w_m} and a set of choise {w}, how can we choose the minimum-replication-time solution

// but how can we decompose this problem?

// one key idea is that, given some w_i such that w_{<=i} > s_j, we can make the optimal solution as equal as possible, and 
// for the next w_{<= i'} > s_{j+1}, we try to make it as equal as possible, by adding one more replicant for each s_{<j}, to make s_i and s_{<j} have same amount of replica.

// prove of optimacy
// seems like the optimicy can be obtained easier.

// if k is the optimal number of maximum replica, then we replicate all if them to see if the sequnce surfice.

// seems ok, done!

void solve(){
  int n, m; std::cin >> n >> m;
  // we need two thing: the number of distince value of s, and the number of w in between and next s
  std::map<int, std::pair<int, int>> strengths; // key: stengthn, value: first: number of workers of strenght w, second: number of loads in between
  for (int i=0; i<n; ++i){
    int s; std::cin >> s;
    if (strengths.find(s) == strengths.end()) {
      strengths[s] = std::pair<int, int> (1, 0);
    }
    else strengths[s].first += 1;
  }
  
  bool impossible=false;
  
  for (int i=0; i<m; ++i){
    int w; std::cin >> w;
    auto iter = strengths.lower_bound(w);
    if (iter == strengths.end()){
      impossible = true;
    } else {
      iter->second.second += 1;
    }
  }
  
  // impossible, there is a weight that no body can lift
  if (impossible) std::cout << "impossible" << std::endl;
  else{
    int min_replicant=0; // number of total time up and down
    int current_worker=0; // if current_worker*min_replicant > total number of w, then it is possible to fulfil the task
    double current_total_weights=0;
    
    for (auto iter = strengths.rbegin(); iter != strengths.rend(); ++iter){
      current_worker += iter->second.first;
      current_total_weights += iter->second.second;
      min_replicant = std::max(min_replicant, int(std::ceil(current_total_weights / current_worker)));
    }
    std::cout << 3*min_replicant - 1 << std::endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin>>t; for (int i=0; i<t; ++i) solve();
}