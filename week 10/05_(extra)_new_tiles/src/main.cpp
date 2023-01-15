///1
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <bitset>
typedef std::vector<int> IV;
typedef std::map<int, int> IIM;
// idea
// the slides is not so clear
// given input, a line of {0,1} possible tile position vector, denoted as s_i
// we can generate all its possible outcome
// denoted as t_ij, the actual tiles we are using, and o_i the outcome.
// o_ij = s_i - t_ij
// because the number of possible tiles is limited, and usually smaller than w, so we use a vector of these vector to store
// we can do this for s_i and previous line s_{i-1}
// we can use a map to store the maximum value
// the value can be transfered only if t_ij is also compatible for some o_{i-1,j}

// for all state we use an int to represent

void generate_outcome(IV & o_l, IV & t_l, int s, int w, int cur_o, int cur_w, int cur_t) {
  // o_l outcome list
  // t_l tile list
  // s state
  // w width
  // cur_o current outcome
  // initially cur_w=0, cur_o = 0, cur_t = 0
  // recursively each time, +cur_w+, if cur_w == w, output
  if (cur_w == w){
    o_l.push_back(cur_o);
    t_l.push_back(cur_t);
  } else {
    // cur_w < w
    // test the current location is possible or not
    if ((s>>cur_w)%2==1 && (s>>(cur_w+1))%2==1){
      // both current and next position is 1
      // then we tile here
      int new_o = cur_o; // because if we tile, it is empty
      int new_t = cur_t + (1<<cur_w) + (1<<(cur_w+1)); // add tile to the corresponding position
      generate_outcome(o_l, t_l, s, w, new_o, cur_w+2, new_t);
    }
    // not tile-able, or we choose not to tile
    // move to next position
    int new_o = cur_o + ((s>>cur_w)%2)*(1<<cur_w); // copy the state of s at cur_w position
    int new_t = cur_t; // no tile operation
    generate_outcome(o_l, t_l, s, w, new_o, cur_w+1, new_t);
  }
}

bool compatible(int s, int t, int w){
  bool comp=true;
  for (int i=0; i<w; ++i){
    if ((t>>i)%2==1 && (s>>i)%2==0){
      // the place where we need state s to have possible position, but it not
      comp=false;
      break;
    }
  }
  return comp;
}

int num_tile(int t, int w){
  int n=0;
  for (int i=0; i<w; ++i){
    n += (t>>i)%2;
  }
  if (n%2==1) std::cout << "wtf?" << std::endl;
  return n/2;
}

void solve(){
  int h, w; std::cin >> h >> w;
  
  // first input
  int init_s=0;
  for (int i=0; i<w; ++i){
    int possible; std::cin >> possible;
    init_s += possible*(1<<i);
  }
  
  IV prev_o_l, cur_o_l, cur_t_l;
  IIM prev_m, cur_m;
  
  // for init state
  cur_o_l.push_back(init_s);
  cur_m[init_s] = 0;
  
  for (int j=1; j<h; ++j){
    // get state
    int state=0;
    for (int i=0; i<w; ++i){
      int possible; std::cin >> possible;
      state += possible*(1<<i);
    }
    
    // handle with privous vectors
    prev_m.swap(cur_m);
    prev_o_l.swap(cur_o_l);
    cur_o_l.clear();
    cur_t_l.clear();
    
    // calculate state
    generate_outcome(cur_o_l, cur_t_l, state,  w, 0, 0, 0);
    
    // std::cout << "state: " << std::bitset<17>(state) << std::endl;
    
    // do the maximization
    for (std::size_t i=0; i<cur_o_l.size(); ++i){
      
      int o=cur_o_l[i], t=cur_t_l[i];
      
      // std::cout << std::bitset<17>(o) << ' ' << std::bitset<17>(t) << ' ' << num_tile(t, w) << std::endl;
      
      for (auto iter=prev_m.begin(); iter!=prev_m.end(); ++iter){
        
        int p_o=iter->first, p_max_tile=iter->second;
        
        if (compatible(p_o, t, w)){
          if (cur_m.find(o) == cur_m.end()){
            cur_m[o] = p_max_tile + num_tile(t, w);
          } else {
            cur_m[o] = std::max(cur_m[o], p_max_tile + num_tile(t, w));
          }
          
          // std::cout << std::bitset<17>(p_o) << ' ' << std::bitset<17>(t) << ' ' << std::bitset<17>(o) << ' ' << num_tile(t, w) << std::endl;
        }
      }
    }
    
  }
  
  int max_tile = 0;
  for (auto iter=prev_m.begin(); iter!=prev_m.end(); ++iter){
    max_tile = std::max(max_tile, iter->second);
  }
  
  std::cout << max_tile << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}