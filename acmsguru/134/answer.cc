#include <algorithm>
#include <iostream>
#include <vector>

const int MAX_N = 16 * 1000 + 10;

std::vector<int> adj[MAX_N];
int subtree_size[MAX_N], associated_value[MAX_N];
int n;

void dfs(int v, int parent);

int main() {
  std::cin >> n;
  for (int i = 1; i < n; ++i) {
    int a, b;
    std::cin >> a >> b;
    adj[a - 1].push_back(b - 1);
    adj[b - 1].push_back(a - 1);
  }

  dfs(0, -1);

  int min_value = *std::min_element(associated_value, associated_value + n);
  std::vector<int> centroids;
  for (int i = 0; i < n; ++i) {
    if (associated_value[i] == min_value) {
      centroids.push_back(i + 1);
    }
  }
  std::sort(centroids.begin(), centroids.end());

  std::cout << min_value << " " << centroids.size() << std::endl;
  for (int centroid : centroids) {
    std::cout << centroid << " ";
  }
  std::cout << std::endl;
  return 0;
}

void dfs(int v, int parent) {
  subtree_size[v] += 1;
  for (auto u : adj[v]) {
    if (u == parent) {
      continue;
    }
    dfs(u, v);
    subtree_size[v] += subtree_size[u];
  }
  int av = n - subtree_size[v];
  for (auto u : adj[v]) {
    if (u == parent) {
      continue;
    }
    av = std::max(av, subtree_size[u]);
  }
  associated_value[v] = av;
}