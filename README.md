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

该算法可以用于可用于计算有向图中的最小反馈弧集（FAS），这是一个与可视化分层结构相关的具有挑战性的问题。它比现有的启发式方法更好，并将FAS大小平均减少了50％以上。尽管由于生成的折线图的大小，对于较大的网图，它的执行时间可能会增加，但即使对于在多达 4,000 个节点的图形绘制应用程序中使用的大型图形，它的运行速度也非常快。因此，这种方法对于研究需要计算 FAS 或涉及有向图（例如排名算法或网络流量分析等）的类似优化任务的研究人员可能很有用。

本项目的实现是基于C语言，可以直接下载源代码并编译运行。详细的使用方法请参考项目中的 README 文件。

## 如何使用

## 数据集

1. 随机生成的图，具有100、200、400、1000、2000、4000个节点，平均出度分别为1.5、3和5。
2. 来自graph Drawing.org数据集的三个有向图，经过适当修改以包含循环(因为原始的是DAG)。
3. 随机生成的图，具有50、100和150个节点，平均出度分别为1.5、3、5、8、10和15。
4. 来自the Laboratory of Web Algorithmics的两个webgraphs

## 项目协作者

## 许可证

This project is licensed under the AGPL-3.0 License. See the LICENSE file for more information.

## 参考文献

[1] Geladaris V ,  Lionakis P ,  Tollis I G . Computing a Feedback Arc Set Using PageRank[J].  2022.
