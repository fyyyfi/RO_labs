#include <stdio.h>
void DummyDataInitialization(double* pMatrix, double* pVector, int Size);
void ProcessInitialization(double*& pMatrix, double*& pVector, double*& pResult, int& Size);
void PrintMatrix(double* pMatrix, int RowCount, int ColCount);
void PrintVector(double* pVector, int Size);

int main() {
    
    double* pMatrix;
    double* pVector;
    double* pResult;
    int Size;

    printf("Serial matrix-vector multiplication program\n");
    ProcessInitialization(pMatrix, pVector, pResult,Size);

    printf ("Initial Matrix: \n");
    PrintMatrix (pMatrix, Size, Size);
    printf ("Initial Vector: \n");
    PrintVector (pVector, Size);
      
    printf("Press Enter to finish...");
    getchar();
    getchar();

    return 0;
}
void ProcessInitialization(double*& pMatrix, double*& pVector, double*& pResult, int& Size) {
    do {
        printf("Enter the size of the initial objects: ");
        scanf("%d", &Size);
        if (Size <= 0)
            printf("Size of objects must be greater than 0!\n");
    } while (Size <= 0);
    printf("Chosen objects size = %d\n", Size);

    pMatrix = new double[Size * Size];
    pVector = new double[Size];
    pResult = new double[Size];
    
    DummyDataInitialization(pMatrix, pVector, Size);
}

void DummyDataInitialization(double* pMatrix, double* pVector, int Size) {
    for (int i = 0; i < Size; i++) {
        pVector[i] = 1;
        for (int j = 0; j < Size; j++)
            pMatrix[i * Size + j] = i;
    }
}

void PrintMatrix(double* pMatrix, int RowCount, int ColCount) {
    for (int i = 0; i < RowCount; i++) {
        for (int j = 0; j < ColCount; j++)
            printf("%7.4f ", pMatrix[i * RowCount + j]);
        printf("\n");
    }
}

void PrintVector(double* pVector, int Size) {
    for (int i = 0; i < Size; i++)
        printf("%7.4f ", pVector[i]);
}