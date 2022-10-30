#ifndef emessage_h
#include "emessage.h"
#endif

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
    char* msgstr = malloc(sizeof(char) * MAX_MESSAGE_LENGTH);
    printf("Your message (ASCII): ");
    scanf("%s", msgstr);
    
    eMessage* message = newEMessage(msgstr);
    
    printf("\nMessage length is  %zu\n", message->length);
    printf("Unique characters: %d\n", message->uniqueCharsNum);
    
    // calculate entropy and build the first level of the BinaryTree structure
    float maxEntropy = getMaxEntropy(message);
    float actEntropy = getActEntropy(message);
    
    printf("\nMax entropy:       %f\n", maxEntropy);
    printf("Actual entropy:    %f\n", actEntropy);
    printf("Redundancy:        %f\n", maxEntropy - actEntropy);
    
    printf("\n");
    
    printf("Bruteforce size:   %d bit\n", (int) ceil(maxEntropy) * message->length);
    printf("Min size:          %d bit\n", (int) ceil(actEntropy * message->length));
    
    TreeNode* rootNode = (TreeNode*) message->p_rootNode;
    
    if (!hasFlatNext(rootNode))
    {
        free(msgstr);
        free(message->uniqueChars);
        free(message);
        return -1;
    }
    
    generatePaths(message);
    
    if (!hasFlatNext(rootNode) || hasFlatNext(rootNode->flatNext))
    {
        free(msgstr);
        free(message->uniqueChars);
        free(message);
        printf("rootNode does not have next after tree construction\n");
        return -1;
    }
    
    
    // generate uncompressed
    printf("\nBruteforce:        ");
    for (size_t i = 0; i < message->length; ++i)
    {
        char s = msgstr[i];
        for (size_t j = 0; j < message->uniqueCharsNum; ++j)
        {
            if (*(message->uniqueChars + j) == s)
            {
                char* str = byteToBin(s, ceil(maxEntropy));
                printf("%s", str);
                free(str);
                break;
            }
        }
    }
    
    printf(" (%zu bit)", ((int) ceil(maxEntropy)) * message->length);
    
    // generate compressed
    printf("\nCompressed:        ");
    
    size_t compressedLength = 0;
    size_t maxSymbolLength = 0;
    for (size_t i = 0; i < message->length; ++i)
    {
        char s = msgstr[i];
        char* encodedSymbol = getStrPath(message->paths[s]);
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
    for (size_t i = 0; i < message->uniqueCharsNum; ++i)
    {
        char currentChar = *(message->uniqueChars + i);
        char* unc_str = byteToBin(currentChar, ceil(maxEntropy));
        printf(" %c - %s - %s\n", currentChar, unc_str, getStrPath(message->paths[currentChar]));
        free(unc_str);
    }
    
    free(message->uniqueChars);
    free(msgstr);
    free(message);

#ifdef _WIN32

    printf("\n");
    system("pause");

#endif // _WIN32

    return 0;
}

int main(int argc, const char * argv[])
{
    return entropy();
}
