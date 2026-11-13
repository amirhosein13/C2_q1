#include "lice.h"
#include <bits/stdc++.h>
using namespace std;
// wrong: believes T^2 always has a Hamiltonian path
int min_days(int n, vector<int> U, vector<int> V){
    vector<int> d(n,0);
    for(int i=0;i<n-1;i++) d[U[i]]++,d[V[i]]++;
    return *max_element(d.begin(),d.end())<=2?1:2;
}
