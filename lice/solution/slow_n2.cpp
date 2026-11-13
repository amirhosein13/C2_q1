#include "lice.h"
#include <bits/stdc++.h>
using namespace std;
// O(n^2): fix the first endpoint u, root at u, walk all spines u->v with automaton states
int min_days(int n, vector<int> U, vector<int> V){
    vector<vector<int>> g(n);
    for(int i=0;i<n-1;i++) g[U[i]].push_back(V[i]),g[V[i]].push_back(U[i]);
    bool path=true; for(auto&a:g) if(a.size()>2) path=false;
    if(path) return 1;
    auto nl=[&](int x){return g[x].size()>=2;};
    auto step=[](int s,int t){ if(t<0||!s) return 0; if(t==1) return s; if(t==0) return 2; return (s&2)?4:0; };
    vector<int> par(n),ord,f(n); vector<char> good(n); vector<int> K(n),bad(n);
    for(int u=0;u<n;u++){
        ord.assign(1,u); fill(par.begin(),par.end(),-2); par[u]=-1;
        for(int i=0;i<(int)ord.size();i++) for(int y:g[ord[i]]) if(par[y]==-2){par[y]=ord[i];ord.push_back(y);}
        for(int i=n-1;i>=0;i--){int c=ord[i];K[c]=bad[c]=0;
            for(int d:g[c]) if(d!=par[c]&&nl(d)){K[c]++; if(!good[d]) bad[c]++;}
            good[c]=!nl(c)||(K[c]<=1&&bad[c]==0);}
        // type of c with parent and optionally child d on spine (children are the only off-spine neighbours)
        auto typ=[&](int c,int d){ int k=K[c],b=bad[c],off=(int)g[c].size()-(par[c]>=0)-(d>=0);
            if(d>=0&&nl(d)){k--; b-=!good[d];} if(b>0||k>2) return -1; if(off==0) return 0; return k==2?2:1; };
        f[u]=1;
        for(int c:ord){
            if(c!=u&&(step(f[c],typ(c,-1))&2)) return 2;
            for(int d:g[c]) if(d!=par[c]) f[d]=step(f[c],typ(c,d));
        }
    }
    return 3;
}
