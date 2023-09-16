#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

const int maxn = 100*1000+100;
int n;
vector<int> adj[maxn]; int full[maxn];
int mark[maxn];
int m;

void dfs(int v, vector<int> &dist);

int main()
{
    cin>>n;
    for (int i=1; i < n; ++i) {
        int p; cin>>p; --p;
        adj[i].push_back(p);
        adj[p].push_back(i);
    }
    cin>>m;
    for (int i=0; i < m; ++i) {
        int p; cin>>p; --p;
        full[p] = 1;
    }

    int ans = 0;
    mark[0] = 1;
    for (int i=0; i < adj[0].size(); ++i) {
        vector<int> dist; 
        int u = adj[0][i];
        dfs(u, dist);
        sort(dist.begin(), dist.end());
        for (int j=0; j < (int)dist.size()-1; ++j)
            if (dist[j]>=dist[j+1])
                dist[j+1]=dist[j]+1;
        if (dist.size())
            ans = max(ans, dist.back()+1);
    }
    cout << ans << endl;
    return 0;
}


void dfs(int v, vector<int> &dist)
{
    mark[v]=1;
    int curr_size = dist.size();
    for (int i=0; i < adj[v].size(); ++i) {
        int u = adj[v][i];
        if (!mark[u])
            dfs(u, dist);
    }
    for (int i=curr_size; i < (int)dist.size(); ++i)
        dist[i]++;
    if (full[v])
        dist.push_back(0);
}
