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
<<<<<<< HEAD
    std::vector<std::vector<int>> getConnectedComponents(Graph &g);
    bool isCyclic(Graph &g);
=======
    std::vector<std::vector<int>> getConnectedComponents(Graph &g);
    bool isCyclic(Graph &g);
>>>>>>> 63eae86c18a96c0dd608f9f35a91430bb212ac33
};

#endif // __PAGERANK_H__
