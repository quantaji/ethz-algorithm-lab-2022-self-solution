///3
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

struct compare
{
    inline bool operator() (const pair<int, bool>& pair_1, const pair<int,bool>& pair_2)
    {
        if (pair_1.first == pair_2.first) {
          return int(pair_1.second) > int(pair_2.second);
        } else {
          return (pair_1.first < pair_2.first);
        }
    }
};

void solve(){
  int n;
  cin >> n;
  int temp;
  vector<pair<int, bool>> a={};
  for (int i=0; i<n; ++i) {
    cin >> temp;
    // 1 for start of new points, 0 for ending
    a.push_back(pair<int, bool> (temp-100, 1));
    a.push_back(pair<int, bool> (temp+100, 0));
  }
  sort(a.begin(), a.end(), less_than());
  int num=0, max_num = 0, dis=0, max_dis=0, k[2*n];
  for (int i=0; i< 2*n; ++i) {
    if (a[i].second) {
      ++num;
      k[i] = num;
      if (max_num<num) {
        // this can only happen at i<n-1 index, so no worry about out of range issue,
        max_num = num;
        max_dis = 0;
      }
    } else {
      if (num==max_num){
        dis = a[i].first - a[i-1].first;
        if (max_dis < dis) {
          max_dis = dis;
        }
      }
      k[i] = num;
      --num;
    }
  }
  cout << max_num << ' ' << 100 - max_dis/2 << endl;
  set<int> posi = {};
  for (int i=1; i<2*n; ++i) {
    if (k[i] == max_num && a[i].second == 0 && (a[i].first - a[i-1].first)/2 == max_dis/2) {
      if ((a[i].first - a[i-1].first) % 2) {
        // r - l is odd, two solution,
        posi.insert(a[i-1].first + (a[i].first - a[i-1].first)/2);
        posi.insert(a[i-1].first + (a[i].first - a[i-1].first)/2 + 1);
      } else {
        // r - l is even, one solution,
        posi.insert(a[i-1].first + (a[i].first - a[i-1].first)/2);
      }
    }
  }
  for (auto iter=posi.begin(); iter!=posi.end(); ++iter) {
    cout << *iter << ' ';
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  for (int i=0; i<t; ++i) {
    solve();
  }
}