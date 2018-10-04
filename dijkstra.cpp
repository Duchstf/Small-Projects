#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using std::make_pair;
using std::pair;
using std::vector;

class minQueue
{
  vector<pair<int, long long>> queue;

public:
  void add_with_priority(int v, long long dist)
  {
    queue.push_back(make_pair(v, dist)); // Consider modifying this if this data structure is used on another algorithm
  }

  //Comparison function to specify in heap
  struct Comp
  {
    bool operator()(const pair<int, long long> &left, const pair<int, long long> &right)
    {
      return left.second >= right.second;
    }
  };

  void turn_to_heap()
  {
    std::make_heap(queue.begin(), queue.end(), Comp());
  }

  int extract_min()
  {
    std::pop_heap(queue.begin(), queue.end(), Comp());
    int vertex_min_priority = queue.back().first; //vertex with min priority
    queue.pop_back();
    return vertex_min_priority;
  }

  void decrease_priority(int v, long long dist)
  {
    for (int i = 0; i < queue.size(); i++)
    {
      if (v == queue[i].first)
        queue[i].second = dist; //O(n)
    }
    std::make_heap(queue.begin(), queue.end(), Comp()); // Reorder priority
  }

  bool empty()
  {
    if (queue.size() > 0)
      return false;
    return true;
  }
};

int distance(vector<vector<int>> &adj, vector<vector<int>> &cost, int s, int t)
{ /**
  Dijkstra's Algorithm on Graphs
  */

  //Initialize distance and previous-vertex vector
  long long lim = std::numeric_limits<long long>::max() - 99999; //potential error, modify if commin distance used is larger than this
  vector<long long> dist(adj.size(), lim);
  vector<int> prev(adj.size(), -1);
  minQueue Q;
  dist[s] = 0; //Distance to itself

  for (int i = 0; i < adj.size(); i++)
  {
    Q.add_with_priority(i, dist[i]);
  }

  Q.turn_to_heap();

  while (!Q.empty())
  {
    int u = Q.extract_min();
    for (int j = 0; j < adj[u].size(); j++)
    {
      int v = adj[u][j];
      if (dist[v] > dist[u] + cost[u][j])
      {
        dist[v] = dist[u] + cost[u][j];
        prev[v] = u;
        Q.decrease_priority(v, dist[v]);
      }
    }
  }

  if (dist[t] == lim)
    return -1;
  else
  {
    return dist[t];
  }
}

int main()
{
  int n, m;
  std::cin >> n >> m;
  vector<vector<int>> adj(n, vector<int>());
  vector<vector<int>> cost(n, vector<int>());
  for (int i = 0; i < m; i++)
  {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    cost[x - 1].push_back(w);
  }
  int s, t;
  std::cin >> s >> t;
  s--, t--;
  std::cout << distance(adj, cost, s, t);
}
