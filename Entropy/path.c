//
//  path.c
//  Entropy
//
//  Created by Даня Демидов on 26.10.22.
//

#include "path.h"

SymbolPath* newSymbolPath(size_t messageLength)
{
    size_t maxPathSize = sizeof(char) * (messageLength + 1);
    SymbolPath path = {calloc(maxPathSize, maxPathSize), messageLength};
    SymbolPath* pathPtr = malloc(sizeof(SymbolPath));
    *pathPtr = path;
    return pathPtr;
}

void addStep(SymbolPath* this, bool right)
{
    char* step = calloc(DOUBLE_CHAR, DOUBLE_CHAR);
    *step = (int) right + 48;
    
    if (strlen(this->path) >= this->messageLength)
    {
        free(step);
        printf("Trying to add an extra-step when there is no place for it in the memory...\n");
        exit(-2);
    }
    
    strcat(this->path, step);
    free(step);
}

char* getStrPath(SymbolPath* this)
{
    char* invertedPath = calloc(this->messageLength + 1, this->messageLength + 1);
    size_t pathRealLength = strlen(this->path);
    
    for (size_t i = 0; i < pathRealLength; ++i)
    {
        *(invertedPath + pathRealLength - 1 - i) = *(this->path + i);
    }
    
    return invertedPath;
}
