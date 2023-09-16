#include <iostream>
#include <algorithm>
using namespace std;

const int maxn=1000+10;
const int maxm=100*1000+10;
const int inf=(1<<30);

struct Edge {
    int sp, ep, w;
    Edge() {
        sp=ep=w=0;
    }
    Edge(int _sp, int _ep, int _w) {
        sp=_sp; ep=_ep; w=_w;
    }
};

Edge e[maxm];
int id[maxn];
int m, n, ans=inf;

bool ismore(Edge l, Edge r) {
    return l.w > r.w;
}

int root(int v) {
    while (v!=id[v]) {
        id[v]=id[id[v]];
        v=id[v];
    }
    return v;
}

void Union(int v, int u) {
    id[root(v)]=root(u);
}

int main() {
    cin>>n>>m;
    for (int i=0; i < m; ++i) {
        int _sp, _ep, _w;
        cin>>_sp>>_ep>>_w;
        e[i]=Edge(_sp, _ep, _w);
    }

    for (int i=0; i < n; ++i)
        id[i]=i;

    sort(e, e+m, ismore);

    for (int i=0; i < m; ++i) {
        int u=e[i].sp; int v=e[i].ep;
        if (root(u)!=root(v)) {
            Union(v, u);
            ans = min(ans, e[i].w);
        }
    }
    cout<<ans<<endl;
}
