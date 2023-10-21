#include <cstdio>

#include <map>
#include <vector>

const int MAX_N = 500 * 1000 + 10;

std::vector<int> adj[MAX_N];
char letters[MAX_N];
bool answer[MAX_N];

struct QueryValue {
  int index;
  int h;
};
std::map<int, std::vector<QueryValue>> queries;
std::map<int, int> hight_letters[MAX_N];

void dfs(int v, int parent, int hight);
void merge(int v, int u);

int main() {
  int n, m;
  scanf("%d%d", &n, &m);

  for (int i = 1; i < n; ++i) {
    int parent;
    scanf("%d", &parent);
    parent--;
    adj[i].push_back(parent);
    adj[parent].push_back(i);
  }
  scanf("%s", letters);

  for (int i = 0; i < m; ++i) {
    int v, h;
    scanf("%d %d", &v, &h);
    queries[v - 1].push_back({i, h});
  }

  dfs(0, -1, 1);

  for (int i = 0; i < m; ++i) {
    printf("%s\n", answer[i] ? "Yes" : "No");
  }
  return 0;
}

void dfs(int v, int parent, int hight) {
  hight_letters[v][hight] ^= (1 << (letters[v] - 'a'));
  for (auto u : adj[v]) {
    if (u == parent) {
      continue;
    }
    dfs(u, v, hight + 1);
    merge(v, u);
  }

  for (auto q : queries[v]) {
    int state = hight_letters[v][q.h];
    answer[q.index] = (state & (state - 1)) == 0;
  }
}

void merge(int v, int u) {
  if (hight_letters[v].size() < hight_letters[u].size()) {
    hight_letters[v].swap(hight_letters[u]);
  }
  for (auto h : hight_letters[u]) {
    hight_letters[v][h.first] ^= h.second;
  }
}