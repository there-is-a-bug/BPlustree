# 1.B+树概述

本次实验采用严版数据结构中的定义：
$$
对于M阶的B+树
\begin{cases}
1.除根节点外，每个节点的关键字个数至少为\frac{M}{2}(向上取整)个\\
2.每个节点的关键字个数最多为M个 \\
3.如果根节点不是叶节点，那么他至少有两个关键字 \\
4.非叶子节点只存放关键字和指向下一个孩子节点的索引，记录只存放在叶子节点中\\
5.非叶子节点关键字的子树的所有数据均不超过该关键字\\
6.所有的叶子节点都在同一层,并且按升序排列\\
\end{cases}
$$



# 2.数据结构定义

在一颗M阶B+树中，M个关键字组成一个树节点，若干个树节点组成一颗树，所以将关键字，树节点和树分别进行如下定义：

- 关键字定义：

~~~c++
struct Node{
    int value; //关键字的值
    Node *pre; //前一个关键字
    Node *next; //后一个关键字
    BPNode *son; //关键字的子节点
    Node *location; //模拟磁盘位置（叶子节点使用） 
   	Node():value(0),pre(NULL),next(NULL),son(NULL),location(NULL){};
};
~~~



- 树节点定义：

```c++
struct BPNode{
    Node *parent; // 树节点的父节点
    Node *head; // 关键字的头节点（所有关键字组成双向链表）
    BPNode():parent(NULL),head(NULL){};
};
```



- 树的定义：

```c++
class BPlusTree{

    private:
        BPNode *root; // 树的根节点
        int MaxNode; // 树的最大阶数
        Node *Head; // 叶子节点的头指针
        
    public:
        一些函数，后面详解，此处不赘述
        /* BPT.cpp */
        几个初始化函数
        
        /* insert.cpp */
        插入新节点函数
        
        /* delet.cpp */
        删除节点函数

};
```

# 3.基础操作

## 3.1基本函数与查找函数BPT.cpp

```c++
	BPNode *Getroot();
    Node *GetEnd(Node *head);
    bool Overflow(BPNode *curr);
    void SetBPlusTree(int maxnum);
    Node* GetMidNode(Node *head);
    BPNode *Inint();
    void Lists();
    Node *SearchNum(int num, BPNode *curr);
    Node *SearchNums(int left, int right);
    bool Balance(BPNode *curr);
```

- Getroot() 得到根节点

- GetEnd(Node *head) 得到末尾的关键字节点

- Overflow(BPNode *curr) 检测关键字溢出，如果溢出则进行处理，同时返回true

- SetBPlusTree(int maxnum) 初始化b+树

- GetMidNode(Node *head) 得到中间的关键字节点，分裂时，由此分裂

- Inint() 初始化一个新的树节点

- SearchNum(int num, BPNode *curr)

```c++
/* 对单个数据进行定位，返回大于目标的数据位置 */
Node *BPlusTree::SearchNum(int num, BPNode *curr){
    Node *tmp = curr->head;
    if(!tmp->next)
    return NULL;
    tmp = tmp->next;
    /* 如果不为叶子节点，递归查找 */
    if(!tmp->son){
        Node *mins = tmp;
        while(1){
            if(tmp->value == num)
            return tmp->location;
            else if(tmp->value < num)  
            mins = tmp;   
            else if(!tmp->next)
            return mins->location;
            tmp = tmp->next;
        } 
    }
    /* 如果为叶子节点，进行定位 */
    else{
        while(1){
            if(tmp->value >= num)
            return SearchNum(num, tmp->son);
            else if(!tmp->next)
            return NULL;
            tmp = tmp->next;
        }
    }
    return NULL;
}
```

- SearchNums(int left, int right)

```c++
/* 进行范围查找，输入左界和右界，若相同，则为查找单个数据 */
Node *BPlusTree::SearchNums(int left, int right){
    /* 调用单个查找函数，得到不大于左值的数据位置 */
    Node *tmp = SearchNum(left, this->root);
    /* 对输出进行计数 */
    int i = 0;
    /* 遍历数据，满足则输出，且计数+1 */
    while(tmp){
        if(tmp->value < left)
        tmp = tmp->next;
        else if(!tmp || tmp->value > right)
        break;
        else{
            i++;
            cout << tmp->value << ' ';
            tmp = tmp->next;
        }
    }
    /* 若计数不增加，说明没有此段数据 */
    if(i == 0)
    cout << "No Such number!" << endl;
    cout << endl;
    return NULL;
}
```

- Balance(BPNode *curr)  判断当前节点的条件是否满足B+树要求 



## 3.2插入节点insert.cpp

```c++
	void CreatBPNode(BPNode *curr);
    void CreatRoot();
    void Add(Node *curr, Node *nexts, bool top);
    bool InsertMid(int num, BPNode *curr);
    bool InsertLeaf(int num, BPNode *curr);
    bool Insert(int num);
```



### 3.2.1插入不分裂

对于插入操作，如果插入后当前节点的关键字（数据）个数仍小于M个，此时不会产生节点分裂：

- **对于非叶子节点，如果当前值大于最大的关键字，要将该树节点的最大关键字替换成当前数据，然后递归往下插入**
- **对于叶子节点，直接插入新的数据到双向链表中**

例如在图3-1中的3阶B+树中插入20：

![3-1](/png/3-1.png)



由于20大于第三个关键字15，所以要将其替换，插入后如图3-2：

![3-2](/png/3-2.png)

### 3.2.2插入产生分裂

如果插入操作导致关键字个数超过M，就要进行分裂：

- 如果是非根节点

**对于非根节点，将当前节点分裂后，要将分裂后旧节点最大的关键字加入到父节点中：**

例如在图3-2继续插入25，得到图3-3：

![3-3](/png/3-3.png)

分裂后得到图3-4：

![3-4](/png/3-4.png)

- **如果是根节点关键子个数超过M，则可以先创造新的根节点，其只有一个关键字，该关键字的孩子节点就是当前节点，然后按照按照非根节点处理**

例如图3-4，首先生成新的根节点得到图3-5：

![3-5](/png/3-5.png)

然后按照非根节点的处理方式得到图3-6：

![3-6](/png/3-6.png)



### 3.2.3函数实现

- 插入函数insert
- 中间节点插入函数InsertMid
- 叶节点插入函数InsertLeaf
- 插入新节点Add
- 创建新的树节点CreatBPNode
- 创建新的根节点CreatRoot

具体函数实现详见代码（insert.cpp）注释

## 3.3删除节点delet.cpp

### 3.3.1删除无影响

**删除后，当前节点仍保持B+树的结构，对于非叶子节点，如果当前删除的节点在关键字中，将其更新为子节点的最大关键字，其余正常删除**

例如在图3-6中删除9后得到图3-7：

![3-7](/png/3-7.png)

### 3.3.2删除有影响

若删除节点后，当前节点不满足B+树的要求，则要对其如下处理：

- 向兄弟节点借

**当其兄弟节点关键字大于(M + 1) / 2时，代表有多余的节点可以借，于是将其兄弟节点多余的关键字进行转移，同时更新父节点的关键字**

例如删除图3-7中的节点7之后，为满足关键字个数，其应向右兄弟节点借一个，得到图3-8：

![3-8](/png/3-8.png)

**向左兄弟借节点时，过程类似**



- 与兄弟节点合并

**当兄弟节点没有多余关键字无法借时，可以与其进行合并**

例如删除图3-8中的节点8后，由于其兄弟节点没有多余的节点，于是进行合并得到图3-9：

![3-9](/png/3-9.png)

**和左兄弟和并时过程类似**

此时第二层节点只有一个关键子，因此再进行一次合并，得到图3-10：

![3-10](/png/3-10.png)

**如果根节点只有一个关键字，则将其删除，将其字节点作为新的根节点**，得到图3-11：

![3-11](/png/3-11.png)

### 3.3.3函数实现

- 删除中间节点函数Delete
- 删除叶子节点函数Cut
- 决定调整策略Adjust
- 向左兄弟借节点GetLeftBro
- 向右兄弟借节点GetRightBro
- 和左兄弟合并MergerLeft
- 和右兄弟合并MergerRight

具体函数实现详见代码（delet.cpp）注释