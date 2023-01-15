#include <iostream>
using namespace std;

void solve() {
  int n;
  cin >> n;
  int v[n];
  for (int i=0; i<n; ++i) {
    cin >> v[i];
  }
  if (n<3) {
    if (n==1) {
      cout << v[0] << endl;
      return;
    } else {
      if (v[0] > v[1]) {
        cout << v[0] << endl;
        return;
      } else {
        cout << v[1] << endl;
        return;
      }
    }
  } else {
    // only the choice of length l-1 and l-2 is used in l.
    int prevprev_score[n], prev_score[n], current_score[n];
    bool prev_choice[n], current_choice[n];
    for (int i=0; i<n; ++i) {
      prevprev_score[i] = v[i];
      if (i<n-1) {
        if (v[i] < v[i+1]) {
          prev_choice[i] = 1;
          prev_score[i] = v[i+1];
        } else {
          prev_choice[i] = 0;
          prev_score[i] = v[i];
        }
      }
    }
    int max_left, max_right;
    for (int l=2; l<n; ++l) {
      for (int left=0; left+l<n; ++left) {
        // if I choose left,
        if (prev_choice[left+1] == 0) {
          max_left = v[left] + prevprev_score[left+2];
        } else {
          max_left = v[left] + prevprev_score[left+1];
        }
        // if I choose right,
        if (prev_choice[left] == 0) {
          max_right = v[left+l] + prevprev_score[left+1];
        } else {
          max_right = v[left+l] + prevprev_score[left];
        }
        if(max_left < max_right) {
          current_score[left] = max_right;
          current_choice[left] = 1;
        } else {
          current_score[left] = max_left;
          current_choice[left] = 0;
        }
      }
      for (int i=0; i<n; ++i) {
        prevprev_score[i] = prev_score[i];
        prev_score[i] = current_score[i];
        prev_choice[i] = current_choice[i];
      }
    }
    cout << current_score[0] << endl;
  }
}

int main() {
  int t;
  cin >> t;
  for (int i=0; i<t; ++i) {
    solve();
  }
}