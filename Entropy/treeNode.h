//
//  treeNode.h
//  Entropy
//
//  Created by Даня Демидов on 26.10.22.
//

#include "entropy.h"

#ifndef treeNode_h
#define treeNode_h

//#include <stdio.h>

#endif /* treeNode_h */

typedef struct TreeNode TreeNode;

struct TreeNode {
    int value;
    char symbol;

    char* leadsTo;

    TreeNode* left;
    TreeNode* right;
    TreeNode* flatNext;
};

void setMessageLength(size_t length);

bool hasFlatNext(TreeNode* node);
TreeNode* newNode(int value);
int insertFlat(TreeNode** rootNodePtr, TreeNode* target);
void addReachingChar(TreeNode* this, char* anotherDestination);
TreeNode* newNode_S(char symbol, int occurrences);
