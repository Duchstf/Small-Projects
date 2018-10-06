#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

using std::make_pair;
using std::pair;
using std::vector;

/*
In this problem, the goal is to build roads between some pairs of the given cities 
such that there is a path between any two cities and the total 
length of the roads is minimized.
*/

//Re use the data structure in Dijkstra's algorithm
class minQueue
{
    vector<pair<int, double>> queue;

  public:
    void add_with_priority(int v, double dist)
    {
        queue.push_back(make_pair(v, dist)); // Consider modifying this if this data structure is used on another algorithm
    }

    //Comparison function to specify in heap
    struct Comp
    {
        bool operator()(const pair<int, double> &left, const pair<int, double> &right)
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

    void decrease_priority(int v, double dist)
    {
        for (int i = 0; i < queue.size(); i++)
        {
            if (v == queue[i].first)
                queue[i].second = dist; //O(n)
        }
        std::make_heap(queue.begin(), queue.end(), Comp()); // Reorder priority
    }

    //Add new find function
    bool Find(int z, double dist)
    {
        pair<int, double> obj = make_pair(z, dist);
        if (std::find(queue.begin(), queue.end(), obj) != queue.end())
            return true;
        else
            return false;
    }

    bool empty()
    {
        if (queue.size() > 0)
            return false;
        return true;
    }
};

double compute_direct_distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));
}

double minimum_distance(vector<int> x, vector<int> y)
{
    double result = 0.;
    int V = x.size();                                         // Number of vertex
    double lim = std::numeric_limits<double>::max() - 99999.; //potential error, modify if common distance used is larger than this
    vector<double> cost(V, lim);

    cost[0] = 0; //You can pick any vertex, but I just choose the first one for simplicity

    minQueue Q;

    for (int i = 0; i < V; i++)
    {
        Q.add_with_priority(i, cost[i]);
    }

    Q.turn_to_heap();

    while (!Q.empty())
    {
        int v = Q.extract_min();
        result += cost[v];
        for (int i = 0; i < V; i++)
        {
            double distance = compute_direct_distance(x[v], y[v], x[i], y[i]);
            if (Q.Find(i, cost[i]) && cost[i] > distance)
            {
                cost[i] = distance;
                Q.decrease_priority(i, cost[i]);
            }
        }
    }
    return result;
}

int main()
{
    size_t n;
    std::cin >> n;
    vector<int> x(n), y(n);
    for (size_t i = 0; i < n; i++)
    {
        std::cin >> x[i] >> y[i];
    }
    std::cout << std::setprecision(10) << minimum_distance(x, y) << std::endl;
}
