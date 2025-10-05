#include <stdio.h>
#include <stdlib.h>
#include <ctime>

void DummyDataInitialization(double* pMatrix, double* pVector, int Size);
void ProcessInitialization(double*& pMatrix, double*& pVector, double*& pResult, int& Size);
void PrintMatrix(double* pMatrix, int RowCount, int ColCount);
void PrintVector(double* pVector, int Size);
void ProcessTermination(double* pMatrix,double* pVector,double* pResult);
void ResultCalculation(double* pMatrix, double* pVector, double* pResult,
int Size);
void RandomDataInitialization (double* pMatrix,double* pVector,int Size);

int main() {
    
    double* pMatrix;
    double* pVector;
    double* pResult;
    int Size;
    clock_t start, finish;
    double duration;

    printf("Serial matrix-vector multiplication program\n");
    ProcessInitialization(pMatrix, pVector, pResult,Size);

    /*
    printf ("Initial Matrix: \n");
    PrintMatrix (pMatrix, Size, Size);
    /*printf ("Initial Vector: \n");
    PrintVector (pVector, Size);*/

    start = clock();
    ResultCalculation(pMatrix, pVector, pResult, Size);
    finish = clock();
    duration = (finish - start) / double(CLOCKS_PER_SEC);
    
    printf("\nTime of execution: %f\n", duration);
    /*printf ("\n Result Vector: \n");
    PrintVector(pResult, Size);*/
    printf("\n Time of execution: %f", duration);

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
    
    RandomDataInitialization(pMatrix, pVector, Size);
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

void ProcessTermination(double* pMatrix,double* pVector,double* pResult) {
delete [] pMatrix;
delete [] pVector;
delete [] pResult;
}

void ResultCalculation(double* pMatrix, double* pVector, double* pResult,
int Size) {
int i, j; // Loop variables
for (i=0; i<Size; i++) {
pResult[i] = 0;
for (j=0; j<Size; j++)
pResult[i] += pMatrix[i*Size+j]*pVector[j];
}
}

void RandomDataInitialization (double* pMatrix,double* pVector,int Size) {
int i, j; // Loop variables
srand(unsigned(clock()));
for (i=0; i<Size; i++) {
pVector[i] = rand()/double(1000);
for (j=0; j<Size; j++)
pMatrix[i*Size+j] = rand()/double(1000);
}
}