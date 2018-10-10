#include <iostream>
#include <vector>
#include <limits>

using std::vector;

int negative_cycle(vector<vector<int>> &adj, vector<vector<int>> &cost)
{
  /* 
  Detect negative circle in a directed graph using BellmanFord's Algorithm
  */
  long long lim = std::numeric_limits<long long>::max() - 99999;
  vector<long long> dist(adj.size(), lim);
  //vector<int> prev(adj.size(), -1); If need to reconstruct circle have to use this


  for (int k = 0; k <= adj.size(); k++)
  { //Repeat V times to find out whether the dist change on V iteration
    for (int i = 0; i < adj.size(); i++)
    {
      for (int j = 0; j < adj[i].size(); j++){
        int u = i;
        int v = adj[u][j];
        if (dist[v] > dist[u] + cost[u][j]){
          dist[v] = dist[u] + cost[u][j];
          if (k == adj.size()) return 1; // Check in at V iteration
          //prev[v] = u;
        }
      }
    }
  }
  return 0;
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
  std::cout << negative_cycle(adj, cost);
}
