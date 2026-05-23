#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

int N;
vector<vector<int>> adj;
// w[v] = ways to color subtree of v with v white
// b[v] = ways to color subtree of v with v black
vector<long long> w, b;

void dfs(int r, int parent) {
    long long wr = 1, br = 1;
    for (int c : adj[r]) {
        if (c == parent) continue;
        dfs(c, r);
        wr = wr * (w[c] + b[c]) % MOD;
        br = br * w[c] % MOD;
    }
    w[r] = wr;
    b[r] = br;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    adj.assign(N + 1, {});
    w.assign(N + 1, 0);
    b.assign(N + 1, 0);

    for (int i = 0; i < N - 1; i++) {
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }

    dfs(1, 0);

    cout << (w[1] + b[1]) % MOD << "\n";
    return 0;
}
