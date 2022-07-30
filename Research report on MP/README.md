# Research report on MP

## 前置知识

### 前缀树

前缀树是一种用于快速检索的**多叉树结构**，利用字符串的公共前缀来降低查询时间，核心思想是**空间换时间**，经常被搜索引擎用于**文本词频统计**。

- 优点：最大限度地减少无谓的字符串比较，查询效率高
- 缺点：内存消耗较大

特性：

1. 不同字符串的相同前缀只**保存一份**
2. 结点不存放数据，数据存储在树的边上，结点存放字符**经过**的次数和**结束**的次数

例如：给出一组单词，adv, air, ait, cat

![img](https://img-blog.csdnimg.cn/93b436ceeaa04c869009c74c7e0f32b3.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAd3nml6Dor60=,size_20,color_FFFFFF,t_70,g_se,x_16)

## 默克尔树
默克尔树（Merkle tree，MT）是一种哈希二叉树，1979年由Ralph Merkle发明. 

默克尔树是自底向上构建的. 将相邻两个节点的哈希值合并成一个字符串，然后计算这个字符串的哈希，得到的就是这两个节点的父节点的哈希值. 

![image-20220730205111832](https://user-images.githubusercontent.com/78082874/181916107-0232ba4b-6e83-4936-a070-41860e7e5bdc.png)
默克尔树有以下性质：

- 校验整体数据的正确性
- 快速定位错误
- 快速校验部分数据是否在原始的数据中
- 存储空间开销大（**大量中间哈希**）

## MPT

### 概述
Merkle Patricia Tree 是一种经过改良的、融合了默克尔树和前缀树两种结构优点的数据结构. 本质上是一种**更节省空间的前缀树**.

MPT树有以下几个作用：

1、存储任意长度的key-value键值对数据.

2、提供了一种快速计算所维护数据集哈希标识的机制.

3、提供了快速状态回滚的机制.

4、提供了一种称为默克尔证明的证明方法，进行轻节点的扩展，实现简单支付验证.

![image-20201231133805927](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/74d05e1331314523a7cdce1e9016a1bc~tplv-k3u1fbpfcp-zoom-in-crop-mark:3024:0:0:0.awebp)

### 节点分类
在MPT中，树节点可以分为：

- 空节点：简单的表示空，在代码中是一个空串
- 分支节点
- 叶子节点：只有两个元素，分别为 key 和 value
- 扩展节点：有两个元素，一个是 key 值，还有一个是 hash 值，这个 hash 值指向下一个节点

### 以太坊中使用到的MPT树结构

- State Trie： 区块头中的状态树
  - key => sha3(以太坊账户地址 address)
  - value => rlp(账号内容信息 account)
- Transactions Trie ：区块头中的交易树
  - key => rlp(交易的偏移量 transaction index)
  - 每个块都有各自的交易树，且不可更改
- Receipts Trie：区块头中的收据树
  - key = rlp(交易的偏移量 transaction index)
  - 每个块都有各自的交易树，且不可更改
- Storage Trie：存储树
  - 存储只能合约状态
  - 每个账号有自己的 Storage Trie

## 轻节点机制
上面的MPT树，有两个问题：

1. 每个节点都包含有大量信息，并且叶子节点中还包含有完整的数据信息. 如果该MPT树并没有发生任何变化，并且没有被使用，则会浪费大量空间.
2. 并不是任何的客户端都对所有的MPT树都感兴趣，若每次都把完整的节点信息都下载下来，会占用大量的磁盘空间和时间.

为了解决上述问题，以太坊使用了一种缓存机制——轻节点机制，具体如下：

- 若某节点数据一直没有发生变化，则仅仅保留该节点的32位hash值，剩下的内容全部释放.
- 若需要插入或者删除某节点，先通过该hash值db中查找对应的节点，并加载到内存，之后再进行删除插入操作.

在轻节点中添加数据过程如下：

![image-20210101204824090](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/5a0e70cb33a44b1d886fa532ee25c31b~tplv-k3u1fbpfcp-zoom-in-crop-mark:3024:0:0:0.awebp)



## 参考链接
1. [前缀树（Trie树）_wy无语的博客-CSDN博客_前缀树](https://blog.csdn.net/wy749929317/article/details/123812165)
2. [死磕以太坊源码分析之MPT树-上 - 掘金 (juejin.cn)](https://juejin.cn/post/6913693343142641671)
