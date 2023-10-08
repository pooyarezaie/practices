// Problem: https://codeforces.com/problemset/problem/1882/D

#include <iostream>
#include <istream>
#include <vector>

struct Tree {
  int order;
  std::vector<long long int> a;
  std::vector<std::vector<int>> adj;
};

std::vector<int> subtree_size, parent;
std::vector<bool> mark;
std::vector<long long int> m;

void initialize(int n);
std::istream &operator>>(std::istream &in, Tree &tree);
void dfs(int v, const Tree &tree);
void dfs_and_compute_m(int v, const Tree &tree);

int main() {
  std::ios::sync_with_stdio(false);

  int num_of_test_cases;
  std::cin >> num_of_test_cases;

  while (num_of_test_cases--) {
    Tree tree;
    std::cin >> tree;

    initialize(tree.order);

    dfs(0, tree);

    for (int i = 0; i < tree.order; ++i) {
      m[0] += subtree_size[i] * (tree.a[parent[i]] ^ tree.a[i]);
    }
    mark.clear();
    mark.resize(tree.order);

    dfs_and_compute_m(0, tree);

    for (int i = 0; i < tree.order; ++i) {
      std::cout << m[i] << " ";
    }
    std::cout << std::endl;
  }
  return 0;
}

void dfs(int v, const Tree &tree) {
  mark[v] = true;
  subtree_size[v] = 1;
  for (int i = 0; i < tree.adj[v].size(); ++i) {
    int u = tree.adj[v][i];
    if (mark[u]) {
      continue;
    }
    parent[u] = v;
    dfs(u, tree);
    subtree_size[v] += subtree_size[u];
  }
}

void dfs_and_compute_m(int v, const Tree &tree) {
  mark[v] = true;
  for (int i = 0; i < tree.adj[v].size(); ++i) {
    int u = tree.adj[v][i];
    if (mark[u]) {
      continue;
    }
    m[u] = m[v] - subtree_size[u] * (tree.a[parent[u]] ^ tree.a[u]) +
           (tree.order - subtree_size[u]) * (tree.a[u] ^ tree.a[v]);
    dfs_and_compute_m(u, tree);
  }
}

std::istream &operator>>(std::istream &in, Tree &tree) {
  in >> tree.order;
  tree.a.resize(tree.order);
  tree.adj.resize(tree.order);
  for (int i = 0; i < tree.order; ++i) {
    in >> tree.a[i];
  }
  for (int i = 1; i < tree.order; ++i) {
    int u, v;
    in >> u >> v;
    tree.adj[u - 1].push_back(v - 1);
    tree.adj[v - 1].push_back(u - 1);
  }
  return in;
}

void initialize(int n) {
  subtree_size.clear();
  subtree_size.resize(n);

  parent.clear();
  parent.resize(n);

  mark.clear();
  mark.resize(n);

  m.clear();
  m.resize(n);
}