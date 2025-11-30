#pragma once

#include <vector>

#include "array2d.h"

struct Edge {
    int u;
    int v;
    float w;

    bool operator<(const Edge& other) const { return w < other.w; }
};

struct Pixel3b {
    unsigned char r, g, b;
};

struct Pixel3f {
    float r, g, b;
};

struct DSU {
    std::vector<int> parent, size;

    DSU(int n) : parent(n), size(n, 1)
    {
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int Find(int i)
    {
        if (parent[i] != i)
            parent[i] = Find(parent[i]);
        return parent[i];
    }

    int Unite(int u, int v)
    {
        int ru = Find(u);
        int rv = Find(v);
        if (ru != rv) {
            parent[rv] = ru;
            size[ru] += size[rv];
        }

        return ru;
    }
};

std::vector<Edge> BuildGraph(const Array2D<Pixel3f>& img);
Array2D<Pixel3b> LabelsToColorImage(const Array2D<int>& labels);
