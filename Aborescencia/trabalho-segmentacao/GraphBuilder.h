// GraphBuilder.h
#ifndef GRAPH_BUILDER_H
#define GRAPH_BUILDER_H

#include <vector>

#include "Image.h"
#include "Edge.h"

std::vector<Edge> buildGraph(const Image& img);

std::vector<Edge> buildDirectedGraph(const Image& img);

#endif
