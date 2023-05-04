# Minimum Feedback Arc Set

[![Boost](https://img.shields.io/badge/Boost-1.81.0-brightgreen.svg)](https://www.boost.org/)
![](https://img.shields.io/badge/gtest-1.11.0--3-red.svg)
![](https://img.shields.io/badge/Version-0.1-orange.svg)
[![emhash](https://img.shields.io/badge/emhash-7%2F8-blueviolet.svg)](https://github.com/ktprime/emhash)

[English](README-en.md) | [简体中文](README.md)

该项目是一个基于链式前向星存图、boost（boost::hash、asio线程池）以及emhash7/8的<b>非官方</b>实现，实现了最小反馈弧集问题的三种近似算法。该问题是在有向图中找到最小的反馈弧集，其中反馈弧集是指一组弧，使得从这些反馈弧的尾部到头部的路径构成一个环。

## 算法实现

该项目基于C++实现了三种近似算法：

* GreedyFAS
这是一种基于贪心策略的算法，用贪心法生成一个线性排列，将该线性排列中的后向边集作为结果返回。

- 贪心策略
    - 查找源头点，若查到源头点则排到序列s1末尾并移除该点，重复直到没有源头点
    - 查找汇集点，若查到汇集点则排到序列s2头部并移除该点，重复直到没有汇集点
    - 若既没有源头点，也没有汇集点，则定义delta值（出度-入度），将delta最大的点排到s1末尾。
    - 计算剩余点的delta，将delta值最大的点排在s1末尾并移除该点。
    - 返回{s1,s2} -> 最小线性排列

![](images/GreedyFAS.png)

* SortFAS
该算法根据序号的自然顺序生成初始最小线性排列问题（LA），不断调整LA使后向边的数量尽可能少。

![](images/SortFAS.png)

* PageRankFAS
该算法是一种启发式算法，来自于论文[1] Geladaris V ,  Lionakis P ,  Tollis I G . Computing a Feedback Arc Set Using PageRank[J].  2022，用于计算有向图中的最小反馈弧集 (FAS)。该算法的工作原理如下：

   检测图是否有环，如果存在环，执行以下循环：
   1. 识别有向图中的强连接分量si, i=0,1,...
   2. 遍历强连通分量si，对于每个强连通分量si，执行：
      1. 如果si的大小为1，跳过该强连通分量的处理
      2. 选择si中的一个随机节点v，从v开始遍历创建si的线图L(si)
      3. 计算L(si)的PageRank
      4. 选择L(si)中PageRank值最大的节点，找到si中对应的边e，添加到最小反馈弧集。
      5. 在si中删除边e
   3. 如果仍有环，重复执行1和2，直到图不存在环为止。


![](images/LineGraph.png)
![](images/PageRank.png)
![](images/PageRankFAS.png)

PageRankFAS 算法的输入是一个有向图 G，由顶点 V 和边 E 组成。输出是 G 的反馈弧集。

该算法可以用于可用于计算有向图中的最小反馈弧集（FAS），这是一个与可视化分层结构相关的具有挑战性的问题。它比现有的启发式方法更好，并将FAS大小平均减少了50％以上。尽管由于生成的折线图的大小，对于较大的网图，它的执行时间可能会增加，但即使对于在多达 4,000 个节点的图形绘制应用程序中使用的大型图形，它的运行速度也非常快。因此，这种方法对于研究需要计算 FAS 或涉及有向图（例如排名算法或网络流量分析等）的类似优化任务的研究人员可能很有用。

本项目的实现是基于C++语言，可以直接下载源代码并编译运行。详细的使用方法请参考项目中的 README 文件。

## 运行项目

如果您想尝试这些算法，需要克隆该项目，然后先安装Boost和gtest库，再使用cmake编译运行该项目

1. 打开终端并输入以下命令来更新软件包列表：

   ```
   sudo apt-get update
   ```

2. 输入以下命令来安装Boost库和gtest库：

   ```
   sudo apt-get install libboost-all-dev libgtest-dev
   ```

3. 输入以下命令编译项目

   ```
   cmake -B build && cmake --build build
   ```

4. 输入以下命令运行项目

   ```
   ./build/FASSolver [path/to/graph] [alorigthm (greedy | sort | pagerank)]
   ```

## 数据集

### 简单图

- graphs/simple.txt

   ```
   0,1
   1,2
   2,3
   3,0
   3,1
   4,5
   5,6
   6,4
   ```

### 大型图

- graphs/wordassociation-2011.txt: 10,617 个顶点和 72,172 条有向边
- graphs/enron.txt: 69,244 个顶点和 276,143 条有向边

## 运行结果

### 简单图: 

- graphs/simple.txt
  - GreedyFAS

   ```
   2
   2,3
   6,4
   ```

![](result/simple.png)
![](result/simple_after_greedy.png)

- graphs/simple.txt
  - SortFAS

   ```
   2
   2,3
   5,6
   ```

![](result/simple.png)
![](result/simple_after_sort.png)

- graphs/simple.txt
  - PageRankFAS

   ```
   2
   1,2
   4,5
   ```

![](result/simple.png)
![](result/simple_after.png)

### 大型图
- graphs/wordassociation-2011.txt
  - GreedyFAS: 13634条反馈弧, 耗时0.701s
  - SortFAS: 13510条反馈弧, 耗时0.817s
  - PageRankFAS: 12086条反馈弧, 耗时68.856s
- graphs/enron.txt
  - GreedyFAS: 38850条反馈弧, 耗时10.989s
  - SortFAS: 36548条反馈弧, 耗时14.281s
  - PageRankFAS: 33796条反馈弧, 耗时1398.224s

<div style="display:flex;">
    <img src="result/fas_wordassociation-2011.png" style="width:40%; object-fit: cover;">
    <img src="result/fas_enron.png" style="width:40%; object-fit: cover;">
</div>

## 贡献者
<a href="https://github.com/PKUcoldkeyboard/FAS/graphs/contributors">
  <img src="https://contrib.rocks./image?repo=PKUcoldkeyboard/FAS" />
</a>

Made with [contrib.rocks](https://contrib.rocks.).

## 许可证

This project is licensed under the AGPL-3.0 License. See the LICENSE file for more information.

## 参考文献

[1] Geladaris V ,  Lionakis P ,  Tollis I G . Computing a Feedback Arc Set Using PageRank[J].  2022.
