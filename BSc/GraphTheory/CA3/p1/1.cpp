#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <vector>

#define SOURCE (n)
#define SINK (n+1)

using namespace std;

const int maxn=1000+10;

struct Node {
    int sum;
    int sub;
    int pro; // product
};

Node results[maxn];

set <int> adj[maxn*4];
set <int> vals;

map <int, int> vals_index;

queue<int> q;
int par[maxn*4];

int n;

bool increase_flow(int s , int t);
void initialize();
void read_input();

int main()
{
    read_input();
    initialize();
    
    while (increase_flow(SOURCE, SINK)) 
        ;

    if (adj[SOURCE].size()!=0)
        cout << "Impossible"<<endl;
    else 
        cout << "Possible"<<endl;
    return 0;
}

bool increase_flow(int s , int t)
{
    memset(par, -1,sizeof par);
    while (!q.empty())
        q.pop(); 

    q.push(s);
    par[s]=s;
    
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        
        set<int>::iterator it;
        for (it=adj[v].begin(); it!= adj[v].end(); ++it)
            if (par[*it]==-1) {
                par[*it] = v;
                q.push(*it);
            }
    }

    if (par[t]==-1) 
        return false;

    for (int v=t; v!=s; v=par[v]) {
        int p = par[v];
        adj[p].erase(v);
        adj[v].insert(p);
    }
    return true;
}

void initialize() 
{
    for (int i=0; i<n; ++i) {
        vals.insert(results[i].sum);
        vals.insert(results[i].sub);
        vals.insert(results[i].pro);
    }
    
    int index=n+2;
    set<int>::iterator it;
    for (it=vals.begin(); it!=vals.end(); ++it)
        vals_index[*it] = index++; 

    for (int i=0; i<n; ++i) {
        adj[i].insert(vals_index[results[i].sum]);
        adj[i].insert(vals_index[results[i].sub]);
        adj[i].insert(vals_index[results[i].pro]);
    }
    
    for (int i=0; i<n; ++i) 
        adj[SOURCE].insert(i);

    for (it=vals.begin(); it!=vals.end(); ++it) 
        adj[vals_index[*it]].insert(SINK);
}

void read_input()
{
    cin>>n;

    int first, second;
    for (int i=0; i<n; ++i) {
        cin>>first>>second;
        results[i].sum = first+second;
        results[i].sub = first-second;
        results[i].pro = first*second;
    }
}
