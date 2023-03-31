/**
 * PageRankFAS.h - PageRankFAS Solver
 * This class implements a FASStrategy for solving the feedback arc set problem using PageRank algorithm.
 */
#ifndef __PAGERANK_FAS_H__
#define __PAGERANK_FAS_H__

#include "strategy.h"

class PageRankFAS: public FASStrategy {
public:
    std::vector<EdgePair> getFeedbackArcSet(Graph &g) override;
private:
    IdToEdgeMap id_to_edge_map;
    EdgeToVertexMap edge_to_vertex_map;
    void getLineGraph(const Graph &g, Graph& lineGraph);
    void getLineGraph(const Graph &g, Graph &lineGraph, Vertex v, Vertex prev, std::vector<bool> &visited);
    void calculatePageRank(const Graph &lineGraph, std::vector<double> &pagerank);
    bool isCyclic(Graph &g);
    void getConnectedComponents(Graph &g, std::vector<Graph> &sccs);
};

#endif // __PAGERANK_H__
