//
// Created by LAPTOP on 4/18/2025.
//

#ifndef MST_H
#define MST_H

#include <bits/stdc++.h>
//using namespace std;
using Matrix = std::vector<std::vector<int>>;

//prim’s MST on adjacency matrix
//returns an adjacency list of the MST
std::vector<std::vector<int>> primMST(const Matrix& mat, int start) {
    int k = mat.size();

    //bild adjacency list for MST
    std::vector<std::vector<int>> mst(k);
    return mst;
}

//union‑Find for Kruskal’s ---
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

//kruskal’s MST on adjacency matrix
std::vector<std::vector<int>> kruskalMST(const Matrix& mat) {
    int k = mat.size();

    std::vector<std::vector<int>> mst(k);

    return mst;
}

//pre‑order DFS to produce a visit order
void dfsOrder(int u, int p, const std::vector<std::vector<int>>& mst, std::vector<int>& order) {
    order.push_back(u);
    for (int v : mst[u]) {
        if (v == p) continue;
        dfsOrder(v, u, mst, order);
    }
}


#endif //MST_H
