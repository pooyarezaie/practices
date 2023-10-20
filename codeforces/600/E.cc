#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

const int MAX_N = 100 * 1000 + 10;

int color[MAX_N];
std::vector<int> adj[MAX_N];
std::map<int, int> color_count[MAX_N];
std::map<int, long long int> count_sum[MAX_N];
long long int answer[MAX_N];

void dfs(int v, int parent);
void merge(int v, int u);

int main() {
  std::ios::sync_with_stdio(false);

  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    std::cin >> color[i];
  }
  for (int i = 1; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    adj[y - 1].push_back(x - 1);
  }

  dfs(0, -1);

  for (int i = 0; i < n; ++i) {
    std::cout << answer[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

void dfs(int v, int parent) {
  color_count[v][color[v]] = 1;
  count_sum[v][1] = color[v];
  answer[v] = color[v];

  for (int u : adj[v]) {
    if (u == parent) {
      continue;
    }
    dfs(u, v);
    merge(v, u);
  }
  answer[v] = count_sum[v].rbegin()->second;
}
void merge(int v, int u) {
  if (color_count[v].size() < color_count[u].size()) {
    std::swap(color_count[v], color_count[u]);
    std::swap(count_sum[v], count_sum[u]);
  }

  for (auto c : color_count[u]) {
    count_sum[v][color_count[v][c.first]] -= c.first;
    color_count[v][c.first] += c.second;
    count_sum[v][color_count[v][c.first]] += c.first;
  }
}