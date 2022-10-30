//
//  treeNode.c
//  Entropy
//
//  Created by Даня Демидов on 26.10.22.
//

#include "treeNode.h"
#include "emessage.h"

bool hasFlatNext(TreeNode* node)
{
    return node->flatNext != NULL;
}

TreeNode* newNode(int value, void* relatedMessage)
{
    unsigned long messageLength = ((eMessage*) relatedMessage)->length;
    
    if (messageLength < 1)
    {
        printf("Message length is set to %zu, terminating...\n", messageLength);
        exit(-1);
    }
    
    TreeNode* nodePtr = malloc(sizeof(TreeNode));
    char* leadsTo = calloc(sizeof(char) * messageLength, sizeof(char) * messageLength);
    TreeNode node = {value, '\0', ((eMessage*) relatedMessage), leadsTo, NULL, NULL, NULL};
    *nodePtr = node;
    return nodePtr;
}

int insertFlat(TreeNode* p_rootNode, TreeNode* target)
{
    int steps = 0;
    TreeNode* p_currentNode = p_rootNode;

    while (hasFlatNext(p_currentNode) && p_currentNode->flatNext->value < target->value)
    {
        p_currentNode = p_currentNode->flatNext;
        ++steps;
    }

    // means that we should insert the target AFTER the what is our currentNode
    TreeNode* currentNext = p_currentNode->flatNext;
    p_currentNode->flatNext = target;
    
    if (currentNext != NULL)
    {
        target->flatNext = currentNext;
    }
    
    return steps;
}

void addReachingChar(TreeNode* this, char* anotherDestination)
{
    size_t adSize = sizeof(char) * strlen(anotherDestination);
    char* ad_nodup = calloc(adSize, adSize);
    size_t ad_nodupLen = 0;
    
    for (size_t i = 0; i < strlen(anotherDestination); ++i)
    {
        char currentSymbol = *(anotherDestination + i);
        bool isNewSymbol = true;
        
        for (int j = 0; j < strlen(this->leadsTo); ++j)
        {
            if (currentSymbol == *(this->leadsTo + j))
            {
                isNewSymbol = false;
                break;
            }
        }
        
        if (isNewSymbol)
        {
            *(ad_nodup + ad_nodupLen) = currentSymbol;
        }
    }
    
    for (int i = 0; i < strlen(this->leadsTo); ++i)
    {
        if (*(this->leadsTo + i) == *anotherDestination)
        {
            free(ad_nodup);
            return;
        }
    }
    
    strcat(this->leadsTo, anotherDestination);
    free(ad_nodup);
}

TreeNode* newNode_S(char symbol, int occurrences, void* relatedMessage)
{
    TreeNode* nodePtr = newNode(occurrences, ((eMessage*) relatedMessage));
    nodePtr->symbol = symbol;
    
    char* symbolStr = malloc(DOUBLE_CHAR);
    symbolStr[0] = symbol;
    symbolStr[1] = '\0';
    
    addReachingChar(nodePtr, symbolStr);
    
    free(symbolStr);
    return nodePtr;
}
