#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

const int MAX_N = 10 * 1000 + 10;
std::vector<int> adj[MAX_N], weight[MAX_N];

int bfs(int v, long long int distance[MAX_N]);

int main() {
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  for (int i = 1; i < n; ++i) {
    int p, w;
    std::cin >> p >> w;
    adj[i].push_back(p - 1);
    adj[p - 1].push_back(i);
    weight[i].push_back(w);
    weight[p - 1].push_back(w);
  }

  long long int u_distance[MAX_N], v_distance[MAX_N];
  int u = bfs(0, u_distance);
  int v = bfs(u, u_distance);
  bfs(v, v_distance);

  for (int i = 0; i < n; ++i) {
    std::cout << std::max(u_distance[i], v_distance[i]) << std::endl;
  }

  return 0;
}

int bfs(int v, long long int distance[MAX_N]) {
  std::fill(distance, distance + MAX_N, -1);
  distance[v] = 0;

  std::queue<int> q;
  q.push(v);
  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (int i = 0; i < adj[u].size(); ++i) {
      int w = adj[u][i];
      int d = weight[u][i];
      if (distance[w] < 0) {
        distance[w] = distance[u] + d;
        q.push(w);
      }
    }
  }
  return std::max_element(distance, distance + MAX_N) - distance;
}