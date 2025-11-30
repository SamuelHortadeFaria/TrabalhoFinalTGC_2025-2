// Segmenter.cpp
#include <algorithm>
#include <unordered_map>
#include <random>
#include <iostream>

#include "Segmenter.h"

struct DSU2 {
    std::vector<int> parent, rnk;

    DSU2(int n = 0) { init(n); }

    void init(int n) {
        parent.resize(n);
        rnk.assign(n, 0);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find_set(int v) {
        if (v == parent[v]) return v;
        return parent[v] = find_set(parent[v]);
    }

    void union_set(int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a == b) return;
        if (rnk[a] < rnk[b]) std::swap(a, b);
        parent[b] = a;
        if (rnk[a] == rnk[b]) rnk[a]++;
    }
};

// Corta a MST nas (num_segments-1) maiores arestas
std::vector<int> segment_by_mst(int n,
    const std::vector<Edge>& mst_edges,
    int num_segments) {
    if (num_segments < 1) num_segments = 1;
    if (num_segments > n) num_segments = n;

    // Copia arestas e ordena por peso DECRESCENTE
    std::vector<Edge> edges = mst_edges;
    std::sort(edges.begin(), edges.end(),
        [](const Edge& a, const Edge& b) {
            return a.w > b.w;
        });

    DSU2 dsu(n);

    for (int i = num_segments - 1; i < (int)edges.size(); ++i) {
        dsu.union_set(edges[i].u, edges[i].v);
    }

    std::vector<int> labels(n);
    for (int v = 0; v < n; ++v) {
        labels[v] = dsu.find_set(v);
    }

    std::unordered_map<int, int> mapLabel;
    mapLabel.reserve(n * 2);
    int nextId = 0;

    for (int& l : labels) {
        auto it = mapLabel.find(l);
        if (it == mapLabel.end()) {
            mapLabel[l] = nextId;
            l = nextId;
            nextId++;
        }
        else {
            l = it->second;
        }
    }

    std::cerr << "Numero de segmentos obtidos (compactados): "
        << nextId << "\n";

    return labels;
}

// Converte labels em uma imagem RGB pseudo-aleatoria
Image labels_to_color_image(int width,
    int height,
    const std::vector<int>& labels) {
    int n = width * height;
    Image img(width, height);

    if ((int)labels.size() != n) {
        std::cerr << "labels_to_color_image: tamanho de labels != pixels\n";
        return img;
    }

    // Descobre o maior label
    int max_label = 0;
    for (int v : labels) {
        if (v > max_label) max_label = v;
    }
    int num_labels = max_label + 1;

    std::vector<Pixel> palette(num_labels);
    std::mt19937 rng(42); // semente fixa
    std::uniform_int_distribution<int> dist(0, 255);

    for (int i = 0; i < num_labels; ++i) {
        palette[i] = Pixel{
            (unsigned char)dist(rng),
            (unsigned char)dist(rng),
            (unsigned char)dist(rng)
        };
    }

    // Atribui cor a cada pixel
    for (int i = 0; i < n; ++i) {
        int lbl = labels[i];
        if (lbl < 0 || lbl >= num_labels) {
            // somente por seguranca, nao deveria acontecer
            img.data[i] = Pixel{ 0, 0, 0 };
        }
        else {
            img.data[i] = palette[lbl];
        }
    }

    return img;
}
