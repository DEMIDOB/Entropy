//
//  emessage.h
//  Entropy
//
//  Created by Даня Демидов on 29.10.22.
//

#ifndef emessage_h
#define emessage_h

#include "entropy.h"
#include "treeNode.h"
#include "path.h"

typedef struct eMessage eMessage;

struct eMessage
{
    string        msgstr;
    unsigned long length;
    
    int         charOccurrences[ALPHABET_CARDINALITY];
    SymbolPath* paths[ALPHABET_CARDINALITY];
    
    char* uniqueChars;
    int   uniqueCharsNum;
    
    float actEntropy;
    float maxEntropy;
    
    void* p_rootNode;
};

eMessage* newEMessage(string msgstr);

void calculateOccurrences(eMessage* message);
void initializePaths(eMessage* message);
void generatePaths(eMessage* message);

float getMaxEntropy(eMessage* message);
float getActEntropy(eMessage* message);

bool containsSymbol(eMessage* message, char s);

#endif /* emessage_h */
