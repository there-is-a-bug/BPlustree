#ifndef BPT_H
#define BPT_H
#include <iostream>
struct BPNode;
struct Node{
    
    int value;
    Node *pre;
    Node *next;
    BPNode *son;
    Node *location;
    Node():value(0),pre(NULL),next(NULL),son(NULL),location(NULL){};
};
struct BPNode{

    Node *parent;
    Node *head;
    BPNode():parent(NULL),head(NULL){};
};
class BPlusTree{

    private:
        BPNode *root;
        int MaxNode;
        Node *Head;
        
    public:
        /* BPT.cpp */
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
        /* insert.cpp */
        void CreatBPNode(BPNode *curr);
        void CreatRoot();
        void Add(Node *curr, Node *nexts, bool top);
        bool InsertMid(int num, BPNode *curr);
        bool InsertLeaf(int num, BPNode *curr);
        bool Insert(int num);
        /* delet.cpp */
        bool GetLeftBro(BPNode *curr, BPNode *bro);
        int GetRightBro(BPNode *curr, BPNode *bro);
        int Adjust(BPNode *curr, int m);
        int Cut(int num,BPNode *curr);
        int Delete(int num, BPNode *curr);
        bool MergerLeft(BPNode *curr, BPNode *bro);
        int MergerRight(BPNode *curr, BPNode *bro);

};
#endif