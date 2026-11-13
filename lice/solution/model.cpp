#include "lice.h"
#include <bits/stdc++.h>
using namespace std;
// answer<=3 (T^3 Hamiltonian-connected); 1 iff path; 2 iff T^2 has a Hamiltonian path:
// exists spine P, each non-leaf branch off P is a caterpillar from its root,
// vertex types on P: 0 (no off-neighbours), A (<=1 big branch), C (2 big branches),
// and the 0/C word (A ignored) matches 0+(C0+)*.
int min_days(int n, vector<int> U, vector<int> V){
    vector<int> deg(n,0);
    for(int i=0;i<n-1;i++) deg[U[i]]++,deg[V[i]]++;
    if(*max_element(deg.begin(),deg.end())<=2) return 1;
    int m=2*(n-1);
    vector<int> st(n+1,0),adj(m),rv(m),pos(n);
    for(int i=0;i<n;i++) st[i+1]=st[i]+deg[i];
    for(int i=0;i<n;i++) pos[i]=st[i];
    for(int i=0;i<n-1;i++){int a=pos[U[i]]++,b=pos[V[i]]++;adj[a]=V[i];adj[b]=U[i];rv[a]=b;rv[b]=a;}
    auto nl=[&](int x){return deg[x]>=2;};
    vector<int> NL(n,0),BAD(n,0);
    for(int x=0;x<n;x++) for(int e=st[x];e<st[x+1];e++) NL[x]+=nl(adj[e]);
    // good[e], e=(w->x): branch at x (away from w) is a caterpillar from x
    vector<signed char> good(m,-1); vector<int> stk;
    for(int e0=0;e0<m;e0++){
        if(good[e0]>=0) continue;
        int e=e0,res; stk.clear();
        while(1){
            if(good[e]>=0){res=good[e];break;}
            int x=adj[e],w=adj[rv[e]],c=NL[x]-nl(w);
            if(!nl(x)||c==0){res=1;break;}
            if(c>=2){res=0;break;}
            stk.push_back(e);
            for(int g=st[x];g<st[x+1];g++) if(adj[g]!=w&&nl(adj[g])){e=g;break;}
        }
        good[e]=res; for(int f:stk) good[f]=res;
    }
    for(int x=0;x<n;x++) for(int e=st[x];e<st[x+1];e++) if(nl(adj[e])&&!good[e]) BAD[x]++;
    // type of w with spine-neighbours given by edges e1(,e2): -1 bad, 0, 1=A, 2=C
    auto typ=[&](int w,int sz,int e1,int e2){
        int K=NL[w],B=BAD[w];
        if(nl(adj[e1])){K--;B-=!good[e1];}
        if(sz==2&&nl(adj[e2])){K--;B-=!good[e2];}
        if(B>0||K>2) return -1;
        if(deg[w]==sz) return 0;
        return K==2?2:1;
    };
    // masks: 1 = no symbol yet, 2 = last symbol 0, 4 = last symbol C
    auto step=[&](int s,int t){
        if(t<0||!s) return 0;
        if(t==1) return s;
        if(t==0) return 2;
        return (s&2)?4:0;
    };
    vector<int> par(n,-1),pe(n,-1),ord; ord.reserve(n); ord.push_back(0); par[0]=0;
    for(int i=0;i<(int)ord.size();i++){int x=ord[i];for(int e=st[x];e<st[x+1];e++){int y=adj[e];if(par[y]<0){par[y]=x;pe[y]=rv[e];ord.push_back(y);}}}
    par[0]=-1;
    vector<int> dn(n,0);
    for(int i=n-1;i>0;i--){
        int c=ord[i],s=step(1,typ(c,1,pe[c],0));
        for(int e=st[c];e<st[c+1];e++) if(adj[e]!=par[c]) s|=step(dn[adj[e]],typ(c,2,e,pe[c]));
        dn[c]=s;
    }
    auto fin=[&](int s,int p){ return p==2?s!=0:(s&2)!=0; };
    for(int v=0;v<n;v++){
        int fe[15],se[15],cnt[15]={0};
        for(int e=st[v];e<st[v+1];e++){
            int d=adj[e]; if(d==par[v]||!dn[d]) continue;
            if(step(dn[d],typ(v,1,e,0))&2) return 2;
            int k=nl(d)?1+(good[e]?7:0)+dn[d]-1:0;
            if(cnt[k]==0) fe[k]=e; else if(cnt[k]==1) se[k]=e;
            if(cnt[k]<2) cnt[k]++;
        }
        for(int a=0;a<15;a++) if(cnt[a]) for(int b=a;b<15;b++){
            if(!cnt[b]||(a==b&&cnt[a]<2)) continue;
            int e1=fe[a],e2=a==b?se[a]:fe[b],t=typ(v,2,e1,e2);
            if(t<0) continue;
            int m1=dn[adj[e1]],m2=dn[adj[e2]];
            for(int x=1;x<=4;x<<=1) if(m1&x) for(int y=1;y<=4;y<<=1) if(m2&y) if(fin(step(x,t),y)) return 2;
        }
    }
    return 3;
}
