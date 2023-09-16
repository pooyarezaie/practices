#include <iostream>

using namespace std;

const int maxn = 10*1000+100;

int deg[maxn];
int m, n;
int q;

int main()
{
    cin>>n>>m;
    int c;
    for (int i=0; i < n; ++i)
        for (int j=0; j < n; ++j) {
            cin>>c;
            if (c>=0) {
                ++deg[i];
            }
        }
    cin>>q;
    int v;
    for (int i=0; i < q; ++i) {
        cin>>v;
        cout<<deg[v]<<endl;
    }
    return 0;
}
