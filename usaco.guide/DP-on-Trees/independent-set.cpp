#include <iostream>
#include <vector>

const long long MOD = 1e9 + 7;

int N;
std::vector<std::vector<int>> adj;
// w[v] = ways to color subtree of v with v white
// b[v] = ways to color subtree of v with v black
std::vector<long long> w, b;

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
    std::cin >> N;
    adj.assign(N + 1, {});
    w.assign(N + 1, 0);
    b.assign(N + 1, 0);

    for (int i = 0; i < N - 1; i++) {
        int x, y;
        std::cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }

    dfs(1, 0);

    std::cout << (w[1] + b[1]) % MOD << std::endl;
    return 0;
}
