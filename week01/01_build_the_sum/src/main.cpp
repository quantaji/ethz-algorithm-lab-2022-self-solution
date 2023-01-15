#include <iostream>

using namespace std;

int main(){
  int t, n, sum, temp;
  cin>>t;
  for (int i=0; i<t; i++) {
    cin>>n;
    sum=0;
    for (int j=0; j<n; j++) {
      cin>>temp;
      sum += temp;
    }
    cout<<sum<<endl;
  }
  return 0;
}