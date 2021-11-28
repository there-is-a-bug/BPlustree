#include "BPT.h"
#include <iostream>
#include <vector>
#include <deque>
using namespace std;

/* 得到末尾的关键字节点 */
Node *BPlusTree::GetEnd(Node *head){
    head = head->next;
    while(head->next){
        head = head->next;
    }
    return head;
}

/* 得到根节点 */
BPNode *BPlusTree::Getroot(){
    return this->root;
}

/* 层序遍历 */
void BPlusTree::Lists(){
    deque <BPNode *> que;
    que.push_back(this->root);
    cout << "Lists:" << endl;
    BPNode *tmp = NULL;
    Node *head = NULL;
    while(!que.empty()){
        tmp = que.front();
        head = tmp->head;
        while(head){ 
            cout << head->value << ' ';
            if(head->son)
            que.push_back(head->son);
            head = head->next;
        }
        cout << endl;
        que.pop_front();
    }
}

/* 初始化b+树 */
void BPlusTree::SetBPlusTree(int maxnum){
    this->MaxNode = maxnum;
    this->root = Inint();
    this->Head = new Node;
}

/* 初始化一个新的节点 */
BPNode *BPlusTree::Inint(){
    BPNode *tmp = new BPNode;
    tmp->head = new Node;
    return tmp;
}

/* 得到中间的关键字节点，分裂时，由此分裂 */
Node* BPlusTree::GetMidNode(Node *head){
    Node *tmp = head->next;
    int i = (this->MaxNode + 1) / 2;
    while(i > 0){
        tmp = tmp->next;
        --i;
    }
    return tmp;
}

/* 关键字溢出，进行处理，同时返回true */
bool BPlusTree::Overflow(BPNode *curr){
    if(curr->head->value > this->MaxNode){
        if(curr->parent)
        CreatBPNode(curr);
        else
        CreatRoot();
        return true;
    }
    else
    return false;
}

/* 对单个数据进行定位 */
Node *BPlusTree::SearchNum(int num, BPNode *curr){
    Node *tmp = curr->head;
    if(!tmp->next)
    return NULL;
    tmp = tmp->next;
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

/* 进行范围查找，输入左界和右界，若相同，则为查找单个数据 */
Node *BPlusTree::SearchNums(int left, int right){
    Node *tmp = SearchNum(left, this->root);
    int i = 0;
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
    if(i == 0)
    cout << "No Such number!" << endl;
    cout << endl;
    return NULL;
}

/* 判断结构是否改变 */
bool BPlusTree::Balance(BPNode *curr){
    if(curr->head->value >= (this->MaxNode + 1) / 2)
    return true;
    return false;
}