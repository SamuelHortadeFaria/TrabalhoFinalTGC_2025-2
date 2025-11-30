// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

#include "Image.h"

Image loadImage(const std::string& filename, std::vector<double>& intensities);
bool saveImagePNG(const Image& img, const std::string& filename);

Image applyGaussianBlur(const Image& img, double sigma);

#endif
