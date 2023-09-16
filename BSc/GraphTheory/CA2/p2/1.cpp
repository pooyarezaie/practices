#include <iostream>
#include <vector>
#include <set>

using namespace std;

const int maxn = 100*1000+10;

vector<int> adj[maxn];
int mark[maxn], value[maxn];
int degu[maxn], deg[maxn];;
int n, m, k;

struct comp {
    bool operator () (int a, int b) {

        if (value[a]*deg[b]!=value[b]*deg[a]) 
            return value[a]*deg[b]<value[b]*deg[a];
        return a<b;
    }
};

set<int,comp> s;

void read_input();
int gcd(int a, int b);

int main()
{
    read_input();

    for (int i=0; i < n; ++i)
        value[i]=deg[i]-degu[i];

    for (int i=0; i < n; ++i)
        if (!mark[i])
            s.insert(i);

    int ans = -1;;
    while (!s.empty()) {
        int u = *(s.begin());
        s.erase(u);
        mark[u] = 1;

        if (ans==-1 || value[ans]*deg[u]<value[u]*deg[ans])
            ans = u; 

        for (int i=0; i < adj[u].size(); ++i)
        {
            int v = adj[u][i];
            if (!mark[v]) {
                s.erase(v);
                value[v] -= 1;
                s.insert(v);
            }
        }
    }

    int g = gcd(value[ans], deg[ans]);
    cout << value[ans]/g << "/" << deg[ans]/g << endl;
    return 0;
}

int gcd(int a, int b) 
{
    return b? gcd(b, a%b): a;
}

void read_input()
{
    cin>>n>>m>>k;
    for (int i=0; i < k; ++i) {
        int v;cin>>v;
        mark[v]=1;
    }
    for (int i=0; i < m; ++i) {
        int u, v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        ++deg[u]; ++deg[v];
        if (mark[u]) 
            ++degu[v];
        if (mark[v])
            ++degu[u];
    }
}
