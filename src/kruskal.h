#include <vector>
#include <iostream>
#include <functional>
#include <numeric>

#ifndef KRUSKAL_H
#define KRUSKAL_H

class kruskal {
  private:
    int graph_weight = 0;  // holds the total weight of the mst

    struct DSU {
        std::vector<int> p, r;

        DSU(int n) : p(n), r(n, 0)
        {
            iota(p.begin(), p.end(), 0);
        }
        int find(int x)
        {
            return p[x] == x ? x : p[x] = find(p[x]);
        }

        bool unite(int a, int b) {
            a = find(a); b = find(b);
            if (a == b) return false;
            if (r[a] < r[b]) std::swap(a,b);
            p[b] = a;
            if (r[a] == r[b]) r[a]++;
            return true;
        }
    };

  public:
    // returns the total weight of mst
    int getWeight()
    {
        return graph_weight;
    }

    // kruskal’s MST on adjacency matrix
    std::vector<std::vector<int>> runKruskalAlgorithm(const std::vector<std::vector<int>>& distances, int startRide) {
        int n = static_cast<int>(distances.size());
        DSU dsu(n);
        std::vector<std::tuple<int, int, int>> edges; // (from, to, weight)

        // Collect all edges from the distance matrix
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (distances[i][j] > 0) {
                    edges.emplace_back(i, j, distances[i][j]);
                }
            }
        }

        // sort the edges by weight (walking distance)
        std::sort(edges.begin(), edges.end(),
            [](const auto& a, const auto& b) {
                return std::get<2>(a) < std::get<2>(b);
            });

        // build MST using Kruskal's algorithm
        std::vector<std::vector<int>> mst(n);
        for (const auto& [u, v, weight] : edges) {
            if (dsu.unite(u, v)) {
                mst[u].push_back(v);
                mst[v].push_back(u);
                graph_weight += weight;
            }
        }

        return mst;
    }

    // call: auto order = getMSTTraversalOrder(mst, startRide);
    std::vector<int> getMSTTraversalOrder(const std::vector<std::vector<int>>& adj, int start)
    {
        std::vector<bool> vis(adj.size(), false);
        std::vector<int> order;
        dfsOrder(start, vis, adj, order);
        return order;
    }


    // runs a dfs traversal on the mst
    void dfsOrder(int u, std::vector<bool>& visited, const std::vector<std::vector<int>>& mst, std::vector<int>& order) {
        // set the current node to visited
        visited[u] = true;

        // add current node to path order
        order.push_back(u);

        // visit all unvisited neighbors
        for (int v : mst[u]) {
            if (!visited[v]) {
                dfsOrder(v, visited, mst, order);
            }
        }
    }
};

#endif //KRUSKAL_H