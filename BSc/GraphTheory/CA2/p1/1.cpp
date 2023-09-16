#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int maxn=10*1000+10;

int c[maxn][maxn];
vector <int> adj[maxn];
queue<int> q;
int par[maxn];

int m, a, b;;
int n;

bool increase_flow(int s , int t);
void initialize();

int main()
{
    initialize();

    while (increase_flow(n, n+1)) 
        ;

    int ans=0;
    for (int i=0; i < a; ++i)
        if (par[i] != -1)
            ++ans;
    for (int i=a; i<a+b; ++i)
        if (par[i]==-1) 
            ++ans;
    cout<<ans<<endl;
    return 0;
}

bool increase_flow(int s , int t)
{
    memset(par, -1,sizeof par);
    while (!q.empty())
        q.pop(); 

    q.push(s);
    par[s]=s ;
 
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int i=0; i < n+2; ++i)
            if (c[v][i]>0 && par[i]==-1) {
                par[i] = v ;
                q.push(i);
            }
    }

    if (par[t]==-1) 
        return false;

    int neck = -1;
    for (int v=t; v!=s; v=par[v])
        if (neck==-1 || neck>c[par[v]][v])
            neck=c[par[v]][v];
    for (int v=t; v!=s; v=par[v]) {
        c[par[v]][v] -= neck;
        c[v][par[v]] += neck;
    }
    return true;
}

void initialize()
{
    cin>>a>>b>>m;
    n = a+b;
    for (int i=0; i < n; ++i)
        for(int j=0; j < n; ++j)
            if (i<a && j>=a)
                c[i][j]=n+1; 
    int x, y;
    for (int i=0; i < m; ++i) {
        cin>>x>>y;
        int v=x, u=a+y;
        adj[v].push_back(u);
        adj[u].push_back(v);
        c[u][v]=c[v][u]=0;
    }
    for (int i=0; i < a; ++i)
        c[n][i]=c[i][n]=1;
    for (int i=a; i <a+b; ++i)
        c[i][n+1]=c[n+1][i]=1;
}
