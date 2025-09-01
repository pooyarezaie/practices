// https://codeforces.com/gym/101908/problem/B

/*
We model this as an impartial combinatorial game with marbles as subgames.
At first it looks like Nim-sum might not apply, because the game ends
immediately when ANY marble reaches (0,0), not when ALL marbles do.

The trick is to notice:
- If a marble is already on the main diagonal (l == c), the first player can
  move it straight to (0,0) and win immediately.
- Otherwise, any move that places a marble on the diagonal (x == y) or an axis
  (x == 0 or y == 0) is losing, because the opponent can then win in one move.
  So optimal play will avoid those squares entirely (except the final move).

Therefore we restrict the game to "safe" positions where l != c and l, c > 0,
and only allow moves that remain safe. In this restricted game, normal
Sprague–Grundy theory applies: each marble position has a Grundy number, and
the XOR of all marbles' Grundy numbers tells us if the position is winning.

    c=0   c=1   c=2
r=0 [ - ] [ - ] [ - ]
r=1 [ - ] [ - ] [G=0]
r=2 [ - ] [G=0] [ - ]

*/

#include <iostream>
#include <vector>

const int MAX_N = 100 + 10;

int grundy[MAX_N][MAX_N];


void calculate_grundy_numbers();
int mex(const std::vector<int>& v);

int main() {
    calculate_grundy_numbers();
    int n; std::cin>>n;
    int ans = 0;
    for (int k=0; k<n; ++k) {
        int a, b; std::cin>>a>>b;
        if ((a == b) || (a == 0) || (b == 0)) {
            std::cout<<"Y"<<std::endl;
            return 0;
        }
        ans ^= grundy[a][b];
    }
    if (ans > 0) {
        std::cout<<"Y"<<std::endl;
    } else {
        std::cout<<"N"<<std::endl;
    }
    return 0;
}

void calculate_grundy_numbers() {
    grundy[1][2] = grundy[2][1] = 0;
    for (int i=1; i<MAX_N; ++i) {
        for (int j=1; j<MAX_N; ++j) {
            if (i == j) {
                continue;
            }
            std::vector<int> nexts;
            for (int u=1; u<i; ++u) {
                if ((i-u) == j) continue;
                nexts.push_back(grundy[i-u][j]);
            }
            for (int u=1; u<j; ++u) {
                if ((j-u) == i) continue;
                nexts.push_back(grundy[i][j-u]);
            }
            for (int u=std::min(i, j)-1; u>0; --u) {
                nexts.push_back(grundy[i-u][j-u]);
            }
            grundy[i][j] = mex(nexts);
        }
    }
}


int mex(const std::vector<int>& v) {
    std::vector<bool> mark(v.size() + 1, 0);
    for (int i : v) {
        if (i <= (int) v.size()) {
            mark[i] = 1;
        }
    }
    int mex = 0;
    while (mark[mex]) {
        ++mex;
    }
    return mex;
}