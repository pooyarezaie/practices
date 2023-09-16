#include "graph.hpp"
#include <queue>
using namespace std;

Edge::Edge(int _u, int _v): u(_u), v(_v){}

Graph::Graph(vector<vector<bool> > matrix)
{
    adj_matrix = matrix;
    _order = matrix.size();
    _iorder = _order;
    _size = 0;
    for (int i = 0; i < _order; i++)
        for (int j = 0; j < _order; j++)
            if (adj_matrix[i][j])
                _size++;
    _size/=2;
}

Edge Graph::select_edge_triv()
{
    for (int i = 0; i < _iorder; i++)
        for(int j = 0; j < _iorder; j++)
            if (adj_matrix[i][j])
                return Edge(i, j);
    return Edge(-1, -1);
}

Polynomial Graph::base_result()
{
    Polynomial result, mul;
    result[1] = 1;
    mul[1] = 1;
    mul[0] = -1;
    for (int i = 0; i < _order-1; i++)
        result = result*mul;
    return result;
}

Edge Graph::select_edge()
{
    vector<bool> vis(_iorder, false);
    vector<int> par(_iorder, -1);
    queue<int> q;
    for (int i = 0; i < _iorder; i++) {
        bool found = false;
        for (int j = 0; j < _iorder; j++) {
            if (adj_matrix[i][j]) {
                q.push(i);
                vis[i]=true;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }
    while (!q.empty()) {
        int c = q.front();
        q.pop();
        for (int i = 0; i < _iorder; i++) {
            if (adj_matrix[c][i]) {
                if (vis[i]&&i!=par[c])
                    return Edge(c, i);
                q.push(i);
                vis[i] = true;
                par[i] = c;
            }
        }
    }
    return Edge(-1, -1);
}

vector<Graph> Graph::connected_components()
{
    vector<bool> vis(_order, false);
    vector<Graph> result;
    for (int i = 0; i < _order; i++) {
        if (vis[i])
            continue;
        vector<int> current;
        queue<int> q;
        q.push(i);
        vis[i]=true;
        while (!q.empty()) {

            int c = q.front(); 
            q.pop();

            current.push_back(c);
            for (int j=0; j < _order; j++) {
                if (adj_matrix[c][j]&&!vis[j]) {
                    q.push(j);
                    vis[j] = true;
                }
            }
        }
        vector<vector<bool> > cur_adj_matrix(
            current.size(),
            vector<bool>(current.size(), false));
        for (int j=0; j < current.size(); j++)
            for (int k = 0; k < current.size(); k++)
                cur_adj_matrix[j][k]=
                    adj_matrix[current[j]][current[k]];
        result.push_back(Graph(cur_adj_matrix));
    }
    return result;
}

Polynomial Graph::chromatic_polynomial_rec()
{
    if (_size==_order-1)
        return base_result();
    Edge e = select_edge();
    adj_matrix[e.u][e.v]=false;
    adj_matrix[e.v][e.u]=false;
    _size--;
    Polynomial p1 = chromatic_polynomial_rec();
    adj_matrix[e.u][e.v]=true;
    adj_matrix[e.v][e.u]=true;
    _size++;
    
    vector<bool> u_row = adj_matrix[e.u];
    vector<bool> v_row = adj_matrix[e.v];
    int psize = _size;
    _size--;
    adj_matrix[e.u][e.v]=false;
    adj_matrix[e.v][e.u]=false;
    for (int i=0; i<adj_matrix[e.u].size(); i++) {
        if(adj_matrix[e.u][i]&&adj_matrix[e.v][i]){
            _size--;
        }
        adj_matrix[e.u][i]=adj_matrix[e.u][i]||
            adj_matrix[e.v][i];
        adj_matrix[i][e.u]=adj_matrix[e.u][i];
        adj_matrix[e.v][i]=false;
        adj_matrix[i][e.v]=false;
    }
    _order--;
    Polynomial p2 = chromatic_polynomial_rec();
    adj_matrix[e.u]=u_row;
    adj_matrix[e.v]=v_row;
    for (int i=0; i<adj_matrix[e.u].size(); i++) {
        adj_matrix[i][e.u] = adj_matrix[e.u][i];
        adj_matrix[i][e.v] = adj_matrix[e.v][i];
    }
    _size = psize;
    _order++;
    
    return p1-p2;
}

Polynomial Graph::chromatic_polynomial()
{
    vector<Graph> components = connected_components();
    Polynomial result;
    result[0]=1;
    for (int i=0; i<components.size(); i++)
        result = result*components[i].chromatic_polynomial_rec();
    return result;
}
