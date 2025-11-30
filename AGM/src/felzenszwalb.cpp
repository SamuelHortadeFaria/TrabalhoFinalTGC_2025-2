#include <algorithm>
#include <cmath>
#include <vector>

#include "felzenszwalb.h"

// Dividido em um kernel horizontal e um vertical
Array2D<Pixel3f> GaussianBlur(Array2D<Pixel3f> img, float sigma)
{
    Array2D<Pixel3f> dst(img.width, img.height);

    int radius = int(std::ceilf(3.f * sigma));
    int kernel_size = 2 * radius + 1;

    std::vector<float> kernel(kernel_size);

    float sum = 0.f;
    float s2 = 2.f * sigma * sigma;

    for (int i = -radius; i <= radius; i++) {
        float v = std::expf(-(i * i) / s2);
        kernel[i + radius] = v;
        sum += v;
    }

    for (int i = 0; i < kernel_size; i++)
        kernel[i] /= sum;

    Array2D<Pixel3f> tmp(img.width, img.height);

    // Horizontal
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Pixel3f acc = {0};

            for (int k = -radius; k <= radius; k++) {
                int xx = std::clamp(x + k, 0, img.width - 1);
                Pixel3f rgb = img(xx, y);
                acc.r += rgb.r * kernel[k + radius];
                acc.g += rgb.g * kernel[k + radius];
                acc.b += rgb.b * kernel[k + radius];
            }

            tmp(x, y) = acc;
        }
    }

    // Vertical
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Pixel3f acc = {0};

            for (int k = -radius; k <= radius; k++) {
                int yy = std::clamp(y + k, 0, img.height - 1);
                Pixel3f rgb = tmp(x, yy);
                acc.r += rgb.r * kernel[k + radius];
                acc.g += rgb.g * kernel[k + radius];
                acc.b += rgb.b * kernel[k + radius];
            }

            dst(x, y) = acc;
        }
    }

    return dst;
}

// https://cs.brown.edu/people/pfelzens/papers/seg-ijcv.pdf
// sigma é o desvio padrão da função gaussiana usada no blur.
// k controla o tamanho dos segmentos, quanto maior, maior os segmento.
Array2D<int> SegmentFelzenszwalb(const Array2D<Pixel3f>& img, float k, float sigma)
{

    // O resultados ficam melhores quando a imagem está borrada
    Array2D<Pixel3f> blurred = GaussianBlur(img, sigma);

    std::vector<Edge> graph = BuildGraph(blurred);

    /*
     * O algoritmo essencialmente se trata de uma versão modificada do
     * algoritmo de Kruskal, mas componentes com muita diferença interna
     * não são unidos.
     */
    std::sort(graph.begin(), graph.end());

    int n = img.width*img.height;

    DSU dsu(n);

    /* 
     * O maior peso de cada componente, no início todos os componentes são
     * nulos, então zero.
     */
    std::vector<float> diff(n, 0);

    for (const Edge& e : graph) {
        int ru = dsu.Find(e.u);
        int rv = dsu.Find(e.v);

        if (ru != rv) {

            // A heurística adicional
            float threshold_u = diff[ru] + k / dsu.size[ru];
            float threshold_v = diff[rv] + k / dsu.size[rv];

            if (e.w <= std::min(threshold_u, threshold_v)) {
                int root = dsu.Unite(ru, rv);
                diff[root] = std::max({e.w, diff[ru], diff[rv]});
            }
        }
    }

    // Talvez remover labels não utilizados?
    Array2D<int> labels(img.width, img.height);
    for (int i = 0; i < n; ++i)
        labels(i) = dsu.Find(i);

    return labels;
}
