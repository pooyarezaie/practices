#include <iostream>
#include <fstream>
#include <vector>

const long long MOD = 1e9 + 7;
const int COLORS = 3;

int N, K;
std::vector<std::vector<int>> adj;

// w[v][i] = ways to color subtree of v with v colored as c_i
std::vector<long long> w[COLORS];

std::vector<int> color;

void dfs(int r, int parrent) {
    long long wr[COLORS] = {1, 1, 1};
    int cr = color[r];

    // already colored
    if (cr >= 0) {
        wr[(cr+1) % COLORS] = 0;
        wr[(cr+2) % COLORS] = 0;
    }

    for (int c : adj[r]) {
        if (c == parrent) continue;
        dfs(c, r);

        for (int i = 0; i < COLORS; ++i) {
            long long ci = w[(i+1) % COLORS][c] + w[(i+2) % COLORS][c];
            wr[i] = (wr[i] * ci) % MOD;
        }
    }
    for (int i = 0; i < COLORS; ++i) {
        w[i][r] = wr[i];
    }
}

int main() {
    std::ifstream fin("barnpainting.in");
    std::ofstream fout("barnpainting.out");

    fin >> N >> K;
    adj.assign(N, {});
    color.assign(N, -1);
    for (int i = 0; i < COLORS; ++i) {
        w[i].assign(N, -1);
    }

    for (int i = 0; i < N - 1; ++i) {
        int x, y;
        fin >> x >> y;
        --x; --y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }

    for (int i = 0; i < K; ++i) {
        int b, c;
        fin >> b >> c;
        --b; --c;
        color[b] = c;
    }

    dfs(0, -1);

    fout << (w[0][0] + w[1][0] + w[2][0]) % MOD << std::endl;
}

