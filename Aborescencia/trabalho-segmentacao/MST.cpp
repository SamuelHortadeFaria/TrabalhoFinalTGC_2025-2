// mst.cpp
#include <algorithm>

#include "MST.h"

struct DSU {
    std::vector<int> parent, rnk;

    DSU(int n = 0) { init(n); }

    void init(int n) {
        parent.resize(n);
        rnk.assign(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find_set(int v) {
        if (v == parent[v]) return v;
        return parent[v] = find_set(parent[v]);
    }

    bool union_set(int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a == b) return false;
        if (rnk[a] < rnk[b]) std::swap(a, b);
        parent[b] = a;
        if (rnk[a] == rnk[b]) rnk[a]++;
        return true;
    }
};

std::pair<double, std::vector<Edge>> kruskal_mst(int n, const std::vector<Edge>& edges_in) {
    std::vector<Edge> edges = edges_in;
    std::sort(edges.begin(), edges.end(),
        [](const Edge& a, const Edge& b) { return a.w < b.w; });

    DSU dsu(n);
    double total = 0.0;
    std::vector<Edge> mst;
    mst.reserve(n - 1);

    for (const auto& e : edges) {
        if (dsu.union_set(e.u, e.v)) {
            total += e.w;
            mst.push_back(e);
            if ((int)mst.size() == n - 1) break;
        }
    }

    return { total, mst };
}
