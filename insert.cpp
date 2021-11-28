#include "BPT.h"
#include <iostream>
#include <vector>
#include <deque>
using namespace std;

/* 增加节点，同时写入磁盘 */
void BPlusTree::Add(Node *curr, Node *nexts, bool top){
    Node *tmp = new Node;
    tmp->value = curr->value;
    curr->location = tmp;
    if(this->Head->value == 0){ 
        tmp->pre = this->Head;
        this->Head->next = tmp;
    }
    else{
        if(top){
            tmp->pre = nexts->pre;
            tmp->pre->next= tmp;
            tmp->next = nexts;
            nexts->pre = tmp;
        }
        else{
            nexts->next = tmp;
            tmp->pre = nexts;
        }
    }
    this->Head->value++;
}

/* 插入叶子节点，同时写入磁盘 */
bool BPlusTree::InsertLeaf(int num, BPNode *curr){
    Node *end = new Node;
    end->value = num;
    curr->head->value++;
    if(curr->head->value == 1){
        curr->head->next = end;
        end->pre = curr->head;
        Add(end,curr->head,false);
        return true;
    }
    Node *tmp = curr->head->next;
    while(1){
        if(tmp->value > num){
            end->pre = tmp->pre;
            tmp->pre->next = end;
            end->next = tmp;
            tmp->pre = end;
            Add(end,tmp->location,true);
            break;
        }
        else if(!tmp->next){
            tmp->next = end;
            end->pre = tmp;
            Add(end,tmp->location,false);
            break;
        }
        else
        tmp = tmp->next;
    }
    return Overflow(curr);
}

/* 遍历，直到叶子节点 */
bool BPlusTree::InsertMid(int num, BPNode *curr){
    Node *tmp = curr->head->next;
    if(!tmp->son){
        return InsertLeaf(num,curr);
    }
    else{
        while(1){
            /* 遇到比当前值还要大，要进行交换 */
            if(tmp->value >= num){
                if(InsertMid(num, tmp->son))
                curr->head->value++;
                break;
            }
            else if(!tmp->next){
                tmp->value = num;
            }
            else
            tmp = tmp->next;
        }
    }
    return Overflow(curr);
}

/* 增加节点，可能直接在叶子，也先遍历 */
bool BPlusTree::Insert(int num){
    /* 先判断当前节点是否是叶子节点 */
    
    BPNode *curr = this->root;
    if(curr->head->value != 0)
    InsertMid(num, curr);
    else
    InsertLeaf(num,curr);
    return true;
}

/* 创建新的中间节点 */
void BPlusTree::CreatBPNode(BPNode *curr){
    
    /* 新增节点，并对于节点关键字更新 */
    BPNode *newbpn = Inint();
    int num = curr->head->value;
    newbpn->head->value = num - (num / 2);
    curr->head->value = num / 2;

    /* 得到分裂节点 */
    Node *tmp = GetMidNode(curr->head);

    /* 父节点处新增一个节点，为前半部分的父节点preparent */
    Node *preparent = new Node;
    preparent->value = tmp->pre->value;
    preparent->pre = curr->parent->pre;
    preparent->pre->next = preparent;
    preparent->next = curr->parent;
    curr->parent->pre = preparent;

    /* 将新增父节点和原来的父节点进行设置，同时将链表中间断开 */ 
    curr->parent->son = newbpn;
    newbpn->parent = curr->parent;
    preparent->son = curr;
    curr->parent = preparent;
    tmp->pre->next = NULL;
    newbpn->head->next = tmp;
    tmp->pre = newbpn->head;

}

/* 创建新的根节点 */
void BPlusTree::CreatRoot(){
    /* 新建根节点，其关键字个数为2 */
    BPNode *newroot = Inint();
    newroot->head->value = 2;
    Node *tmp = new Node;
    tmp->pre = newroot->head;
    newroot->head->next = tmp;
    tmp->son = this->root;
    this->root->parent = tmp;
    Node *end = this->root->head;
    while(end->next){
        end = end->next;
    }
    tmp->value = end->value;

    /* 调用分裂函数 */
    CreatBPNode(this->root);

    /* 更新根节点 */
    this->root = newroot;
}