#include "entropy.h"
#include "treeNode.h"
#include "path.h"

typedef char byte;

int charOccurrences[ALPHABET_CARDINALITY];

char* byteToBin(byte in, size_t length)
{
    char* str = calloc(sizeof(char) * (length + 1), sizeof(char) * (length + 1));
    
    for (int i = (int) length - 1; i >= 0; i--)
    {
        char currentChar = abs(in % 2) + 48;
        *(str + i * sizeof(char)) = currentChar;
        
        in -= currentChar;
        in >>= 1;
    }
    
    return str;
}

int entropy(void)
{
    // scan the message
    char* message = malloc(sizeof(char) * MAX_MESSAGE_LENGTH);
    printf("Your message (ASCII): ");
    scanf("%s", message);
    
    // determine its length
    size_t messageLength = strlen(message);
    setMessageLength(messageLength);
    // while (++messageLength < MAX_MESSAGE_LENGTH && *(message + messageLength) != '\0');
    
    // 1st output
    printf("\nMessage length is  %zu\n", messageLength);
    
    // determine unique chars
    char* uniqueChars = malloc(sizeof(char) * messageLength);
    int uniqueCharsNum = 0;
    
    for (size_t i = 0; i < messageLength; ++i)
    {
        char charCode = *(message + i);
        ++charOccurrences[charCode];
        
        if (charOccurrences[charCode] - 1)
            continue;
        
        *(uniqueChars + uniqueCharsNum++) = charCode;
    }
    
    printf("Unique characters: %d\n", uniqueCharsNum);
    
    // calculate entropy and build the first level of the BinaryTree structure
    float entropy = 0;
    float maxEntropy = log2(uniqueCharsNum);
    
    TreeNode* rootNodePtr = newNode(-1);
    
    for (size_t i = 0; i < uniqueCharsNum; ++i)
    {
        char c = *(uniqueChars + i);
        float occurrences = charOccurrences[c];
        entropy += log2(messageLength / occurrences) / messageLength * occurrences;
        
        // no fancy sorting, sorry for that
        TreeNode* newNode = newNode_S(*(uniqueChars + i), (int) occurrences);
        insertFlat(&rootNodePtr, newNode);
    }
    
    printf("\nMax entropy:       %f\n", maxEntropy);
    printf("Actual entropy:    %f\n", entropy);
    printf("Redundancy:        %f\n", maxEntropy - entropy);
    
    printf("\n");
    
    if (!hasFlatNext(rootNodePtr))
    {
        free(message);
        free(uniqueChars);
        return -1;
    }
    
    TreeNode* c0 = NULL;
    TreeNode* c1 = NULL;
    
    SymbolPath* paths[ALPHABET_CARDINALITY];
    
    for (size_t i = 0; i < uniqueCharsNum; i++)
    {
        paths[*(uniqueChars + i)] = newSymbolPath(messageLength);
    }
    
    while (hasFlatNext(rootNodePtr->flatNext))
    {
        c0 = rootNodePtr->flatNext;
        c1 = c0->flatNext;
        
        rootNodePtr->flatNext = NULL;
        
        if (hasFlatNext(c1))
        {
            rootNodePtr->flatNext = c1->flatNext;
        }
        
        c0->flatNext = NULL;
        c1->flatNext = NULL;
        int sum = c0->value + c1->value;
        
        TreeNode* newSumNode = newNode(sum);
        newSumNode->symbol = '\0';
        newSumNode->left = c0;
        newSumNode->right = c1;
        
        addReachingChar(newSumNode, c0->leadsTo);
        addReachingChar(newSumNode, c1->leadsTo);
        
        char* leftLeadsTo = newSumNode->left->leadsTo;
        char* rightLeadsTo = newSumNode->right->leadsTo;
        
        for (size_t i = 0; i < strlen(leftLeadsTo); ++i)
        {
            addStep(paths[*(leftLeadsTo + i)], false);
        }
        
        for (size_t i = 0; i < strlen(rightLeadsTo); ++i)
        {
            addStep(paths[*(rightLeadsTo + i)], true);
        }
        
        insertFlat(&rootNodePtr, newSumNode);
    }
    
    if (!hasFlatNext(rootNodePtr) || hasFlatNext(rootNodePtr->flatNext))
    {
        free(message);
        free(uniqueChars);
        printf("rootNode does not have next after tree construction\n");
        return -1;
    }
    
    printf("Max size:          %d bit\n", (int) ceil(maxEntropy) * messageLength);
    printf("Min size:          %d bit\n", (int) ceil(entropy * messageLength));
    
    
    // generate uncompressed
    printf("\nUncompressed:      ");
    for (size_t i = 0; i < messageLength; ++i)
    {
        char s = message[i];
        for (size_t j = 0; j < uniqueCharsNum; ++j)
        {
            if (*(uniqueChars + j) == s)
            {
                char* str = byteToBin(s, ceil(maxEntropy));
                printf("%s", str);
                free(str);
                break;
            }
        }
    }
    
    printf(" (%zu bit)", ((int) ceil(maxEntropy)) * messageLength);
    
    // generate compressed
    printf("\nCompressed:        ");
    
    size_t compressedLength = 0;
    size_t maxSymbolLength = 0;
    for (size_t i = 0; i < messageLength; ++i)
    {
        char s = message[i];
        char* encodedSymbol = getStrPath(paths[s]);
        size_t currentLength = strlen(encodedSymbol);
        compressedLength += currentLength;
        
        if (maxSymbolLength < currentLength)
        {
            maxSymbolLength = currentLength;
        }
        
        printf("%s", encodedSymbol);
    }
    
    printf(" (%zu bit)\n", compressedLength);
    
    printf("\n----------------\n");
    printf(" Encoding table \n");
    printf("----------------\n");
    for (size_t i = 0; i < uniqueCharsNum; ++i)
    {
        char currentChar = *(uniqueChars + i);
        char* unc_str = byteToBin(currentChar, ceil(maxEntropy));
        printf(" %c — %s — %s\n", currentChar, unc_str, getStrPath(paths[currentChar]));
        free(unc_str);
    }
    
    free(uniqueChars);
    free(message);

    return 0;
}

int main(int argc, const char * argv[])
{
    return entropy();
}
