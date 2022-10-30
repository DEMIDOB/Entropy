//
//  emessage.c
//  Entropy
//
//  Created by Даня Демидов on 29.10.22.
//

#include "emessage.h"

eMessage* newEMessage(string msgstr)
{
    unsigned long length = strlen(msgstr);
    
    eMessage message = {
        msgstr,
        length,
        
        {},                            // charOccurrences
        {},
        
        malloc(sizeof(char) * length), // uniqueChars
        0,                             // uniqueCharsNum
        
        -1,                            // realEntropy
        -1,                            // maxEntropy
        
        (void*) newNode(-1, &message), // rootNode
    };
    
    calculateOccurrences(&message);
    initializePaths(&message);
    
    eMessage* p_msg = malloc(sizeof(eMessage));
    *p_msg = message;

    return p_msg;
}

void initializePaths(eMessage* message)
{
    for (size_t i = 0; i < message->uniqueCharsNum; i++)
    {
        message->paths[*(message->uniqueChars + i)] = newSymbolPath(message->length);
    }
}

void calculateOccurrences(eMessage* message)
{
    message->uniqueCharsNum = 0;
    
    for (size_t i = 0; i < message->length; ++i)
    {
        char charCode = *(message->msgstr + i);
        ++message->charOccurrences[charCode];
        
        if (message->charOccurrences[charCode] - 1)
            continue;
        
        *(message->uniqueChars + message->uniqueCharsNum++) = charCode;
    }
}

void generatePaths(eMessage* message)
{
    TreeNode* rootNode = (TreeNode*) message->p_rootNode;
    
    TreeNode* c0 = NULL;
    TreeNode* c1 = NULL;
    
    while (hasFlatNext(rootNode->flatNext))
    {
        c0 = rootNode->flatNext;
        c1 = c0->flatNext;
        
        rootNode->flatNext = NULL;
        
        if (hasFlatNext(c1))
        {
            rootNode->flatNext = c1->flatNext;
        }
        
        c0->flatNext = NULL;
        c1->flatNext = NULL;
        int sum = c0->value + c1->value;
        
        TreeNode* newSumNode = newNode(sum, message);
        newSumNode->symbol = '\0';
        newSumNode->left = c0;
        newSumNode->right = c1;
        
        addReachingChar(newSumNode, c0->leadsTo);
        addReachingChar(newSumNode, c1->leadsTo);
        
        char* leftLeadsTo = newSumNode->left->leadsTo;
        char* rightLeadsTo = newSumNode->right->leadsTo;
        
        for (size_t i = 0; i < strlen(leftLeadsTo); ++i)
        {
            addStep(message->paths[*(leftLeadsTo + i)], false);
        }
        
        for (size_t i = 0; i < strlen(rightLeadsTo); ++i)
        {
            addStep(message->paths[*(rightLeadsTo + i)], true);
        }
        
        insertFlat(rootNode, newSumNode);
    }
}

float getMaxEntropy(eMessage* message)
{
    if (message->maxEntropy != -1)
    {
        return message->maxEntropy;
    }
    
    float maxEntropy = log2(message->uniqueCharsNum);
    message->maxEntropy = maxEntropy;
    
    return maxEntropy;
}

float getActEntropy(eMessage* message)
{
    if (message->actEntropy != -1)
    {
        return message->actEntropy;
    }
    
    if (message->p_rootNode == NULL)
    {
        message->p_rootNode = newNode(-1, message);
    }
    
    float entropy = 0;
    
    for (size_t i = 0; i < message->uniqueCharsNum; ++i)
    {
        char c = *(message->uniqueChars + i);
        float occurrences = message->charOccurrences[c];
        entropy += log2(message->length / occurrences) / message->length * occurrences;
        
        // no fancy sorting, sorry for that
        TreeNode* newNode = newNode_S(*(message->uniqueChars + i), (int) occurrences, message);
        insertFlat(message->p_rootNode, newNode);
    }
    
    message->actEntropy = entropy;
    
    return entropy;
}
