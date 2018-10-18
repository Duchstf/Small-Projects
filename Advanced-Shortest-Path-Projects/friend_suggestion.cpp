#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
#include <utility>

/*
Here I implement Bidirectional Dijkstra Algorithm, which is assigned in Advanced Week 6 of the
Cousera's course on Algorithm on String.

ATTENTION:

**This algorithm is just to calculate the distance between two nodes.
**More details are required to reconstruct the shortest path between two nodes.
*/

using namespace std;

// External vector of size 2 - for forward and backward search.
// Internal 2-dimensional vector is vector of adjacency lists for each node.
typedef vector<vector<vector<int>>> Adj;

// Distances can grow out of int type
typedef long long Len;

// Vector of two priority queues - for forward and backward searches.
// Each priority queue stores the closest unprocessed node in its head.
typedef vector<
    priority_queue<
        pair<Len, int>, vector<pair<Len, int>>, greater<pair<Len, int>>>>
    Queue;

const Len VERY_LARGE = numeric_limits<Len>::max() / 4;

class Bidijkstra
{
    // Number of nodes
    int n_;
    // Graph adj_[0] and cost_[0] correspond to the initial graph,
    // adj_[1] and cost_[1] correspond to the reversed graph.
    // Graphs are stored as vectors of adjacency lists corresponding
    // to nodes.
    // Adjacency list itself is stored in adj_, and the corresponding
    // edge costs are stored in cost_.
    Adj adj_;
    Adj cost_;
    // distance_[0] stores distances for the forward search,
    // and distance_[1] stores distances for the backward search.
    vector<vector<Len>> distance_;
    // Stores all the nodes visited either by forward or backward search.
    vector<int> workset_;
    // Stores a flag for each node which is True iff the node was visited
    // either by forward or backward search.
    vector<vector<bool>> visited_;

  public:
    Bidijkstra(int n, Adj adj, Adj cost)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<Len>(n, VERY_LARGE)), visited_(2, vector<bool>(n, false))
    {
        workset_.reserve(n);
    }

    // Initialize the data structures before new query,
    // clear the changes made by the previous query.
    void clear()
    {
        for (int i = 0; i < workset_.size(); ++i)
        {
            int v = workset_[i];
            distance_[0][v] = distance_[1][v] = VERY_LARGE;
            visited_[0][v] = false;
            visited_[1][v] = false;
        }
        workset_.clear();
    }

    // Processes visit of either forward or backward search
    // (determined by value of side), to node v trying to
    // relax the current distance by dist.
    void visit(Queue &q, int side, int v, Len dist)
    {
        //Check if this one is an old version of the node in the queue
        if (dist > distance_[side][v])
            return;

        //Relax the distances
        for (int i = 0; i < adj_[side][v].size(); i++)
        {
            int u = adj_[side][v][i];

            if (distance_[side][u] > dist + cost_[side][v][i])
            {
                workset_.push_back(u);
                distance_[side][u] = dist + cost_[side][v][i];
                q[side].push(make_pair(distance_[side][u], u));
            }
        }
        //Add v to workset_ and change its flag in visited_
        workset_.push_back(v);
        visited_[side][v] = true;
    }

    // Returns the distance from s to t in the graph.
    Len query(int s, int t)
    {

        clear();
        Queue q(2);

        distance_[0][s] = 0;
        q[0].push(make_pair(0, s));

        distance_[1][t] = 0;
        q[1].push(make_pair(0, t));

        while (!q[0].empty() || !q[1].empty())
        {
            if (!q[0].empty())
            {
                //Forward Dijkstra
                pair<Len, int> node_forward = q[0].top();
                q[0].pop();
                int v_f = node_forward.second;
                Len dist_f = node_forward.first;
                //Check if v_f is visited by backward
                if (visited_[1][v_f])
                {
                    return shortest_path(s, t);
                }
                // Visit v_f if not
                visit(q, 0, v_f, dist_f);
            }

            //Repeat symetrically for backward
            if (!q[1].empty())
            {
                pair<Len, int> node_backward = q[1].top();
                q[1].pop();
                int v_b = node_backward.second;
                Len dist_b = node_backward.first;
                //Check if v_b is visited by forward
                if (visited_[0][v_b])
                {
                    return shortest_path(s, t); //shortest_path;
                }
                // Visit v_b if not
                visit(q, 1, v_b, dist_b);
            }
        }

        return -1;
    }

    Len shortest_path(int s, int t)
    {
        Len distance = VERY_LARGE;
        for (int i = 0; i < workset_.size(); i++)
        {
            int u = workset_[i];
            if (distance > (distance_[0][u] + distance_[1][u]))
            {
                distance = distance_[0][u] + distance_[1][u];
            }
        }
        return distance;
    }
};

int main()
{
    int n, m;
    scanf("%d%d", &n, &m);
    Adj adj(2, vector<vector<int>>(n));
    Adj cost(2, vector<vector<int>>(n));
    for (int i = 0; i < m; ++i)
    {
        int u, v, c;
        scanf("%d%d%d", &u, &v, &c);
        adj[0][u - 1].push_back(v - 1);
        cost[0][u - 1].push_back(c);
        adj[1][v - 1].push_back(u - 1);
        cost[1][v - 1].push_back(c);
    }

    Bidijkstra bidij(n, adj, cost);

    int t;
    scanf("%d", &t);
    for (int i = 0; i < t; ++i)
    {
        int u, v;
        scanf("%d%d", &u, &v);
        printf("%lld\n", bidij.query(u - 1, v - 1));
    }
}
