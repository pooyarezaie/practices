#include <iostream>
#include <vector> 

const int MAX_N = 2 * 100 * 1000 + 10;

int solve(const std::vector<int> &boss, int n);

int dp[2][MAX_N];

int main() {
    int t; std::cin >> t;
    for (int z = 0; z < t; ++z) {
        std::vector<int> boss;
        int n;
        
        std::cin >> n;
        for (int i = 0; i < n; ++i) {
            int a; std::cin >> a;
            boss.push_back(a);
        }

        std::cout << solve(boss, n) << std::endl;
    }
    return 0;
}

int solve(const std::vector<int> &boss, int n) {
    dp[0][n] = dp[1][n] = 0;
    dp[0][n-1] = 0; dp[1][n-1] = boss[n-1];

    for (int i = n-2; i >= 0; --i) {
        dp[0][i] = std::min(dp[1][i+1], dp[1][i+2]);
        dp[1][i] = std::min(boss[i] + dp[0][i+1], 
        boss[i] + boss[i+1] + dp[0][i+2]);
    }
    return dp[1][0];
}