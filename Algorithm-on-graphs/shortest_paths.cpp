#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <algorithm>

using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;
using std::make_pair;

void update_reachable(vector<vector<int> > &adj, int s, vector<int> &reachable) {
  vector<int> dist(adj.size(),-1);
  dist[s] = 0;
  queue<int> Q;
  Q.push(s);
  while (!Q.empty()){
    int u = Q.front();
    reachable[u] = 1;
    Q.pop();
    if (adj[u].size() > 0){
      for(int v =0; v<adj[u].size(); v++){
        if (dist[adj[u][v]] == -1){
          Q.push(adj[u][v]);
          dist[adj[u][v]] = dist[u] + 1;
        }
      }
    }
  }
}

void BFS(vector<vector<int> > &adj, int s, vector<int> &shortest) {
  vector<int> dist(adj.size(),-1);
  dist[s] = 0;
  queue<int> Q;
  Q.push(s);
  while (!Q.empty()){
    int u = Q.front();
    shortest[u] = 0;
    Q.pop();
    if (adj[u].size() > 0){
      for(int v =0; v<adj[u].size(); v++){
        if (dist[adj[u][v]] == -1){
          Q.push(adj[u][v]);
          dist[adj[u][v]] = dist[u] + 1;
        }
      }
    }
  }
}

void shortest_paths(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, vector<long long> &distance, vector<int> &reachable, vector<int> &shortest) {
  /*
    Problem: exchange money optimally
    Given an directed graph with possibly negative edge weights and with n vertices and m edges 
    as well as its vertex s, compute the length of shortest paths from s to all other vertices of the graph.
    Output: • “*”,if there is no path from s to u;
            • “-”, if there is a path from s to u, but there is no shortest path from s to u (that is, the distance
              from s to u is −∞);
            • the length of a shortest path otherwise.
  */
 long long lim = std::numeric_limits<long long>::max();
  queue<int> Q; 
  distance[s] = 0;
  //First run Bellmanford's
  for (int k = 0; k <= adj.size(); k++)
  { //Repeat V times to find out whether the dist change on Vth iteration
    for (int i = 0; i < adj.size(); i++)
    {
      for (int j = 0; j < adj[i].size(); j++){
        int u = i;
        int v = adj[u][j];
        long long a = distance[v];
        long long b = (distance[u] == lim ? distance[u]:(distance[u]+ cost[u][j]));
        if (a > b){
          distance[v] = distance[u] + cost[u][j];
          if (k == adj.size()){
            Q.push(v);
          } // Check in at Vth iteration
        }
      }
    }
  }
  //Breath-First-Search for those in Q
  while (!Q.empty()){
    int u = Q.front();
    shortest[u] = 0;
    Q.pop();
    BFS(adj,u,shortest);
  }
  //BFS to identify reachable
  update_reachable(adj,s,reachable);
}

int main() {
  int n, m, s;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  vector<vector<int> > cost(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    cost[x - 1].push_back(w);
  }
  std::cin >> s;
  s--;
  vector<long long> distance(n, std::numeric_limits<long long>::max());//Prevent number outflow
  vector<int> reachable(n, 0);
  vector<int> shortest(n, 1);
  shortest_paths(adj, cost, s, distance, reachable, shortest);
  for (int i = 0; i < n; i++) {
    if (!reachable[i]) {
      std::cout << "*\n";
    } else if (!shortest[i]) {
      std::cout << "-\n";
    } else {
      std::cout << distance[i] << "\n";
    }
  }
}
