#include <stdio.h>
#include <stdlib.h>

struct RecordType
{
    int id;
    char name;
    int order;
};

// node in the linked list
struct ListNode
{
    struct RecordType data;
    struct ListNode* next;
};

// Fill out this structure
struct HashType
{
    struct ListNode** lists;
    int size;
};

// Compute the hash function
int hash(int x, int tableSize)
{
    return x % tableSize;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;
    *ppData = NULL;
    
    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }
    return dataSz;
}

void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

void displayRecordsInHash(struct HashType* pHashArray)
{
    int i;
    struct ListNode* node;
    for (i = 0; i < pHashArray->size; ++i)
    {
        node = pHashArray->lists[i];
        if (node)
        {
            printf("index %d -> ", i);
            while (node)
            {
                printf("%d, %c, %d -> ", node->data.id, node->data.name, node->data.order);
                node = node->next;
            }
            printf("NULL\n");
        }
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    struct HashType hashTable;
    int i, idx;
    int hashSize = 10;

    hashTable.lists = (struct ListNode**) malloc(sizeof(struct ListNode*) * hashSize);
    hashTable.size = hashSize;
    for (i = 0; i < hashSize; ++i)
        hashTable.lists[i] = NULL;

    for (i = 0; i < recordSz; ++i)
    {
        idx = hash(pRecords[i].id, hashSize);
        struct ListNode* newNode = (struct ListNode*) malloc(sizeof(struct ListNode));
        newNode->data = pRecords[i];
        newNode->next = hashTable.lists[idx];
        hashTable.lists[idx] = newNode;
    }

    displayRecordsInHash(&hashTable);

    for (i = 0; i < hashTable.size; ++i) {
        struct ListNode* current = hashTable.lists[i];
        while (current != NULL) {
            struct ListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable.lists);
    free(pRecords);

    return 0;
}
