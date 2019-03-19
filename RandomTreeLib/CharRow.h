#ifndef CharRow_H_INCLUDE_GUARD
#define CharRow_H_INCLUDE_GUARD

typedef struct {
    int Size;
    char* Data;
} CharRow;

void CrInit(CharRow *vector);
int CrAppend(CharRow *vector, const char* value, const unsigned int valueLen);
void CrFreeMemory(CharRow *vector);

#endif