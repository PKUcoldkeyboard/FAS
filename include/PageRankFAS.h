/**
 * PageRankFAS.h - PageRankFAS Solver
 * This class implements a FASStrategy for solving the feedback arc set problem using PageRank algorithm.
 */
#ifndef __PAGERANK_FAS_H__
#define __PAGERANK_FAS_H__

#include "strategy.h"

class PageRankFAS: public FASStrategy {
public:
    boost::container::vector<EdgePair> getFeedbackArcSet(Graph &g) override;
private:
    void getLineGraph(const Graph &g, LineGraph &lineGraph, const Vertex &v, const LineVertex &prev,
                      boost::container::vector<bool> &visited, const AdjacentEdgesMap &adj_edge_map,
                      OutDegreeMap &out_degree_map);

    void computePageRank(const LineGraph &lineGraph, boost::container::vector<float> &pagerank, const OutDegreeMap &out_degree_map);
    void computeStronglyConnectedComponents(const Graph &g, boost::container::vector<VertexHashSet> &sccs);
    bool isCyclic(const Graph &g);
};

#endif // __PAGERANK_H__
