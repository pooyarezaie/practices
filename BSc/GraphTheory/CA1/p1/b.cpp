#include <iostream>
#include <vector>
#include <set>
#include <cstring>

using namespace std;

const int maxn=10*1000+100;
int n, m;
vector<int> adj[maxn], cost[maxn]; int dis[maxn];
int q;

struct comp {
    bool operator () (int a, int b) {

        if (dis[a]!=dis[b]) return dis[a]<dis[b]; 
        return a<b;
    }
};

set<int,comp> sit;

void dijkstra(int v);

int main()
{
    cin>>n>>m;
    for (int i=0; i < n; ++i)
        for (int j=0; j < n; ++j) {
            int c;
            cin>>c;
            if (c>=0) {
                adj[i].push_back(j);
                cost[i].push_back(c);
            }
        }
    cin>>q;
    while (q--) {
        int v; 
        cin>>v;
        dijkstra(v);
        double ans = 0.0;
        for (int i=0; i < n; ++i)
            ans += dis[i];
        cout<<1.0/ans<<endl;
    }
}

void dijkstra(int v)
{
    memset(dis, 63, sizeof dis);
    dis[v]=0;
    for (int i=0; i < n; ++i) 
        sit.insert(i);
    while (!sit.empty()) {
        int u=*( sit.begin() );
        sit.erase(u);
        for (int i=0; i < int(adj[u].size()); ++i)
            if (dis[u]+cost[u][i] < dis[ adj[u][i] ]) { 
                sit.erase( adj[u][i] );
                dis[ adj[u][i] ]=dis[u]+cost[u][i];
                sit.insert( adj[u][i] );
            }
    }
}
