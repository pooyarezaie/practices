#include <iostream>
#include <vector>
#include <cstring>
#include <climits>

#define target (n-1)

using namespace std;

const int maxn=1000+100;

int max_flow();
void init();
bool agu_path(int v);

int rc[maxn][maxn], gc[maxn][maxn];
int mark[maxn], par[maxn], bn[maxn];
vector <int> adj[maxn];
int m, n;

int main()
{
    ios::sync_with_stdio(false);
    int k; cin>>k;
    while (k--) {

        init();

        cin>>n>>m;
        for (int i=0; i < m; ++i) {
            int x, y, c; cin>>x>>y>>c;
            gc[x][y]=gc[y][x]=c;
            adj[x].push_back(y);
            adj[y].push_back(x);
        }
        int mf=max_flow();

        int mfp=mf;

        vector<pair<int , int> > cut_edges;
        for (int i=0; i < n; ++i)
            for (int j=0; j < adj[i].size(); ++j)
                if (mark[i]&&!mark[adj[i][j]])
                    cut_edges.push_back(make_pair(i, adj[i][j]));
        for (int i=0; i < cut_edges.size(); ++i) {
            int v=cut_edges[i].first, u=cut_edges[i].second;
            gc[v][u]*=2; gc[u][v]*=2;
            mfp=max(mfp, max_flow());
            gc[v][u]/=2; gc[u][v]/=2;
        }
        cout<<mf<<" "<<mfp<<endl;
    }
}

int max_flow()
{
    for (int i=0; i < n; ++i)
        for(int j=0; j < n; ++j)
            rc[i][j]=gc[i][j];
    bool end=false;
    while (!end) {
        for (int i=0; i < maxn; ++i) {
            mark[i]=0; par[i]=-1;
            bn[i]=INT_MAX;
        }
        end = !agu_path(0);
    }
    int ans=0;
    for (int i=0; i < n; ++i)
        ans+=gc[i][target]-rc[i][target];
    return ans;
}
bool agu_path(int v)
{
    if (v==target) {
        int c=v;
        int fe=bn[v];
        while (par[c]!=-1) {
            rc[par[c]][c]-=fe;
            rc[c][par[c]]+=fe;
            c=par[c];
        }
        return true;
    }
    mark[v]=1;
    for (int i=0; i < adj[v].size(); ++i) {
        int u=adj[v][i];
        if (!mark[u]&&rc[v][u]) {
            par[u]=v;
            bn[u]=min(bn[v], rc[v][u]);
            if (agu_path(u))
                return true;
        }
    }
    return false;
}
void init()
{
    for (int i=0; i < maxn; ++i)
        adj[i].clear();
    memset(rc, 0, sizeof rc);
    memset(gc, 0, sizeof gc);
}
