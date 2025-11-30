// Image.cpp
#include "Image.h"

Image::Image() : width(0), height(0) {}

Image::Image(int w, int h)
    : width(w), height(h), data(w* h) {
}

Pixel& Image::at(int x, int y) {
    return data[y * width + x];
}

const Pixel& Image::at(int x, int y) const {
    return data[y * width + x];
}
