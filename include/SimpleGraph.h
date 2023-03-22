/**
*  SimpleGraph.h - 简单图，用于测试
*/
#ifndef __SIMPLE_GRAPH_H__
#define __SIMPLE_GRAPH_H__

#include "base.h"

class SimpleGraph : public Graph {
public:
    SimpleGraph() {
        // Add vertices
        auto A = add_vertex(*this);
        auto B = add_vertex(*this);
        auto C = add_vertex(*this);
        auto D = add_vertex(*this);
        auto E = add_vertex(*this);
        auto F = add_vertex(*this);
        auto G = add_vertex(*this);

        // Add edges
        add_edge(A, B, *this); // e1
        add_edge(B, C, *this); // e2
        add_edge(C, D, *this); // e3
        add_edge(D, A, *this); // e4
        add_edge(D, B, *this); // e5
        add_edge(E, F, *this); // e6
        add_edge(F, G, *this); // e7
        add_edge(G, E, *this); // e8
    };
};

#endif // __SIMPLE_GRAPH_H__