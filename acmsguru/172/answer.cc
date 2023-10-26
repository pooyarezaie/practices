#include <iostream>
#include <vector>

const int MAX_N = 200 + 10;

std::vector<int> adj[MAX_N];
bool mark[MAX_N];
int color[MAX_N];

bool dfs(int v);

int main() {
  int n, m;
  std::cin >> n >> m;
  for (int i = 0; i < m; ++i) {
    int a, b;
    std::cin >> a >> b;
    adj[a - 1].push_back(b - 1);
    adj[b - 1].push_back(a - 1);
  }

  for (int i = 0; i < n; ++i) {
    if (mark[i]) {
      continue;
    }
    if (!dfs(i)) {
      std::cout << "no" << std::endl;
      return 0;
    }
  }
  std::vector<int> first_day_subjects;
  for (int i = 0; i < n; ++i) {
    if (color[i] == 0) {
      first_day_subjects.push_back(i + 1);
    }
  }
  std::cout << "yes" << std::endl << first_day_subjects.size() << std::endl;
  for (auto subject : first_day_subjects) {
    std::cout << subject << " ";
  }
  std::cout << std::endl;
  return 0;
}

bool dfs(int v) {
  mark[v] = true;
  for (int u : adj[v]) {
    if (mark[u] && color[u] == color[v]) {
      return false;
    }
    if (mark[u]) {
      continue;
    }
    color[u] = 1 - color[v];
    if (!dfs(u)) {
      return false;
    }
  }
  return true;
}