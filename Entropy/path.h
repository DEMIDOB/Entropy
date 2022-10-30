//
//  path.h
//  Entropy
//
//  Created by Даня Демидов on 26.10.22.
//

#ifndef path_h
#define path_h

#include <stdio.h>

#include "entropy.h"

typedef struct SymbolPath SymbolPath;

struct SymbolPath
{
    char* path;
    size_t messageLength;
};

SymbolPath* newSymbolPath(size_t messageLength);
void addStep(SymbolPath* this, bool right);
char* getStrPath(SymbolPath* this);

#endif /* path_h */
