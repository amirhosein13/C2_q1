#include "lice.h"
#include <bits/stdc++.h>
using namespace std;
// wrong: thinks answer 2 iff the tree is a caterpillar (that is the Hamiltonian *cycle* criterion)
int min_days(int n, vector<int> U, vector<int> V){
    vector<int> d(n,0),in(n,0);
    for(int i=0;i<n-1;i++) d[U[i]]++,d[V[i]]++;
    if(*max_element(d.begin(),d.end())<=2) return 1;
    for(int i=0;i<n-1;i++) if(d[U[i]]>1&&d[V[i]]>1) in[U[i]]++,in[V[i]]++;
    for(int i=0;i<n;i++) if(in[i]>2) return 3;
    return 2;
}
