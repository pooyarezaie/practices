#include <iostream>
#include <vector>
#include "graph.hpp"
using namespace std;

void init_matrix(vector<vector<bool> >& mat, int n)
{
    for (int i=0; i < n; i++) {
        vector<bool> row;
        row.resize(n);
        for (int j=0;j<n; ++j)
            row[j]=0;
        mat.push_back(row);
    }
}

int main(){
    int n, m;
    cin>>n>>m;
    vector<vector<bool> > matrix;

    init_matrix(matrix, n);

    for (int i=0; i < m; ++i ) {
        int x, y;
        cin>>x>>y;
        matrix[x][y]=1;
        matrix[y][x]=1;
    }
 
    Graph G(matrix);
    G.chromatic_polynomial().print('k');
    cout<<endl;
    return 0;
}
