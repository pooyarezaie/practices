#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

typedef pair<int, int> Unit;
typedef vector<Unit> Block;

const int maxn=30+2, maxp=10+2;
const Unit PNULL=make_pair(-1,-1);

void init();
Unit next_unit(int row, int col);
int solve(Unit u);
int is_fit(int index, Unit u);
void put(int index, Unit u);

int map[maxn][maxn];
Block blocks[maxp];
int used[maxp];
int n, m, p; int k;

int main()
{
    cin>>k;
    while (k--) {

        init();

        cin>>p>>m>>n;
        for (int i=0; i < p; ++i) {
            int c; cin>>c;
            for (int j=0; j < c; ++j) {
                int row, col; cin>>row>>col;
                blocks[i].push_back(make_pair(row, col));
            }
        }

        for (int i=0; i < p; ++i)
            sort(blocks[i].begin(), blocks[i].end());
        for (int i=0; i < p; ++i)
            for (int j=blocks[i].size()-1; j>=0; --j) {
                blocks[i][j].first-=blocks[i][0].first;
                blocks[i][j].second-=blocks[i][0].second;
            }

        cout<<solve(make_pair(0, 0))<<endl;
    }
}
int solve(Unit u)
{
    if (u==PNULL)
        return 1;
    for (int i=0; i < p; ++i)
        if (!used[i]&&is_fit(i, u)) {
            put(i, u); used[i]=1;
            Unit nu=next_unit(u.first, u.second);
            if(solve(nu))
                return 1;
            else {
                put(i, u);
                used[i]=0;
            }
        }
    return 0;
}
Unit next_unit(int row, int col)
{
    int it=row*n+col;
    while (it<m*n) {
        int nrow=it/n;
        int ncol=it-nrow*n;
        if (!map[nrow][ncol])
            return make_pair(nrow, ncol);
        ++it;
    }
    return PNULL;
}
int is_fit(int index, Unit u)
{
    for (int i=0; i < blocks[index].size(); ++i) {
        int crow=blocks[index][i].first+u.first;
        int ccol=blocks[index][i].second+u.second;
        if (crow<0 || ccol<0 || crow>=m || ccol>=n)
            return 0;
        if (map[crow][ccol])
            return 0;
    }
    return 1;
}
void put(int index, Unit u)
{
    for(int i=0; i< blocks[index].size(); ++i) {
        int crow=blocks[index][i].first+u.first;
        int ccol=blocks[index][i].second+u.second;
        map[crow][ccol] ^= 1;
    }
}
void init()
{
    memset(map, 0, sizeof map);
    for (int i=0; i < maxp; ++i) {
        blocks[i].clear();
        used[i]=0;
    }
}
