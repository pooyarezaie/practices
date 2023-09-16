#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cstring>

using namespace std;

const int maxn=10*1000+100;
int n, m;
vector<int> adj[maxn], P[maxn]; 
int dist[maxn], sigma[maxn];
double delta[maxn], cb[maxn];
int q;

void bfs(int v);
void clear_all();

int main()
{
    cin>>n>>m;
    for (int i=0; i < n; ++i)
        for (int j=0; j < n; ++j) {
            int c; cin>>c;
            if (c) adj[i].push_back(j);
        }

    for (int i=0; i < n; ++i)
        bfs(i);

    cin>>q;
    while (q--) {
        int v; cin>>v;
        cout<<cb[v]<<endl;
    }
}

void bfs(int v)
{
    clear_all();
    stack<int> s;
    queue<int> bq;
    sigma[v]=1; dist[v]=0;
    bq.push(v);
    while ( !bq.empty() ) {
        int u = bq.front(); bq.pop();
        s.push(u);
        for (int i=0; i < adj[u].size(); ++i) {

            int w = adj[u][i];

            if (dist[w]<0) {
                dist[w]=dist[u]+1;
                bq.push(w);
            }
            if (dist[w]==dist[u]+1) {
                sigma[w]+=sigma[u];
                P[w].push_back(u);
            }
        }
    }
    while ( !s.empty() ) {
        int w = s.top(); s.pop();
        for (int i=0; i < P[w].size(); ++i) {
            int u = P[w][i];
            delta[u] += (1.0+delta[w])*sigma[u]/sigma[w];
        }
        if (w != v) 
            cb[w] += delta[w];
    }
}

void clear_all()
{
    for (int i=0; i < n; ++i) {
        P[i].resize(0);
        dist[i]=-1;
        delta[i]=0;
    }
    memset(sigma, 0, sizeof sigma);
}
