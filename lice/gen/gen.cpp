#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;
// usage: gen <mode> <T> <nmax> <sumn> <seed> [p1] [p2]
// writes T trees (each 2<=n<=nmax, total <= sumn), labels/edges shuffled.
// modes: rand window path star spider cat binary spine noncat horse cc bad k3 deep
typedef vector<pair<int,int>> E;
string mode; int P1,P2;
int R(int a,int b){return rnd.next(a,b);}
void addCat(E&e,int&n,int at,int sz){ // caterpillar-from-root hanging at 'at', sz>=1 vertices
    int r=n++; e.push_back({at,r}); sz--; int cur=r;
    while(sz>0){
        int lv=R(0,min(sz,3)); if(sz>0&&lv==sz&&R(0,1)) lv=sz-1;
        for(int i=0;i<lv&&sz>1;i++){e.push_back({cur,n++});sz--;}
        if(sz>0){int c=n++;e.push_back({cur,c});cur=c;sz--;}
    }
}
void addBad(E&e,int&n,int at,int sz){ // non-caterpillar branch: root r - x, x has two non-leaf children
    int r=n++;e.push_back({at,r}); int x=n++; e.push_back({r,x});
    for(int j=0;j<2;j++){int y=n++;e.push_back({x,y});e.push_back({y,n++});}
    sz-=6; if(sz>0) addCat(e,n,x,sz);
}
string word(int L,bool cc){ // random word over 0,A,C matching 0+(C0+)* with A's inserted (or a CC if cc)
    string w; bool last0=false;
    for(int i=0;i<L;i++){
        int r=R(0,9); char c;
        if(i==0) c=R(0,1)?'0':'A';
        else if(r<3) c='0'; else if(r<6) c='A'; else c=last0?'C':'0';
        if(c=='0') last0=true;
        if(c=='C') last0=false;
        w+=c;
    }
    w.back()='0';
    if(w[0]=='C') w[0]='0';
    if(cc&&L>=4){int p=R(1,L-3); w[p]='C'; w[p+1]='C'; }
    return w;
}
bool isCat(const E&e,int n){
    vector<int> d(n,0),in(n,0); for(auto&p:e) d[p.first]++,d[p.second]++;
    for(auto&p:e) if(d[p.first]>1&&d[p.second]>1) in[p.first]++,in[p.second]++;
    for(int i=0;i<n;i++) if(in[i]>2) return false;
    return true;
}
E build0(int N);
E build(int N){
    if(mode!="noncat") return build0(N);
    E e; for(int it=0;it<200;it++){ e=build0(N); int n=0; for(auto&p:e) n=max(n,max(p.first,p.second)+1); if(!isCat(e,n)) break; }
    return e;
}
E build0(int N){
    E e; int n=0;
    if(mode=="rand"||mode=="window"){
        int k=mode=="rand"?N:max(1,P1>0?P1:R(1,5)); n=N;
        for(int i=1;i<N;i++) e.push_back({i,R(max(0,i-k),i-1)});
    } else if(mode=="path"){ n=N; for(int i=1;i<N;i++) e.push_back({i-1,i}); }
    else if(mode=="star"){ n=N; for(int i=1;i<N;i++) e.push_back({0,i}); }
    else if(mode=="binary"){ n=N; for(int i=1;i<N;i++) e.push_back({i,(i-1)/2}); }
    else if(mode=="spider"){ // P1 = number of legs of length >=2 (-1 random), rest leaves
        n=1; int big=P1>=0?P1:R(0,7); int rem=N-1;
        vector<int> legs;
        for(int i=0;i<big&&rem>=2;i++){legs.push_back(2);rem-=2;}
        int nb=legs.size();
        while(rem>0){ if(nb>0&&R(0,1)){legs[R(0,nb-1)]++;} else legs.push_back(1); rem--; }
        if(legs.size()<3) while(legs.size()<3) legs.push_back(0);
        for(int L:legs){int prv=0;for(int j=0;j<L;j++){e.push_back({prv,n});prv=n++;}}
    } else if(mode=="cat"){
        int L=max(1,N/R(2,5)); n=L; for(int i=1;i<L;i++) e.push_back({i-1,i});
        while(n<N) e.push_back({R(0,L-1),n++});
    } else if(mode=="spine"||mode=="noncat"){
        int L=R(1,max(1,min(6,N))); n=L; for(int i=1;i<L;i++) e.push_back({i-1,i});
        for(int s=0;s<L;s++){int b=R(0,3); while(b--){int sz=R(1,5); if(n+sz>N) break; int base=n; e.push_back({s,base}); for(int j=1;j<sz;j++) e.push_back({base+j,base+R(0,j-1)}); n+=sz;}}
    } else { // horse family / deep
        int L= mode=="deep"? max(4,N/3) : max(2,min(N/3, R(2,max(2,(int)sqrt(N)*2))));
        string w=word(L,mode=="cc"); n=L; for(int i=1;i<L;i++) e.push_back({i-1,i});
        int budget=N-L; vector<int> need;
        for(char c:w) need.push_back(c=='C'?2:c=='A'?R(0,1):0);
        int bigs=accumulate(need.begin(),need.end(),0); if(mode=="k3") bigs++;
        int per=max(2,bigs?budget/(bigs+1):2);
        bool badDone=false;
        for(int i=0;i<L&&n<N;i++){
            int k=need[i]+((mode=="k3"&&w[i]=='C'&&!badDone)?1:0);
            if(mode=="k3"&&w[i]=='C'&&!badDone) badDone=true;
            for(int j=0;j<k&&n+2<=N;j++){
                int sz=min(N-n,max(2,R(per/2,per*3/2)));
                if(mode=="bad"&&!badDone&&sz>=6){addBad(e,n,i,sz);badDone=true;} else addCat(e,n,i,sz);
            }
            if(w[i]=='A'&&k==0&&n<N) e.push_back({i,n++});
            if(w[i]!='0'&&n<N&&R(0,2)==0){int lv=R(0,2);while(lv--&&n<N) e.push_back({i,n++});}
        }
        while(n<N){ // leftover: leaves on A/C spine vertices, else extend last caterpillar end
            vector<int> ok; for(int i=0;i<L;i++) if(w[i]!='0') ok.push_back(i);
            if(ok.empty()||mode=="deep"){ e.push_back({n-1,n}); n++; } else e.push_back({ok[R(0,(int)ok.size()-1)],n++});
        }
    }
    return e;
}
int main(int argc,char*argv[]){
    registerGen(argc,argv,1);
    mode=argv[1]; int T=atoi(argv[2]),nmax=atoi(argv[3]); long long sumn=atoll(argv[4]);
    P1=argc>6?atoi(argv[6]):-1; P2=argc>7?atoi(argv[7]):-1;
    vector<E> out; vector<int> ns; long long left=sumn;
    for(int t=0;t<T;t++){
        int rest=T-t-1; long long cap=min<long long>(nmax,left-2LL*rest); if(cap<2) break;
        long long target=min<long long>(cap,left/(T-t));
        int N= (T==1)? (int)cap : R(max(2,(int)(target*3/4)),(int)max(2LL,target));
        if(P2>0) N=(int)min<long long>(cap,R(2,P2));
        E e=build(N); int n=0; for(auto&p:e) n=max(n,max(p.first,p.second)+1); if(n<2){e={{0,1}};n=2;}
        vector<int> perm(n); iota(perm.begin(),perm.end(),1); shuffle(perm.begin(),perm.end());
        for(auto&p:e){p.first=perm[p.first];p.second=perm[p.second]; if(R(0,1)) swap(p.first,p.second);}
        shuffle(e.begin(),e.end()); out.push_back(e); ns.push_back(n); left-=n;
    }
    printf("%d\n",(int)out.size());
    for(size_t i=0;i<out.size();i++){printf("%d\n",ns[i]); for(auto&p:out[i]) printf("%d %d\n",p.first,p.second);}
}
