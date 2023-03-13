# PageRankFAS

本项目是根据论文[1] Geladaris V ,  Lionakis P ,  Tollis I G . Computing a Feedback Arc Set Using PageRank[J].  2022.，基于C语言复现了 PageRankFAS 算法。

PageRankFAS 是一种启发式算法，用于计算有向图中的最小反馈弧集 (FAS)。该算法的工作原理如下：

1. 首先识别有向图中的强连接分量，这些分量是每个顶点都可以从其他每个顶点到达的子图。
2. 对于每个强连接分量，创建其线向图 L。
3. 选择一个随机节点 v，并使用 getLineGraph 函数计算 L 与 v 的折线图。
4. 将 PageRank 算法应用于折线图 L，为每个节点分配 PageRank 分数。
5. 选择 PageRank 分数最高的节点，并将 G 中相应的边缘添加到 FAS 中。
6. 重复步骤 3-5 直到 G 中不再有循环，这意味着已计算出最小 FAS。

PageRankFAS 算法的输入是一个有向图 G，由顶点 V 和边 E 组成。输出是 G 的反馈弧集。

该算法可以用于诸如网络拓扑优化、程序分析和电路设计等领域。

本项目的实现是基于C语言，可以直接下载源代码并编译运行。详细的使用方法请参考项目中的 README 文件。

参考文献：

[1] Geladaris V ,  Lionakis P ,  Tollis I G . Computing a Feedback Arc Set Using PageRank[J].  2022.
