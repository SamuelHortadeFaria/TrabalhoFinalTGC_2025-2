// Image.h
#ifndef IMAGE_H
#define IMAGE_H

#include <vector>

struct Pixel {
    unsigned char r, g, b;
};

class Image {
public:
    int width;
    int height;
    std::vector<Pixel> data;

    Image();
    Image(int w, int h);

    Pixel& at(int x, int y);
    const Pixel& at(int x, int y) const;
};

#endif
