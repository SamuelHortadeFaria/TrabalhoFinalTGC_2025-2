// Segmenter.h
#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <vector>

#include "Edge.h"
#include "Image.h"

std::vector<int> segment_by_mst(int n,
    const std::vector<Edge>& mst_edges,
    int num_segments);

Image labels_to_color_image(int width,
    int height,
    const std::vector<int>& labels);

#endif
