# BFS 算法解题套路框架

BFS 的核心思想应该不难理解的，就是把一些问题抽象成图，从一个点开始，向四周开始扩散。一般来说，我们写 BFS 算法都是用「队列」这种数据结构，每次将一个节点周围的所有节点加入队列。

BFS 相对 DFS 的最主要的区别是：**BFS 找到的路径一定是最短的，但代价就是空间复杂度可能比 DFS 大很多**，至于为什么，我们后面介绍了框架就很容易看出来了。

**问题的本质就是让你在一幅「图」中找到从起点 `start` 到终点 `target` 的最近距离，这个例子听起来很枯燥，但是 BFS 算法问题其实都是在干这个事儿**，



```java
// 计算从起点 start 到终点 target 的最近距离
int BFS(Node start, Node target) {
    Queue<Node> q; // 核心数据结构
    Set<Node> visited; // 避免走回头路
    
    q.offer(start); // 将起点加入队列
    visited.add(start);
    int step = 0; // 记录扩散的步数

    while (q not empty) {
        int sz = q.size();
        /* 将当前队列中的所有节点向四周扩散 */
        for (int i = 0; i < sz; i++) {
            Node cur = q.poll();
            /* 划重点：这里判断是否到达终点 */
            if (cur is target)
                return step;
            /* 将 cur 的相邻节点加入队列 */
            for (Node x : cur.adj()) {
                if (x not in visited) {
                    q.offer(x);
                    visited.add(x);
                }
            }
        }
        /* 划重点：更新步数在这里 */
        step++;
    }
}
```

队列 `q` 就不说了，BFS 的核心数据结构；`cur.adj()` 泛指 `cur` 相邻的节点，比如说二维数组中，`cur` 上下左右四面的位置就是相邻节点；`visited` 的主要作用是防止走回头路，大部分时候都是必须的，但是像一般的二叉树结构，没有子节点到父节点的指针，不会走回头路就不需要 `visited`。



##### LeeCode

| 题目                                                         | 时间               | 难度 | 备注                                                         |
| ------------------------------------------------------------ | ------------------ | ---- | ------------------------------------------------------------ |
| [111. 二叉树的最小深度](https://leetcode.cn/problems/minimum-depth-of-binary-tree/) | 2022/05/11 20:07   | 简单 | 求最小的，用bfs，使用队列，结合while 和for循环               |
| [752. 打开转盘锁](https://leetcode.cn/problems/open-the-lock/) | TODO               | 中等 |                                                              |
| [127. 单词接龙](https://leetcode.cn/problems/word-ladder/)   |                    | 困难 |                                                              |
| [139. 单词拆分](https://leetcode.cn/problems/word-break/)    | 2022-5-15 16:30:57 | 中等 | 15minbfs没通过，遇到一个很难的用例，超时了，这题解法很多，bfs/dfs/dp后面再做做 |
| [130. 被围绕的区域](https://leetcode.cn/problems/surrounded-regions/) | 2022-5-15 16:55:37 | 中等 | 30min，通过了部分用例后发现自己的解法错误，看了题解的bfs     |



## 岛屿问题

DFS解决岛屿问题

| 题目                                                         | 时间                  | 难度 | 备注                                                         |
| ------------------------------------------------------------ | --------------------- | ---- | ------------------------------------------------------------ |
| [200. 岛屿数量](https://leetcode.cn/problems/number-of-islands/) | 2022年5月27日19:07:17 | 中等 | 找了好久的原因，原来是字符‘0’ 我判断和赋值成了数字0，无语啊  |
| [1254. 统计封闭岛屿的数目](https://leetcode.cn/problems/number-of-closed-islands/) | 2022-5-29 21:11:16    | 中等 | 15min，一次性通过，套路都是一样的 dfs                        |
| [1020. 飞地的数量](https://leetcode.cn/problems/number-of-enclaves/) | 2022-5-31 21:44:49    | 中等 | 12min, 返回的是岛屿包含的单元格数量，不是岛屿的个数，复杂一点 |
| [695. 岛屿的最大面积](https://leetcode.cn/problems/max-area-of-island/) |                       |      |                                                              |
| [1905. 统计子岛屿](https://leetcode.cn/problems/count-sub-islands/) |                       |      |                                                              |



