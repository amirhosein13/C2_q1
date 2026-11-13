#include "lice.h"
#include <bits/stdc++.h>
using namespace std;
// O(n^2): fix the first endpoint u, root at u, walk all spines u->v with automaton states
int min_days(int n, vector<int> U, vector<int> V){
    vector<int> deg(n,0),st(n+1,0),adj(2*(n-1));
    for(int i=0;i<n-1;i++) deg[U[i]]++,deg[V[i]]++;
    if(*max_element(deg.begin(),deg.end())<=2) return 1;
    for(int i=0;i<n;i++) st[i+1]=st[i]+deg[i];
    {vector<int> p(st.begin(),st.end()-1); for(int i=0;i<n-1;i++) adj[p[U[i]]++]=V[i],adj[p[V[i]]++]=U[i];}
    auto step=[](int s,int t){ if(t<0||!s) return 0; if(t==1) return s; if(t==0) return 2; return (s&2)?4:0; };
    vector<int> par(n),ord(n),f(n),K(n),bad(n),seen(n,-1); vector<char> good(n);
    for(int u=0;u<n;u++){
        int h=0,tl=0; ord[tl++]=u; par[u]=-1; seen[u]=u;
        while(h<tl){int x=ord[h++]; for(int e=st[x];e<st[x+1];e++){int y=adj[e]; if(seen[y]!=u){seen[y]=u;par[y]=x;ord[tl++]=y;}}}
        for(int i=n-1;i>=0;i--){int c=ord[i],k=0,b=0;
            for(int e=st[c];e<st[c+1];e++){int d=adj[e]; if(d!=par[c]&&deg[d]>=2){k++; b+=!good[d];}}
            K[c]=k; bad[c]=b; good[c]=deg[c]<2||(k<=1&&b==0);}
        // type of c with parent and optionally child d on the spine (children are the only off-spine neighbours)
        auto typ=[&](int c,int d){ int k=K[c],b=bad[c],off=deg[c]-(par[c]>=0)-(d>=0);
            if(d>=0&&deg[d]>=2){k--; b-=!good[d];}
            if(b>0||k>2) return -1;
            if(off==0) return 0;
            return k==2?2:1; };
        f[u]=1;
        for(int i=0;i<n;i++){int c=ord[i];
            if(c!=u&&(step(f[c],typ(c,-1))&2)) return 2;
            for(int e=st[c];e<st[c+1];e++){int d=adj[e]; if(d!=par[c]) f[d]=step(f[c],typ(c,d));}
        }
    }
    return 3;
}
