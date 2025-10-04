#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int ProcNum = 0; // Number of available processes
int ProcRank = 0; // Rank of current process

// Function for simple setting the matrix and vector elements
void DummyDataInitialization (double* pMatrix, double* pVector, int Size) {
    int i, j; // Loop variables
    for (i=0; i<Size; i++) {
        pVector[i] = 1;
        for (j=0; j<Size; j++)
            pMatrix[i*Size+j] = i;
    }
}

// Function for random setting the matrix and vector elements
void RandomDataInitialization(double* pMatrix, double* pVector, int Size) {
    int i, j; // Loop variables
    srand(unsigned(clock()));
    for (i=0; i<Size; i++) {
        pVector[i] = rand()/double(1000);
        for (j=0; j<Size; j++)
            pMatrix[i*Size+j] = rand()/double(1000);
    }
}

// Function for memory allocation and data initialization
void ProcessInitialization (double* &pMatrix, double* &pVector,
double* &pResult, double* &pProcRows, double* &pProcResult,
int &Size, int &RowNum) {
    int RestRows; // Number of rows, that haven’t been distributed yet
    int i; // Loop variable
    setvbuf(stdout, 0, _IONBF, 0);
    if (ProcRank == 0) {
        do {
            printf("\nEnter the size of the matrix and vector: ");
            scanf("%d", &Size);
            if (Size < ProcNum) {
                printf("Size of the objects must be greater than number of processes! \n ");
            }
        } while (Size < ProcNum);
    }

    MPI_Bcast(&Size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Determine the number of matrix rows stored on each process
    RestRows = Size;

    for (i=0; i<ProcRank; i++)
        RestRows = RestRows-RestRows/(ProcNum-i);
    RowNum = RestRows/(ProcNum-ProcRank);

    // Memory allocation
    pVector = new double [Size];
    pResult = new double [Size];
    pProcRows = new double [RowNum*Size];
    pProcResult = new double [RowNum];

    // Obtain the values of initial data
    if (ProcRank == 0) {
        // Initial matrix exists only on the root process
        pMatrix = new double [Size*Size];
        // Values of elements are defined only on the root process
        RandomDataInitialization(pMatrix, pVector, Size);
    }
}


