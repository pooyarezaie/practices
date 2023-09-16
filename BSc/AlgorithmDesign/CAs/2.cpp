#include <iostream>
#include <string>
#include <climits>
#include <cstring>

using namespace std;

const int maxm=400+10;
const int TEN=10;

string t, q;
int n, m, sigma, delta;

int dp[2][maxm];

int main() {
    ios::sync_with_stdio(false);
    int k; cin>>k;

    while (k--) {
        memset(dp, 0, sizeof(dp));
        cin>>n>>m>>sigma>>delta;
        cin>>t>>q;

        int ans=INT_MIN;

        for (int j=0; j < m; ++j)
            dp[n%2][j]=(j-m)*sigma;

        for (int i=n-1; i >= 0; --i)
            for (int j=m-1; j >= 0; --j) {
                int g=((t[i]!=q[j])?-delta:TEN);
                dp[i%2][j]=max(max(dp[(i+1)%2][j]-sigma,dp[i%2][j+1]-sigma)
                        , dp[(i+1)%2][j+1]+g);
                if (j==0)
                    ans=max(ans, dp[i%2][j]);
            }
        cout<<ans<<endl;
    }
}
