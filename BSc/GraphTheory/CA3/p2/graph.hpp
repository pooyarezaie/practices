#ifndef __GRAPH_HPP
#define __GRAPH_HPP

#include "polynomial.hpp"
#include <vector>

struct Edge{
    int u, v;
    Edge(int, int);
};

class Graph{
private:

    int _size;
    int _order;
    int _iorder;
    std::vector<std::vector<bool> > adj_matrix;

    Edge select_edge();
    Edge select_edge_triv();

    Polynomial base_result();
    Polynomial chromatic_polynomial_rec();

public:

    Graph(std::vector<std::vector<bool> > matrix);
    Polynomial chromatic_polynomial();
    std::vector<Graph> connected_components();
};

#endif
