// GraphBuilder.cpp
#include <cmath>

#include "GraphBuilder.h"

static double rgb_distance(const Pixel& a, const Pixel& b) {
    double dr = double(a.r) - double(b.r);
    double dg = double(a.g) - double(b.g);
    double db = double(a.b) - double(b.b);
    return std::sqrt(dr * dr + dg * dg + db * db);
}

std::vector<Edge> buildGraph(const Image& img) {
    std::vector<Edge> edges;
    int W = img.width;
    int H = img.height;

    auto idx = [W](int x, int y) { return y * W + x; };

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            int u = idx(x, y);

            // direita
            if (x + 1 < W) {
                int v = idx(x + 1, y);
                double w = rgb_distance(img.at(x, y), img.at(x + 1, y));
                edges.emplace_back(u, v, w);
            }
            // baixo
            if (y + 1 < H) {
                int v = idx(x, y + 1);
                double w = rgb_distance(img.at(x, y), img.at(x, y + 1));
                edges.emplace_back(u, v, w);
            }
            // diagonal baixo-direita
            if (x + 1 < W && y + 1 < H) {
                int v = idx(x + 1, y + 1);
                double w = rgb_distance(img.at(x, y), img.at(x + 1, y + 1));
                edges.emplace_back(u, v, w);
            }
            // diagonal baixo-esquerda
            if (x - 1 >= 0 && y + 1 < H) {
                int v = idx(x - 1, y + 1);
                double w = rgb_distance(img.at(x, y), img.at(x - 1, y + 1));
                edges.emplace_back(u, v, w);
            }
        }
    }

    return edges;
}

std::vector<Edge> buildDirectedGraph(const Image& img) {
    auto undirected = buildGraph(img);

    std::vector<Edge> directed;
    directed.reserve(undirected.size() * 2);

    for (const auto& e : undirected) {
        directed.push_back({ e.u, e.v, e.w });
        directed.push_back({ e.v, e.u, e.w });
    }
    return directed;
}
