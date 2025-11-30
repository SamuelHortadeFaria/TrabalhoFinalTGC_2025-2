#include <cmath>
#include <random>

#include "graph.h"

float distance(Pixel3f a, Pixel3f b)
{
    float dr = a.r - b.r;
    float dg = a.g - b.g;
    float db = a.b - b.b;
    return std::sqrtf(dr*dr + dg*dg + db*db);
}

std::vector<Edge> BuildGraph(const Array2D<Pixel3f>& img)
{
    std::vector<Edge> graph;

    int n_edges = 2*img.width*img.height - img.width - img.height;
    graph.reserve(n_edges);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {

            int u = img.idx(x, y);

            if (x + 1 < img.width) {
                int v = img.idx(x + 1, y);

                Pixel3f u_rgb = img(u);
                Pixel3f v_rgb = img(v);

                float w = distance(u_rgb, v_rgb);

                graph.push_back({u, v, w});
            }

            if (y + 1 < img.height) {
                int v = img.idx(x, y + 1);

                Pixel3f u_rgb = img(u);
                Pixel3f v_rgb = img(v);

                float w = distance(u_rgb, v_rgb);

                graph.push_back({u, v, w});
            }
        }
    }

    return graph;
}

Array2D<Pixel3b> LabelsToColorImage(const Array2D<int>& labels)
{

    Array2D<Pixel3b> img(labels.width, labels.height);

    // Descobre o maior label
    int n = labels.width * labels.height;
    int max_label = 0;
    for (int i = 0; i < n; i++) {
        int v = labels(i);
        if (v > max_label) max_label = v;
    }
    int num_labels = max_label + 1;

    // Gera uma cor para cada label
    std::vector<Pixel3b> palette(num_labels);
    std::mt19937 rng(42); // semente fixa
    std::uniform_int_distribution<int> dist(0, 255);

    for (int i = 0; i < num_labels; ++i) {
        palette[i] = Pixel3b{
            (unsigned char)dist(rng),
            (unsigned char)dist(rng),
            (unsigned char)dist(rng)
        };
    }

    // Atribui cor a cada pixel
    for (int i = 0; i < n; ++i) {
        int lbl = labels(i);
        if (lbl < 0 || lbl >= num_labels) {
            // so por seguranca, nao deveria acontecer
            img(i) = Pixel3b{};
        }
        else {
            img(i) = palette[lbl];
        }
    }

    return img;
}

