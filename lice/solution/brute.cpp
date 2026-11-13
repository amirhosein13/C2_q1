#include "lice.h"
#include <bits/stdc++.h>
using namespace std;
// exponential: bitmask DP for Hamiltonian path in T^d, d=1,2,... (only for n<=20)
int min_days(int n, vector<int> U, vector<int> V){
    if(n>20) return 3;
    vector<vector<int>> D(n,vector<int>(n,1e9));
    for(int i=0;i<n;i++) D[i][i]=0;
    for(int i=0;i<n-1;i++) D[U[i]][V[i]]=D[V[i]][U[i]]=1;
    for(int k=0;k<n;k++) for(int i=0;i<n;i++) for(int j=0;j<n;j++) D[i][j]=min(D[i][j],D[i][k]+D[k][j]);
    for(int d=1;;d++){
        vector<int> dp(1<<n,0); for(int i=0;i<n;i++) dp[1<<i]=1<<i;
        for(int m=1;m<(1<<n);m++) if(dp[m]) for(int i=0;i<n;i++) if(dp[m]>>i&1)
            for(int j=0;j<n;j++) if(!(m>>j&1)&&D[i][j]<=d) dp[m|1<<j]|=1<<j;
        if(dp[(1<<n)-1]) return d;
    }
}
