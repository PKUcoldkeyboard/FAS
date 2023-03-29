/**
 * PageRankFAS.h - PageRankFAS Solver
*/
#ifndef __PAGERANK_FAS_H__
#define __PAGERANK_FAS_H__

#include "strategy.h"

class PageRankFAS: public FASStrategy {
public:
    std::vector<Edge> getFAS(Graph &g) override;
private:
    Graph* getLineGraph(Graph &g);
    std::vector<double> calculatePageRank(Graph &lingGraph);
    std::vector<std::vector<int>> connected_components(Graph &g);
    bool has_circle(Graph &g);
};

#endif // __PAGERANK_H__
