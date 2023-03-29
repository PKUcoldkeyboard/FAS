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
    /**
     * @brief getLineGraph
     * @param g the graph
     * @return the line graph
    */
    Graph* getLineGraph(Graph &g);
    /**
     * @brief calculatePageRank for each edge
     * @param lineGraph the line graph
     * @return the page rank value for each edge
    */
    std::vector<double> calculatePageRank(Graph &lingGraph);
    /**
     * @brief isCyclic judge is the graph cyclic
     * @param g the graph
     * @return true if the graph has cycle else false
    */
    bool isCyclic(Graph &g);
    /**
     * @brief getConnectedComponents for Graph
     * @param g the graph
     * @return the connected components
    */
    std::vector<std::vector<int>> getConnectedComponents(Graph &g);
};

#endif // __PAGERANK_H__
