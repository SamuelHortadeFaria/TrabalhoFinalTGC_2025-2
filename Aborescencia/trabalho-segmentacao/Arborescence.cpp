// Arborescence.cpp
#include <vector>
#include <queue>
#include <iostream>

#include "Arborescence.h"

using namespace std;

pair<double, vector<Edge>>
arborescence_from_mst(int n, const vector<Edge>& mst_edges, int root) {
    vector<vector<pair<int, double>>> adj(n);
    double total_cost = 0.0;

    for (size_t i = 0; i < mst_edges.size(); ++i) {
        const Edge& e = mst_edges[i];
        adj[e.u].push_back(make_pair(e.v, e.w));
        adj[e.v].push_back(make_pair(e.u, e.w));
        total_cost += e.w;
    }

    vector<int> parent(n, -1);
    vector<double> parent_w(n, 0.0);
    queue<int> q;

    parent[root] = root;
    q.push(root);

    while (!q.empty()) {
        int u = q.front(); q.pop();

        for (size_t j = 0; j < adj[u].size(); ++j) {
            int v = adj[u][j].first;
            double w = adj[u][j].second;

            if (parent[v] == -1) {
                parent[v] = u;
                parent_w[v] = w;
                q.push(v);
            }
        }
    }

    for (int v = 0; v < n; ++v) {
        if (parent[v] == -1) {
            cerr << "[ARBO] MST desconexa, arborescencia incompleta.\n";
            return make_pair(total_cost, vector<Edge>());
        }
    }

    vector<Edge> arb_edges;
    arb_edges.reserve(n - 1);

    for (int v = 0; v < n; ++v) {
        if (v == root) continue;
        arb_edges.push_back(Edge(parent[v], v, parent_w[v]));
    }

    return make_pair(total_cost, arb_edges);
}
