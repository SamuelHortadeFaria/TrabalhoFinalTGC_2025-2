// MST.h
#ifndef MST_H
#define MST_H

#include <vector>
#include <utility>

#include "Edge.h"

std::pair<double, std::vector<Edge>> kruskal_mst(int n, const std::vector<Edge>& edges);

#endif
