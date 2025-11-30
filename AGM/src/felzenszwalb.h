#pragma once

#include "array2d.h"
#include "graph.h"

Array2D<int> SegmentFelzenszwalb(const Array2D<Pixel3f>& img, float k, float sigma);
