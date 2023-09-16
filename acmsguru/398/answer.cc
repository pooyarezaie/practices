#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

const int MAX_N = 50 + 5;

std::vector<int> adj[MAX_N];
int mark[MAX_N], distance[MAX_N], parrent[MAX_N];

std::vector<int> solve(int v);

int main()
{
    int n, x;
    std::cin >> n >> x;
    for (int i = 0; i < n; ++i)
    {
        int degree;
        std::cin >> degree;
        for (int j = 0; j < degree; ++j)
        {
            int neighbour;
            std::cin >> neighbour;
            adj[i].push_back(neighbour - 1);
        }
    }

    std::vector<int> solution = solve(x - 1);
    std::cout << solution.size() << std::endl;
    for (int i = 0; i < solution.size(); ++i)
    {
        std::cout << solution[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}

std::vector<int> solve(int s)
{
    std::fill(distance, distance + MAX_N, -1);
    std::fill(parrent, parrent + MAX_N, -1);
    std::queue<int> q;

    mark[s] = 1; distance[s] = 0; q.push(s);
    while (!q.empty()) {
        int v = q.front(); q.pop();

        if (distance[v] > 2) {
            break;
        }

        for (int i=0; i < adj[v].size(); ++i) {
            int u = adj[v][i];
            if (mark[u] == 0) {
                q.push(u); mark[u] = 1; 
                distance[u] = distance[v] + 1;
                parrent[u] = v; 
            }
        }
    }

    std::vector<int> solution; 
    for (int i = 0; i < MAX_N; ++i) {
        if (distance[i] == 2) {
            solution.push_back(i+1);
        }
    }
    std::sort(solution.begin(), solution.end());
    return solution;
}