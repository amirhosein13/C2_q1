#include "lice.h"
#include <bits/stdc++.h>
using namespace std;
// only for spiders: 1 if path, else 2 iff at most 4 legs have length >= 2
int min_days(int n, vector<int> U, vector<int> V){
    vector<vector<int>> g(n);
    for(int i=0;i<n-1;i++) g[U[i]].push_back(V[i]),g[V[i]].push_back(U[i]);
    int c=-1; for(int i=0;i<n;i++) if(g[i].size()>2) c=i;
    if(c<0) return 1;
    int longLegs=0; for(int x:g[c]) if(g[x].size()>=2) longLegs++;
    return longLegs<=4?2:3;
}
