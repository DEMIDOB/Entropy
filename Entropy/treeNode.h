//
//  treeNode.h
//  Entropy
//
//  Created by Даня Демидов on 26.10.22.
//

#ifndef treeNode_h
#define treeNode_h

//#include <stdio.h>

#include "entropy.h"
#include "emessage.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    int value;
    char symbol;
    
    void* relatedMessage;

    char* leadsTo;

    TreeNode* left;
    TreeNode* right;
    TreeNode* flatNext;
};

//void setMessageLength(size_t length);

bool hasFlatNext(TreeNode* node);
TreeNode* newNode(int value, void* relatedMessage);
int insertFlat(TreeNode* p_rootNode, TreeNode* target);
void addReachingChar(TreeNode* this, char* anotherDestination);
TreeNode* newNode_S(char symbol, int occurrences, void* relatedMessage);

#endif /* treeNode_h */
