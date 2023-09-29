#include <iostream>
#include <map>
#include <vector>

const int MAX_N = 10;
const std::string NO_SOLUTION = "No solution";

int edge_num[MAX_N][MAX_N];
std::vector<int> dominos[MAX_N][MAX_N];
std::vector<int> adj[MAX_N];

void dfs_eulerian(int v, std::vector<int> &list);

int main() {

  int m;
  std::cin >> m;
  for (int i = 0; i < m; ++i) {
    int v, u;
    std::cin >> v >> u;
    edge_num[v][u]++;
    edge_num[u][v]++;
    adj[v].push_back(u);
    adj[u].push_back(v);
    dominos[v][u].push_back(i + 1);
  }

  // check if graph is eulerian
  int odd_vertices = 0, start = -1;
  for (int i = 0; i < MAX_N; ++i) {
    int degree = adj[i].size();
    if (start == -1 && degree > 0) {
      start = i;
    }
    if (degree % 2 == 1) {
      odd_vertices += 1;
      start = i;
    }
  }
  if (odd_vertices != 2 && odd_vertices != 0) {
    std::cout << NO_SOLUTION << std::endl;
    return 0;
  }

  std::vector<int> list;
  dfs_eulerian(start, list);

  if (list.size() != (m + 1)) {
    std::cout << NO_SOLUTION << std::endl;
    return 0;
  }

  for (int i = 0; i < m; ++i) {
    int v = list[i], u = list[i + 1];
    if (dominos[v][u].size() > 0) {
      std::cout << dominos[v][u].back() << " +" << std::endl;
      dominos[v][u].pop_back();
    } else if (dominos[u][v].size() > 0) {
      std::cout << dominos[u][v].back() << " -" << std::endl;
      dominos[u][v].pop_back();
    }
  }
  return 0;
}

void dfs_eulerian(int v, std::vector<int> &list) {
  while (!adj[v].empty()) {
    int u = adj[v].back();
    adj[v].pop_back();

    if (edge_num[v][u] > 0) {
      edge_num[v][u]--;
      edge_num[u][v]--;
      dfs_eulerian(u, list);
    }
  }
  list.push_back(v);
}