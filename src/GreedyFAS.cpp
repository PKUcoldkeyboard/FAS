#include "GreedyFAS.h"

boost::container::vector<EdgePair> GreedyFAS::getFeedbackArcSet(Graph &g) {
    boost::container::vector<EdgePair> feedback_arcs;
    SPDLOG_INFO("Starting GreedyFAS...");
    
    // boost::container::vector<int> s1, s2, s;
    // boost::container::vector<int> index;//记录每个顶点的id
    
    // g::vertex_iterator vertexIt, vertexEnd;//会报错
    // g::edge_iterator edgeIt, edgeEnd;//会报错

    // Graph g2 = g; //拷贝一份不做移除的原图

    // //记录每个顶点的索引对应的id
    // for (auto vertexIt: boost::make_iterator_range(boost::vertices(g))) {
    // 	if (out_degree(*vertexIt, g)>0){
	//     	boost::tie(edgeIt, edgeEnd) = boost::out_edges(*vertexIt, g);
	//     	int u = boost::source(edgeIt, g);
	//     	index.push_back(u);
	//     else{
	//         boost::tie(edgeIt, edgeEnd) = boost::in_edges(*vertexIt, g);
	//     	int v = boost::target(edgeIt, g);
	//     	index.push_back(v);
	//     }
    // }

    // while(boost::num_vertices(g)!=0){
    // 	vertex_iter vertexIt, vertexEnd;

    // 	// 找源头点 但是孤立点的id确定不了 下同
    //     for (auto vertexIt: boost::make_iterator_range(boost::vertices(g))) {
    // 		if (in_degree(*vertexIt, g)==0){
	//     	    boost::tie(edgeIt, edgeEnd) = boost::out_edges(*vertexIt, g);
	//     	    int u = boost::source(edgeIt, g);
    // 		    s1.push_back(u); //确定对应id
    // 		    boost::clear_vertex(*vertexIt, g);
    // 		    boost::remove_vectex(*vertexIt, g);
    // 	}
    // 	if (boost::num_vertices(g)==0){
    // 		break;
    // 	}

    //     // 找汇聚点
    //     for (auto vertexIt: boost::make_iterator_range(boost::vertices(g))) {
    // 	    if (out_degree(*vertexIt, g)==0){
    // 	        boost::tie(edgeIt, edgeEnd) = boost::in_edges(*vertexIt, g);
	//     	    int v = boost::target(edgeIt, g);
    // 		    s2.push_back(v); //确定对应id
    // 		    boost::clear_vertex(*vertexIt, g);
    // 		    boost::remove_vectex(*vertexIt, g);
    //     }

    // 	if (boost::num_vertices(g)==0){
    // 		break;
    // 	}
    //     // 找delta最大的点
    //     int dtemp = -1;
    //     int flag = 1;
    //     vtemp;//确定数据类型
    //     for (auto vertexIt: boost::make_iterator_range(boost::vertices(g))) {
    // 	    int dout = out_degree(*vertexIt, g);
    // 	    int din = in_degree(*vertexIt, g);
    // 	    if (dout-din > dtemp){
    // 		    vtemp = vertexIt;
    // 	    }
    //     }
    //     if (in_degree(*vtemp, g)==0){
    //         for (auto edgeIt : boost::make_iterator_range(boost::out_edges(*vtemp, g))) {
	//     	    int u = boost::source(edgeIt, g);
    //     	    s1.push_back(u); //确定对应id
    //             break;
    //         }
    //     }
    //     else{ //多余的
    //         for (auto edgeIt : boost::make_iterator_range(boost::in_edges(*vtemp, g))) {
	//     	    int v = boost::target(edgeIt, g);
    //     	    s1.push_back(v); //确定对应id
    //             break;
    //     }
    //     boost::clear_vertex(vtemp, g);
    //     boost::remove_vectex(vtemp, g);
    // }
    
    // reverse(s2.begin(), s2.end());
    // s.insert(s.end(), s1.begin(), s1.end());
    // s.insert(s.end(), s2.begin(), s2.end());
    
    // for (int i=0; i<s.size(); i++){
    // 	int u = s[i];
    // 	for (auto ei : boost::make_iterator_range(boost::out_edges(u, g2))) {
    //         int v = boost::target(ei, g);
    //         if (index[u]>index[v]){
    //         	feedback_arcs.emplace_back(std::make_pair(g2[u], g2[v]));
    //         }
    //     }
    // }
    return feedback_arcs;
}