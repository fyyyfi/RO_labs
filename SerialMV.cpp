#include <stdio.h>
void DummyDataInitialization(double* pMatrix, double* pVector, int Size);
void ProcessInitialization(double*& pMatrix, double*& pVector, double*& pResult, int& Size);
void ProcessTermination(double* pMatrix, double* pVector, double* pResult);

int main() {
    
    double* pMatrix;
    double* pVector;
    double* pResult;
    int Size;

    printf("Serial matrix-vector multiplication program\n");
    ProcessInitialization(pMatrix, pVector, pResult,Size);
    
    ProcessTermination(pMatrix, pVector, pResult);
    
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

void ProcessTermination(double* pMatrix, double* pVector, double* pResult) {
    delete[] pMatrix;
    delete[] pVector;
    delete[] pResult;
}