#include <iostream>
#include <charconv>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "array2d.h"
#include "graph.h"
#include "felzenszwalb.h"

/*
 * No artigo original do Felzenszwalb ele calcula a distância euclidiana
 * entre cores sRGB entre 0 e 255, talvez seja uma boa ideia normalizar
 * e converter para RGB linear...
 */
Array2D<Pixel3f> LoadFile(char *file_name)
{
    // n_channels não será utilizado
    int width, height, n_channels;
    unsigned char *img_data = stbi_load(file_name, &width, &height, &n_channels, 3);

    if (img_data == nullptr) {
        std::cerr << "Erro ao carregar imagem.\n";
	std::exit(1);
    }

    Array2D<Pixel3f> img(width, height);

    for (int i = 0; i < width*height; i++) {
        int orig = i*3;
        Pixel3f rgb = {
            float(img_data[orig+0]),
            float(img_data[orig+1]),
            float(img_data[orig+2])
        };

        img(i) = rgb;
    }

    return img;
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0]
            << " entrada.png [numero k]\n";
        return 1;
    }

    char saida[] = "saida.png";
    char *entrada = argv[1];
    float sigma = .8f; // recomendado pelo artigo
    float k = strtof(argv[2], nullptr);

    // Falha na conversão
    if (k <= 0.f || k == HUGE_VALF)
	    k = 300.f;

    std::cout << "k = " << k << ", sigma = " << sigma << '\n';

    Array2D<Pixel3f> img = LoadFile(entrada);

    Array2D<int> labels = SegmentFelzenszwalb(img, k, sigma);

    Array2D<Pixel3b> seg = LabelsToColorImage(labels);

    if (!stbi_write_png(saida, seg.width, seg.height, 3, (unsigned char *)seg.data, seg.width*3)) {
        std::cerr << "Falha ao salvar imagem de saida.\n";
        return 1;
    }

    std::cout << "Fim OK.\n";
    
    return 0;
}
