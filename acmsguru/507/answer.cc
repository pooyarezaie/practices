#include <iostream>
#include <pthread.h>
#include <queue>
#include <set>
#include <vector>

const int MAX_N = 50 * 1000 + 100;
const long long int INF = (1ll << 31) - 1;

int answer[MAX_N];
std::vector<int> adj[MAX_N];
std::set<int> leaves[MAX_N];

void dfs(int v, int par);
int merge(std::set<int> &s1, std::set<int> &s2);

int main() {
  std::fill(answer, answer + MAX_N, INF);

  int n, m;
  std::cin >> n >> m;
  for (int i = 1; i < n; ++i) {
    int parent;
    std::cin >> parent;
    parent--;
    adj[i].push_back(parent);
    adj[parent].push_back(i);
  }
  for (int i = n - m; i < n; ++i) {
    int value;
    std::cin >> value;
    leaves[i].insert(value);
  }

  dfs(0, -1);

  for (int i = 0; i < n - m; ++i) {
    std::cout << answer[i] << " ";
  }
  std::cout << std::endl;
  return 0;
}

void dfs(int v, int par) {
  for (int i = 0; i < adj[v].size(); ++i) {
    int u = adj[v][i];
    if (u == par) {
      continue;
    }
    dfs(u, v);
    answer[v] = std::min(answer[v], answer[u]);
    answer[v] = std::min(answer[v], merge(leaves[v], leaves[u]));
  }
}

int merge(std::set<int> &s1, std::set<int> &s2) {
  if (s1.size() < s2.size()) {
    std::swap(s1, s2);
  }
  int min_diff = INF;
  for (auto e : s2) {
    std::set<int>::iterator pre, suc;
    pre = suc = s1.lower_bound(e);
    if (pre != s1.begin()) {
      pre--;
    }
    if (pre != s1.end()) {
      min_diff = std::min(min_diff, std::abs(*pre - e));
    }
    if (suc != s1.end()) {
      min_diff = std::min(min_diff, std::abs(*suc - e));
    }
    s1.insert(e);
  }
  return min_diff;
}