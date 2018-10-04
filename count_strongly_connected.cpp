#include <algorithm>
#include <iostream>
#include <vector>

using std::pair;
using std::vector;

//I was writing thi for an online class so didn't comment much on this, SO SORRY!

class SCCsCounter
{
public:
  SCCsCounter(int n)
  {
    visited.resize(n, false);
    traversal_visited.resize(n,false);
    pre.resize(n);
    post.resize(n);
  }

  void read_vec(vector<vector<int>> input_vec1,vector<vector<int>> input_vec2 )
  {
    adj = input_vec1;
    reversed_adj = input_vec2;
  }

  void previsit(int v)
  {
    pre[v] = clock;
    clock++;
  }

  void postvisit(int v)
  {
    post[v] = clock;
    clock++;
  }

  void explore(int v) // Use this when traver the graph in revesed post order
  {
    traversal_visited[v] = true;
    for (int i = 0; i < adj[v].size(); i++)
    {
      if (!traversal_visited[adj[v][i]])
      {
        explore(adj[v][i]);
      }
    }
  }

  void dfs()
  {
    for (int i = 0; i < adj.size(); i++)
    {
      if (!traversal_visited[i])
      {
        explore(i);
      }
    }
  }

  //Explore and dfs for reversed graph

  void reversed_explore(int v)
  {
    visited[v] = true;
    previsit(v);
    for (int i = 0; i < reversed_adj[v].size(); i++)
    {
      if (!visited[reversed_adj[v][i]])
      {
        reversed_explore(reversed_adj[v][i]);
      }
    }
    postvisit(v);
  }

  void reversed_dfs()
  {
    for (int i = 0; i < reversed_adj.size(); i++)
    {
      if (!visited[i])
      {
        reversed_explore(i);
      }
    }
  }


  int number_of_strongly_connected_components()
  { 
    /*Count the number of strongly_connected_components in a directed graph
    */
    reversed_dfs();
    vector<pair<int, int>> mixed(reversed_adj.size());
    vector<int> order;
    int result = 0;
    for (int i = 0; i < reversed_adj.size(); i++)
    {
      mixed.push_back(std::make_pair(i, post[i]));
    }

    std::sort(mixed.begin(),mixed.end(),[](const pair<int, int> &i, const pair<int, int> &j){
      return i.second > j.second;
    });

    for (int j = 0; j < adj.size(); j++)
    {
      order.push_back(mixed[j].first);
    }

    for(int k = 0; k < order.size(); k++){
      if (!traversal_visited[order[k]]){
        explore(order[k]);
        result++;
      }
    }

    return result;
  }

private:
  int clock = 0; // Keep track of time
  vector<int> pre;
  vector<int> post;
  int n;
  vector<bool> visited;
  vector<bool> traversal_visited;
  vector<vector<int>> adj;
  vector<vector<int>> reversed_adj;
};

int main()
{
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int>> adj(n, vector<int>());
  vector<vector<int>> reversed_adj(n, vector<int>());
  for (size_t i = 0; i < m; i++)
  {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    //Reverse Graph
    reversed_adj[y - 1].push_back(x - 1);
  }

  SCCsCounter Count(n);
  Count.read_vec(adj,reversed_adj);
  std::cout << Count.number_of_strongly_connected_components();
}
