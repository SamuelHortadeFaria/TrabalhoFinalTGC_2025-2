// Edge.h
#ifndef EDGE_H
#define EDGE_H

struct Edge {
    int u;
    int v;
    double w;

    Edge(int uu = 0, int vv = 0, double ww = 0.0)
        : u(uu), v(vv), w(ww) {
    }
};

#endif
