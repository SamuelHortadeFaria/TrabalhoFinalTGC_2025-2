#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <vector>
#include <algorithm>

#include "utils.h"
#include "stb_image.h"
#include "stb_image_write.h"

Image loadImage(const std::string& filename, std::vector<double>& intensities) {
    int w, h, ch;
    // for�a RGB (3 canais)
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &ch, 3);

    if (!data) {
        std::cerr << "Erro ao carregar imagem: " << filename << "\n";
        return Image(0, 0);
    }

    Image img(w, h);
    intensities.resize(w * h);

    for (int i = 0; i < w * h; ++i) {
        unsigned char r = data[3 * i + 0];
        unsigned char g = data[3 * i + 1];
        unsigned char b = data[3 * i + 2];

        img.data[i] = { r, g, b };

        double gray = 0.299 * r + 0.587 * g + 0.114 * b;
        intensities[i] = gray;
    }

    stbi_image_free(data);
    return img;
}

bool saveImagePNG(const Image& img, const std::string& filename) {
    if (img.width <= 0 || img.height <= 0 || img.data.empty()) {
        std::cerr << "Imagem invalida, nao foi possivel salvar.\n";
        return false;
    }

    int n = img.width * img.height;
    std::vector<unsigned char> buffer(n * 3);

    for (int i = 0; i < n; ++i) {
        buffer[3 * i + 0] = img.data[i].r;
        buffer[3 * i + 1] = img.data[i].g;
        buffer[3 * i + 2] = img.data[i].b;
    }

    int stride = img.width * 3;
    if (!stbi_write_png(filename.c_str(), img.width, img.height, 3,
        buffer.data(), stride)) {
        std::cerr << "Erro ao salvar PNG em: " << filename << "\n";
        return false;
    }

    std::cout << "Imagem salva em: " << filename << "\n";
    return true;
}

Image applyGaussianBlur(const Image& img, double sigma) {
    if (sigma <= 0.0) return img; // Sem blur se sigma for invalido

    int W = img.width;
    int H = img.height;
    Image temp(W, H);
    Image dst(W, H);

    //Criar o Kernel Gaussiano 1D
    int radius = (int)std::ceil(3.0 * sigma);
    int kernelSize = 2 * radius + 1;
    std::vector<double> kernel(kernelSize);

    double sum = 0.0;
    double s2 = 2.0 * sigma * sigma;
    for (int i = -radius; i <= radius; ++i) {
        double val = std::exp(-(i * i) / s2);
        kernel[i + radius] = val;
        sum += val;
    }
    
    //Normalizar o kernel para que a soma seja 1.0
    for (double& v : kernel) v /= sum;

    //Passo Horizontal
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            double r = 0, g = 0, b = 0;
            for (int k = -radius; k <= radius; ++k) {
                
                int nx = std::clamp(x + k, 0, W - 1);
                
                const Pixel& p = img.at(nx, y);
                double w = kernel[k + radius];
                
                r += p.r * w;
                g += p.g * w;
                b += p.b * w;
            }
            temp.at(x, y) = { (unsigned char)r, (unsigned char)g, (unsigned char)b };
        }
    }

    //Passo Vertical
    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            double r = 0, g = 0, b = 0;
            for (int k = -radius; k <= radius; ++k) {
                int ny = std::clamp(y + k, 0, H - 1);
                
                const Pixel& p = temp.at(x, ny);
                double w = kernel[k + radius];
                
                r += p.r * w;
                g += p.g * w;
                b += p.b * w;
            }
            dst.at(x, y) = { (unsigned char)r, (unsigned char)g, (unsigned char)b };
        }
    }

    return dst;
}
