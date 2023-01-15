#include <iostream>
using namespace std;

int solve(int n){
  int max_index = 1;
  int height;
  bool stop = false;
  int result;
  for (int i=0; i<n; i++) {
    
    // check if the current will fall
    if (i >= max_index) {
      if (!stop) {
        result = i;
        stop = true;
      }
    }
    
    // check max_index
    cin >> height;
    if (max_index < i + height) {
      max_index = i + height;
      // cout << "ddd" << max_index << endl;
    }
  }
  if (stop) {
    return result;
  } else {
    return n;
  }
}

int main() {
  int t, n;
  cin>>t;
  for (int i=0; i<t; i++) {
    cin >> n;
    cout << solve(n) << endl;
  }
}