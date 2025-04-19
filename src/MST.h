//
// Created by LAPTOP on 4/18/2025.
//

#ifndef MST_H
#define MST_H

#include <bits/stdc++.h>
using namespace std;
using Matrix = vector<vector<int>>;

//prim’s MST on adjacency matrix
//returns an adjacency list of the MST
vector<vector<int>> primMST(const Matrix& mat, int start) {
    int k = mat.size();
    vector<vector<int>> k;

    for(int i = 0; i < k^2; i++)
    {

    }
    //bild adjacency list for MST
    
    return mst;
}

//union‑Find for Kruskal’s ---
struct DSU {
    vector<int> p, r;
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
        if (r[a] < r[b]) swap(a,b);
        p[b] = a;
        if (r[a] == r[b]) r[a]++;
        return true;
    }
};

//kruskal’s MST on adjacency matrix
vector<vector<int>> kruskalMST(const Matrix& mat) {
    int k = mat.size();

    vector<vector<int>> mst(k);

    return mst;
}

//pre‑order DFS to produce a visit order
void dfsOrder(int u, int p, const vector<vector<int>>& mst, vector<int>& order) {
    order.push_back(u);
    for (int v : mst[u]) {
        if (v == p) continue;
        dfsOrder(v, u, mst, order);
    }
}


#endif //MST_H
