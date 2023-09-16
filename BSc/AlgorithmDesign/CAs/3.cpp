#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

typedef long long ll;

const int maxn=100*1000+10;

ll fx[maxn], fy[maxn];
ll sx[maxn], sy[maxn];
int m, n, k;

int main()
{
    ios::sync_with_stdio(false);
    cin>>k;
    while (k--) {
        cin>>n>>m;
        for (int i=0; i < n; ++i)
            cin>>fx[i]>>fy[i];
        for (int i=0; i < m; ++i)
            cin>>sx[i]>>sy[i];

        vector<pair <ll, ll> > pans, ans;

        for (int i=0; i < n; ++i) {
            int p=lower_bound(sx, sx+m, fx[i])-sx;
            if(sx[p]!=fx[i])
                --p;
            pans.push_back(make_pair(fx[i], max(fy[i],sy[p])));
        }
        for (int i=0; i < m; ++i) {
            int p=lower_bound(fx, fx+n, sx[i])-fx;
            if(fx[p]!=sx[i])
                --p;
            pans.push_back(make_pair(sx[i], max(sy[i],fy[p])));
        }

        sort(pans.begin(), pans.end());

        for (int i=0; i < pans.size(); ++i)
            if(!i)
                ans.push_back(pans[i]);
            else if(i && (pans[i].second != pans[i-1].second))
                ans.push_back(pans[i]);
        cout<<ans.size()<<endl;
        for (int i=0; i < ans.size(); ++i)
            cout<<ans[i].first<<" "<<ans[i].second<<endl;
    }
}
