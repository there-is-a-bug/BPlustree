#include "BPT.h"
#include <iostream>
#include <vector>
using namespace std;

bool insert(BPlusTree *t, int num){
    return t->Insert(num);
}

bool search(BPlusTree *t, int left, int right){
    t->SearchNums(left,right);
    return true;
}

bool searchs(BPlusTree *t, int num){
    Node *ans = NULL;
    ans = t->SearchNum(num,t->Getroot());
    if(ans)
    cout << ans->value << endl;
    return true;
}

int main(int argc, char *argv[]){
    int maxnum = 0;
    int i = 0;
    maxnum = atoi(argv[1]);
    BPlusTree *t = new BPlusTree;
    t->SetBPlusTree(maxnum);
    /* 调用insert进行插入 */
    for(i = 1; i < 10; i++){
        if(i != 4)
        insert(t,i);
    }
    insert(t,4);
    /* 调用delete进行删除，并调用lists进行层序遍历 */
    t->Lists();
    t->Delete(6,t->Getroot());
    t->Lists();
    t->Delete(5,t->Getroot());
    t->Lists();
    insert(t,6);
    t->Lists();
    
    return 0;
}