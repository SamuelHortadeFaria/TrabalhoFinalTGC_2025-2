// Arborescence.h
#ifndef ARBORESCENCE_H
#define ARBORESCENCE_H

#include <vector>
#include <utility>
#include "Edge.h"

std::pair<double, std::vector<Edge>>
arborescence_from_mst(int n, const std::vector<Edge>& mst_edges, int root);

#endif
