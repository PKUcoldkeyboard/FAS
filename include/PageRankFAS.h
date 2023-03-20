/**
 * PageRankFAS.h - PageRankFAS Solver
*/
#ifndef __PAGERANK_FAS_H__
#define __PAGERANK_FAS_H__

#include "Strategy.h"

class PageRankFAS: public FASStrategy {
public:
    std::vector<int> getFAS(Graph &g) override;
private:
    Graph* getLineGraph(Graph &g);
    std::vector<double> calculatePageRank(Graph &lingGraph);
    std::vector<int> PageRankFAS::calculateStrongComponents(Graph &g)
};

#endif // __PAGERANK_H__