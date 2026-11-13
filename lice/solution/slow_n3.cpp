#include "lice.h"
#include <bits/stdc++.h>
using namespace std;
// O(n^3): every pair (u,v), check the spine u..v directly against 0+(C0+)* with A ignored
int min_days(int n, vector<int> U, vector<int> V){
    vector<vector<int>> g(n);
    for(int i=0;i<n-1;i++) g[U[i]].push_back(V[i]),g[V[i]].push_back(U[i]);
    bool path=true; for(auto&a:g) if(a.size()>2) path=false;
    if(path) return 1;
    auto nl=[&](int x){return g[x].size()>=2;};
    vector<int> par(n),ord; vector<char> good(n),on(n,0);
    for(int u=0;u<n;u++){
        ord.assign(1,u); fill(par.begin(),par.end(),-2); par[u]=-1;
        for(int i=0;i<(int)ord.size();i++) for(int y:g[ord[i]]) if(par[y]==-2){par[y]=ord[i];ord.push_back(y);}
        for(int i=n-1;i>=0;i--){int c=ord[i],k=0; bool ok=true;
            for(int d:g[c]) if(d!=par[c]&&nl(d)){k++; ok&=good[d];}
            good[c]=!nl(c)||(k<=1&&ok);}
        for(int v=u+1;v<n;v++){
            vector<int> P; for(int x=v;x!=-1;x=par[x]) P.push_back(x);
            for(int x:P) on[x]=1;
            string w; bool ok=true;
            for(int x:P){ int k=0,off=0;
                for(int y:g[x]) if(!on[y]){off++; if(nl(y)){k++; if(!good[y]) ok=false;}}
                if(k>2) ok=false;
                if(off==0) w+='0'; else if(k==2) w+='C'; }
            for(int x:P) on[x]=0;
            if(!ok) continue;
            bool m=!w.empty()&&w.front()=='0'&&w.back()=='0';
            for(size_t i=0;i+1<w.size();i++) if(w[i]=='C'&&w[i+1]=='C') m=false;
            if(m) return 2;
        }
    }
    return 3;
}
