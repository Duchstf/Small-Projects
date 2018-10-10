#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <cmath>

using std::make_pair;
using std::pair;
using std::vector;

/*
Given n points on a plane and an integer k, compute the largest possible value of d such that 
the given points can be partitioned into k non-empty subsets in such a way that the distance 
between any two points from different subsets is at least d.
*/

//TO-DO:
//1. ~Implement disjoint sets
//2. ~Implement distance function and create a sorted-by-distance vector
//3. ~Adapt the krusal's algorithm
//!KEY POINT: when getting the right number of set, the next smallest
//distance between any sets is the result.

struct DisjointSetsElement
{
  int parent, rank;

  DisjointSetsElement(int parent = -1, int rank = 0) : parent(parent), rank(rank) {}
};

class DisjointSets
{
public:
  DisjointSets(int num_vertex) : num_vertex(num_vertex), sets(num_vertex), num_uniques(num_vertex)
  {
    for (int i = 0; i < num_vertex; i++)
    {
      sets[i].parent = i;
    }
  }

  int Find(int vertex)
  {
    //find the vertex's parent and compress path
    if (vertex != sets[vertex].parent)
    {
      sets[vertex].parent = Find(sets[vertex].parent);
    }
    return sets[vertex].parent;
  }

  void Union(int u, int v)
  {
    //merge set containing v into the set containing vertex u
    int realDestination = Find(u);
    int realSource = Find(v);
    //!Consider find the real root of v as well, but in this algorithm is not necessary
    //!Consider implement union by rank, this is not necessary for this algorithm
    if (realDestination != realSource)
    {
      // merge two components
      // use union by rank heuristic
      if (sets[realDestination].rank < sets[realSource].rank)
      {
        sets[realDestination].parent = realSource;
      }
      else
      {
        sets[realSource].parent = realDestination;
        if (sets[realDestination].rank == sets[realSource].rank)
        {
          sets[realDestination].rank++;
        }
      }
      num_uniques--;
    }
  }
  //TO-DO: implement this
  int num_unique_components()
  {
    return num_uniques;
  }

private:
  int num_vertex;
  vector<DisjointSetsElement> sets;
  int num_uniques; //Number of unique components;
};

double compute_distance(int x1, int y1, int x2, int y2)
{
  return sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));
}

struct Edges
{
  pair<int, int> vertices;
  double distance;
};

//Boolean function to sort edge vector
bool sortbydist(const Edges &a, const Edges &b)
{
  return a.distance < b.distance;
}

double clustering(vector<int> x, vector<int> y, int k)
{
  //Compute distances from all edges
  int V = x.size();
  vector<Edges> E;
  for (int i = 0; i < V; i++)
  {
    for (int j = i + 1; j < V; j++)
    {
      double dist = compute_distance(x[i], y[i], x[j], y[j]);
      Edges edge;
      edge.distance = dist;
      edge.vertices = make_pair(i, j);
      E.push_back(edge);
    }
  }
  //Sort E
  std::sort(E.begin(), E.end(), sortbydist);
  //Kruskal's Algorithm
  DisjointSets X(V);
  for (int i = 0; i < E.size(); i++)
  {
    if (X.Find(E[i].vertices.first) != X.Find(E[i].vertices.second))
    {
      if (X.num_unique_components() == k)
        return E[i].distance;
      X.Union(E[i].vertices.first, E[i].vertices.second);
    }
  }
}

int main()
{
  size_t n;
  int k;
  std::cin >> n;
  vector<int> x(n), y(n);
  for (size_t i = 0; i < n; i++)
  {
    std::cin >> x[i] >> y[i];
  }
  std::cin >> k;
  std::cout << std::setprecision(10) << clustering(x, y, k) << std::endl;
}
