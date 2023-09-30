#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

const int MAX_N = 200 + 10;
const int INF = (1 << 30);
int n, m;
std::vector<int> adj[MAX_N], color[MAX_N];
int distance[3][MAX_N];

void find_shortest_path(int v);

int main() {
  std::cin >> n >> m;
  for (int i = 0; i < m; ++i) {
    int v, u, c;
    std::cin >> v >> u >> c;
    adj[v - 1].push_back(u - 1);
    color[v - 1].push_back(c - 1);
  }

  find_shortest_path(0);

  int min_distance = std::min(distance[0][n - 1], distance[1][n - 1]);
  min_distance = std::min(min_distance, distance[2][n - 1]);

  if (min_distance < INF) {
    std::cout << min_distance << std::endl;
  } else {
    std::cout << -1 << std::endl;
  }
  return 0;
}

void find_shortest_path(int v) {
  for (int i = 0; i < 3; ++i) {
    std::fill(distance[i], distance[i] + MAX_N, INF);
  }

  std::queue<int> q;
  q.push(v);
  distance[0][v] = distance[1][v] = distance[2][v] = 0;

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (int i = 0; i < adj[u].size(); ++i) {
      int w = adj[u][i], c1 = color[u][i];
      if (distance[c1][w] >= INF) {
        int c2 = (c1 + 1) % 3, c3 = (c1 + 2) % 3;
        int d = std::min(distance[c2][u], distance[c3][u]) + 1;
        if (d < INF) {
          distance[c1][w] = d;
          q.push(w);
        }
      }
    }
  }
}