#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

typedef pair<int, int> Pair;

const int maxn=100*1000+100;
int n;
vector<int> adj[maxn], cost[maxn];
vector<Pair> edges, edgee;
int mark[maxn], child[maxn];
int pare[maxn];
int q;

int ans;

void dfs(int v);
void clear();

int main()
{
    cin>>n;
    for (int i=0; i < n-1; ++i) {
        int s, t, l;
        cin>>s>>t>>l; --s; --t;
        adj[s].push_back(t);
        cost[s].push_back(l);
        adj[t].push_back(s);
        cost[t].push_back(l);
        edges.push_back(make_pair(s, cost[s].size()-1));
        edgee.push_back(make_pair(t, cost[t].size()-1));
    }
    cin>>q;
    while (q--) {
        int i, w;
        cin>>i>>w; --i;
        Pair s=edges[i], t=edgee[i];
        cost[s.first][s.second]=w;
        cost[t.first][t.second]=w;
        clear(); 
        dfs(0);
        cout<<fixed << setprecision(6)<< (6.00*ans)/(n*(n-1))<<endl;
    }
    return 0;
}

void dfs(int v) 
{
    mark[v]=1;
    for (int i=0; i < adj[v].size(); ++i) {
        int u = adj[v][i];
        if (!mark[u]) {
            pare[u]=cost[v][i];
            dfs(u);
            child[v]+=child[u];
        }
    }
    ++child[v];
    ans += child[v]*(n-child[v])*pare[v];
}
void clear()
{
   ans = 0;
   for (int i=0; i < maxn; ++i)
        child[i]=pare[i]=mark[i]=0;
}
