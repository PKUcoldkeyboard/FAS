/**
 * PageRankFAS.h - PageRankFAS Solver
 * This class implements a FASStrategy for solving the feedback arc set problem using PageRank algorithm.
 */
#ifndef __PAGERANK_FAS_H__
#define __PAGERANK_FAS_H__

#include "strategy.h"

class PageRankFAS: public FASStrategy {
public:
    /**
     * @brief 计算最小反馈弧集
     * @param g - 输入图
     * @return 最小反馈弧集，其中最小反馈弧用std::pair表示，如：(0,1),(1,2)...第一个数为起点，第二数为终点
     * @note 该函数使用PageRankFAS算法实现
    */
    boost::container::vector<EdgePair> getFeedbackArcSet(Graph &g) override;
private:
    void getLineGraph(const Graph &g, LineGraph &lineGraph, Vertex v, LineVertex prev,
                      boost::container::vector<bool> &visited, boost::unordered_set<Vertex> &scc,
                      EdgeToVertexMap &edge_to_vertex_map, VertexToEdgeMap &vertex_to_edge_map);
    /**
     * @brief 计算PageRank值
     * @param lineGraph - 输入图
     * @param pagerank - PageRank值
     * @return
    */
    void computePageRank(const LineGraph &lineGraph, boost::container::vector<double> &pagerank);
    /**
     * @brief 计算强连通分量
     * @param g - 输入图
     * @param sccs - 强连通分量集合
     * @return
     * @note 该函数使用Kosaraju算法实现
    */
    void computeStronglyConnectedComponents(Graph &g, boost::container::vector<boost::unordered_set<Vertex>> &sccs);
    /**
     * @brief 判断图是否有环
     * @param g - 输入图
     * @return
     * @note 该函数使用DFS算法实现
    */
    bool isCyclic(Graph &g);
};

#endif // __PAGERANK_H__
