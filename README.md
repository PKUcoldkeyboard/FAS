# Minimum Feedback Arc Set

该项目是一个基于C++ Boost库的实现，实现了最小反馈弧集问题的三种近似算法。该问题是在有向图(DAG)中找到最小的反馈弧集，其中反馈弧集是指一组弧，使得从这些反馈弧的尾部到头部的路径构成一个环。

## 算法实现

该项目基于C++ Boost库实现了三种近似算法：

* GreedyFAS
这是一种基于贪心策略的算法，它选择每个顶点出度最大的反馈弧来构建反馈弧集。

* SortFAS
该算法使用一种启发式策略，它首先计算出每个顶点的出度和入度之和，然后对所有顶点进行排序，按照顺序选择反馈弧，直到形成一个弧集。

* PageRankFAS
该算法是一种启发式算法，来自于论文[1] Geladaris V ,  Lionakis P ,  Tollis I G . Computing a Feedback Arc Set Using PageRank[J].  2022，用于计算有向图中的最小反馈弧集 (FAS)。该算法的工作原理如下：

    1. 首先识别有向图中的强连接分量，这些分量是每个顶点都可以从其他每个顶点到达的子图。
    2. 对于每个强连接分量，创建Line Graph，即L(G)。
    3. 选择一个随机节点 v，并使用 getLineGraph 函数计算 L 与 v 的Line 。
    4. 将 PageRank 算法应用于Line Graph，为每个节点分配 PageRank 分数。
    5. 选择 PageRank 分数最高的节点，并将 G 中相应的边缘添加到 FAS 中。
    6. 重复步骤 3-5 直到 G 中不再有循环，这意味着已计算出最小 FAS。

PageRankFAS 算法的输入是一个有向图 G，由顶点 V 和边 E 组成。输出是 G 的反馈弧集。

该算法可以用于可用于计算有向图中的最小反馈弧集（FAS），这是一个与可视化分层结构相关的具有挑战性的问题。它比现有的启发式方法更好，并将FAS大小平均减少了50％以上。尽管由于生成的折线图的大小，对于较大的网图，它的执行时间可能会增加，但即使对于在多达 4,000 个节点的图形绘制应用程序中使用的大型图形，它的运行速度也非常快。因此，这种方法对于研究需要计算 FAS 或涉及有向图（例如排名算法或网络流量分析等）的类似优化任务的研究人员可能很有用。

本项目的实现是基于C++语言，可以直接下载源代码并编译运行。详细的使用方法请参考项目中的 README 文件。

## 运行项目

如果您想尝试这些算法，需要克隆该项目，然后先安装Boost库，再使用cmake编译运行该项目

1. 打开终端并输入以下命令来更新软件包列表：

   ```
   sudo apt-get update
   ```

2. 输入以下命令来安装Boost库：

   ```
   sudo apt-get install libboost-all-dev
   ```

3. 输入以下命令编译项目

   ```
   cmake .
   make
   ```

## 数据集

1. 简单图
2. 大型图

## 运行结果

## 贡献者

## 许可证

This project is licensed under the AGPL-3.0 License. See the LICENSE file for more information.

## 参考文献

[1] Geladaris V ,  Lionakis P ,  Tollis I G . Computing a Feedback Arc Set Using PageRank[J].  2022.
