#include "BPT.h"
#include <iostream>
#include <vector>
#include <deque>
using namespace std;

/* 找左兄弟借节点 */
bool BPlusTree::GetLeftBro(BPNode *curr, BPNode *bro){
    int n = bro->head->value;
    int i = (this->MaxNode + 1) / 2;
    /* 找兄弟节点中要借的节点 */
    Node *tmp = bro->head->next;
    while(i > 0){
        i--;
        tmp = tmp->next;
    }
    Node *end = tmp;
    while(end->next){
        end = end->next;
    }
    /* 调整兄弟的父节点，以及两个兄弟节点的关键字个数 */
    bro->parent->value = tmp->pre->value;
    tmp->pre->next = NULL;
    i = (this->MaxNode + 1) / 2;
    bro->head->value = i;
    curr->head->value = n - i + curr->head->value;
    /* 将剪下来的多余节点给当前节点 */
    end->next = curr->head->next;
    curr->head->next->pre = end;
    curr->head->next = tmp;
    tmp->pre = curr->head;

    return Balance(curr);
}

/* 找右兄弟借节点 */
int BPlusTree::GetRightBro(BPNode *curr, BPNode *bro){
    /* 找兄弟节点中要借的节点 */
    int n = bro->head->value;
    int i = (this->MaxNode + 1) / 2;
    i = n - i;
    Node *tmp = bro->head->next;
    while(i > 0){
        i--;
        tmp = tmp->next;
    }
    Node *end = curr->head->next;
    while(end->next){
        end = end->next;
    }
    /* 调整兄弟的父节点，以及两个兄弟节点的关键字个数 */
    curr->parent->value = tmp->pre->value;
    bro->head->value = (this->MaxNode + 1) / 2;
    curr->head->value = n - bro->head->value + curr->head->value;
    n = tmp->pre->value;

    /* 将剪下来的多余节点给当前节点 */
    end->next = bro->head->next;
    bro->head->next->pre = end;
    tmp->pre->next = NULL;
    bro->head->next = tmp;
    tmp->pre = bro->head;

    return n;
}

/* 找左兄弟合并 */
bool BPlusTree::MergerLeft(BPNode *curr, BPNode *bro){
    /* 将兄弟节点的关键字插入到当前节点 */
    curr->head->value = bro->head->value + curr->head->value;
    Node *end = GetEnd(bro->head);
    curr->head->next->pre = end;
    end->next = curr->head->next;
    end->pre = curr->head;
    curr->head->next = end;
    curr->head->next = bro->head->next;
    bro->head->next->pre = curr->head;
    /* 删除兄弟节点 */
    end = bro->parent;
    end->pre->next = curr->parent;
    curr->parent->pre = end->pre;
    delete bro;
    delete end;
    /* 更新当前节点的关键字数 */
    Node *tmp = curr->parent;
    while(tmp->son){
        tmp = tmp->pre;
    }
    tmp->value--;

    return Balance(curr);
}

/* 找右兄弟合并 */
int BPlusTree::MergerRight(BPNode *curr, BPNode *bro){
    int m = 0;
    /* 修改当前节点的关键字个数，并把兄弟节点的关键字插入到当前节点中 */
    curr->head->value = bro->head->value + curr->head->value;
    Node *end = GetEnd(curr->head);
    end->next = bro->head->next;
    bro->head->next->pre = end;
    end = GetEnd(end);
    m = end->value;
    /* 删除兄弟节点 */
    end = bro->parent;
    curr->parent->value = end->value;
    curr->parent->next = end->next;
    if(end->next)
    end->next->pre = curr->parent;
    delete bro;
    delete end;
    /* 更新关键字所在节点的关键字总数 */
    Node *tmp = curr->parent;
    while(tmp->son){
        tmp = tmp->pre;
    }
    tmp->value--;
    return m;
}

/* 作相应的调整 */
int BPlusTree::Adjust(BPNode *curr, int m){
    bool ok = false;
    int n = (this->MaxNode + 1) / 2;
    /* 试图找左兄弟借节点 */
    Node *s = curr->parent->pre;
    if(s->son && s->son->head->value > n){
        ok = GetLeftBro(curr, s->son);
        return m;
    }
    /* 试图找右兄弟借节点 */
    s = curr->parent->next;
    if(!ok && s && s->son->head->value > n){
        m = GetRightBro(curr, s->son);    
        return m;
    }
    /* 试图找左兄弟合并 */
    s = curr->parent->pre;
    if(s->son){
        ok = MergerLeft(curr, s->son);
        return m;
    }
    /* 试图找右兄弟合并 */
    s = curr->parent->next;
    if(s)
    m = MergerRight(curr, s->son);
    return m;
}

/* 叶子结点的删除操作 */
int BPlusTree::Cut(int num,BPNode *curr){
    Node *s = NULL;
    Node *tmp = curr->head->next;
    if(tmp->value > num)
    cout << "No Such number!" << endl;
    else
    while(1){
        if(tmp->value == num){
            curr->head->value--;
            this->Head->value--;
            if(tmp->next){
                s = tmp->location;
                s->pre->next = s->next;
                s->next->pre = s->pre;
                delete s;
                tmp->pre->next = tmp->next;
                tmp->next->pre = tmp->pre;
                delete tmp;
                return num;
            }
            else{
                tmp->location->pre->next = NULL;
                delete tmp->location;
                s = tmp->pre;
                tmp->pre->next = NULL;
                delete tmp;
                return s->value;
            }
        }
        else if(!tmp->next){
            cout << "No Such number!" << endl;
            return num;
        }
        else
        tmp = tmp->next;
    }
    return 0;
}

/* 中间节点的删除 */
int BPlusTree::Delete(int num, BPNode *curr){
    Node *tmp = curr->head->next;
    if(!tmp)
    cout << "No Such number!" << endl;
    int m = 0;
    /* 此节点为叶子节点 */
    if(!tmp->son){
        m = Cut(num,curr);
    }
    else{
        /* 继续递归删除 */
        while(1){
            if(tmp->value > num){
                m = Delete(num,tmp->son);
                break;
            }
            else if(tmp->value == num){
                m = Delete(num,tmp->son);
                if(tmp->son->head->value != 0){
                    tmp->value = m;
                }
                else{
                    m = tmp->pre->value;
                    curr->head->value--;
                    delete tmp->son;
                    if(tmp->next){
                        tmp->next->pre = tmp->pre;
                        tmp->pre->next = tmp->next;  
                    }
                    else
                    tmp->pre->next = NULL;
                    delete tmp;
                }
                break;
            }
            tmp = tmp->next;
        }
    }
    if(curr != this-> root && !Balance(curr))
    m = Adjust(curr, m);
    /* 如果根节点不平衡，直接删除根节点 */
    else if(curr == this->root && this->root->head->value == 1){
        BPNode *tmp = this->root;
        this->root = tmp->head->next->son;
        delete tmp;
        this->root->parent = NULL;
    }
    return m;
}