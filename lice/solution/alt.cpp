#include "lice.h"
#include <bits/stdc++.h>
using namespace std;
// independent O(n): rerooting over all directed edges. in[e] for e=(v->y) = profile masks of
// spine prefixes that start at some endpoint on v's side, end at v and continue to y.
// answer 2 iff some y can be the final endpoint.  goodness computed by rerooting counts.
int min_days(int n, vector<int> U, vector<int> V){
    vector<vector<int>> g(n);
    for(int i=0;i<n-1;i++) g[U[i]].push_back(V[i]),g[V[i]].push_back(U[i]);
    bool path=true; for(auto&a:g) if(a.size()>2) path=false;
    if(path) return 1;
    auto nl=[&](int x){return g[x].size()>=2;};
    vector<int> par(n,-1),ord={0}; vector<char> vis(n,0); vis[0]=1;
    for(int i=0;i<(int)ord.size();i++) for(int y:g[ord[i]]) if(!vis[y]){vis[y]=1;par[y]=ord[i];ord.push_back(y);}
    // gd[c]: subtree of c (away from parent) is caterpillar-from-c; gu[c]: component of par[c] away from c is
    vector<char> gd(n,1),gu(n,1); vector<int> nlc(n,0),badc(n,0),NL(n,0);
    for(int x=0;x<n;x++) for(int y:g[x]) NL[x]+=nl(y);
    for(int i=n-1;i>=0;i--){int c=ord[i]; for(int d:g[c]) if(d!=par[c]&&nl(d)){nlc[c]++; if(!gd[d]) badc[c]++;}
        gd[c]= !nl(c) || (nlc[c]<=1 && badc[c]==0);}
    for(int i=0;i<n;i++){
        int p=ord[i]; vector<int> nb; // non-leaf neighbours of p (only needed when there are <=2)
        for(int y:g[p]) if(nl(y)&&nb.size()<3) nb.push_back(y);
        for(int c:g[p]) if(c!=par[p]){
            int y=-1,cnt=0; for(int z:nb) if(z!=c){cnt++;y=z;}
            if(!nl(p)||cnt==0) gu[c]=1;
            else if(cnt>=2||NL[p]-nl(c)>=2) gu[c]=0;
            else gu[c]= y==par[p]?gu[p]:gd[y];
        }
    }
    auto gb=[&](int v,int x)->bool{ return x==par[v]?gu[v]:gd[x]; }; // branch at x away from v good?
    vector<int> BAD(n,0); for(int v=0;v<n;v++) for(int x:g[v]) if(nl(x)&&!gb(v,x)) BAD[v]++;
    auto typ=[&](int v,int x,int y){ // spine neighbours x and y (y=-1 for endpoint)
        int K=NL[v],B=BAD[v],sz=1; if(nl(x)){K--;B-=!gb(v,x);}
        if(y>=0){sz=2; if(nl(y)){K--;B-=!gb(v,y);}}
        if(B>0||K>2) return -1;
        if((int)g[v].size()==sz) return 0;
        return K==2?2:1; };
    auto step=[](int s,int t){ if(t<0||!s) return 0; if(t==1) return s; if(t==0) return 2; return (s&2)?4:0; };
    vector<int> up(n,0),dn(n,0); // up[c]=in(c->par c), dn[c]=in(par c->c)
    for(int i=n-1;i>0;i--){int c=ord[i],s=step(1,typ(c,par[c],-1));
        for(int d:g[c]) if(d!=par[c]) s|=step(up[d],typ(c,d,par[c]));
        up[c]=s;}
    // top-down: for p, aggregate incoming masks by neighbour class (0 leaf,1 nonleaf good,2 nonleaf bad)
    for(int i=0;i<n;i++){int p=ord[i];
        int cnt[3][8]={{0}};
        auto inm=[&](int x){return x==par[p]?dn[p]:up[x];};
        auto cls=[&](int x){return !nl(x)?0:(gb(p,x)?1:2);};
        int r1[3]={-1,-1,-1},r2[3]={-1,-1,-1}; // two representatives per class (typ depends only on class)
        for(int x:g[p]){int k=cls(x); cnt[k][inm(x)]++; if(r1[k]<0) r1[k]=x; else if(r2[k]<0) r2[k]=x;}
        for(int c:g[p]) if(c!=par[p]){
            int s=step(1,typ(p,c,-1)); cnt[cls(c)][inm(c)]--;
            int rep[3]; for(int k=0;k<3;k++) rep[k]= r1[k]!=c?r1[k]:r2[k];
            for(int k=0;k<3;k++) if(rep[k]>=0){int t=typ(p,rep[k],c); for(int m=1;m<8;m++) if(cnt[k][m]) s|=step(m,t);}
            cnt[cls(c)][inm(c)]++; dn[c]=s;
        }}
    for(int v=0;v<n;v++) for(int x:g[v]){ int in= x==par[v]?dn[v]:up[x]; if(step(in,typ(v,x,-1))&2) return 2; }
    return 3;
}
