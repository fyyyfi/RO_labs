#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

int ProcNum = 0; // Number of available processes
int ProcRank = 0; // Rank of current process
int GridSize; // Size of virtual processor grid
int GridCoords[2]; // Coordinates of current processor in grid
MPI_Comm GridComm; // Grid communicator
MPI_Comm ColComm; // Column communicator
MPI_Comm RowComm; // Row communicator


// Function for simple initialization of matrix cells
void DummyDataInitialization(double* pAMatrix, double* pBMatrix, int Size) {
     int i, j;
    for (i = 0; i < Size; i++)
         for (j = 0; j < Size; j++) {
             pAMatrix[i * Size + j] = 1;
             pBMatrix[i * Size + j] = 1;
         }
 }

// Function for creating the two-dimensional grid communicator
// and communicators for each row and each column of the grid
void CreateGridCommunicators() {
    int DimSize[2]; // Number of processes in each dimension of the grid
    int Periodic[2]; // =1, if the grid dimension should be periodic
    int Subdims[2]; // =1, if the grid dimension should be fixed

    GridSize = sqrt((double)ProcNum);
    DimSize[0] = GridSize;
    DimSize[1] = GridSize;
    Periodic[0] = 0;
    Periodic[1] = 0;

    // Creation of the Cartesian communicator
    MPI_Cart_create(MPI_COMM_WORLD, 2, DimSize, Periodic, 1, &GridComm);

    // Determination of the cartesian coordinates for every process
    MPI_Cart_coords(GridComm, ProcRank, 2, GridCoords);

    // Creating communicators for rows
    Subdims[0] = 0; // Dimensionality fixing
    Subdims[1] = 1; // The presence of the given dimension in the subgrid
    MPI_Cart_sub(GridComm, Subdims, &RowComm);

    // Creating communicators for columns
    Subdims[0] = 1;
    Subdims[1] = 0;
    MPI_Cart_sub(GridComm, Subdims, &ColComm);
}

// Function for memory allocation and data initialization
void ProcessInitialization(double* &pAMatrix, double* &pBMatrix, double* &pCMatrix,
                           double* &pAblock, double* &pBblock, double* &pCblock,
                           double* &pMatrixAblock, int &Size, int &BlockSize) {
    if (ProcRank == 0) {
        do {
            printf("\nEnter the size of matrices: ");
            scanf("%d", &Size);
            if (Size % GridSize != 0) {
                printf("Size of matrices must be divisible by the grid size!\n");
            }
        } while (Size % GridSize != 0);
    }

    MPI_Bcast(&Size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    BlockSize = Size / GridSize;
    pAblock = new double[BlockSize * BlockSize];
    pBblock = new double[BlockSize * BlockSize];
    pCblock = new double[BlockSize * BlockSize];
    pMatrixAblock = new double[BlockSize * BlockSize];

    for (int i = 0; i < BlockSize * BlockSize; i++) {
        pCblock[i] = 0;
    }

    if (ProcRank == 0) {
        pAMatrix = new double[Size * Size];
        pBMatrix = new double[Size * Size];
        pCMatrix = new double[Size * Size];
        DummyDataInitialization(pAMatrix, pBMatrix, Size);
    }
}

void CheckerboardMatrixScatter(double* pMatrix, double* pMatrixBlock, int Size, int BlockSize) {
    double * MatrixRow = new double [BlockSize * Size];
    if (GridCoords[1] == 0) {
        MPI_Scatter(pMatrix, BlockSize * Size, MPI_DOUBLE, MatrixRow, BlockSize * Size, MPI_DOUBLE, 0, ColComm);
    }
    for (int i = 0; i < BlockSize; i++) {
        MPI_Scatter(&MatrixRow[i * Size], BlockSize, MPI_DOUBLE, &(pMatrixBlock[i * BlockSize]), BlockSize, MPI_DOUBLE, 0, RowComm);
    }
    delete [] MatrixRow;
}

void DataDistribution(double* pAMatrix, double* pBMatrix, double* pMatrixAblock, double* pBblock, int Size, int BlockSize) {
    CheckerboardMatrixScatter(pAMatrix, pMatrixAblock, Size, BlockSize);
    CheckerboardMatrixScatter(pBMatrix, pBblock, Size, BlockSize);
}

// Function for computational process termination
void ProcessTermination(double* pAMatrix, double* pBMatrix,
                        double* pCMatrix, double* pAblock, double* pBblock, double* pCblock,
                        double* pMatrixAblock) {
    if (ProcRank == 0) {
        delete[] pAMatrix;
        delete[] pBMatrix;
        delete[] pCMatrix;
    }
    delete[] pAblock;
    delete[] pBblock;
    delete[] pCblock;
    delete[] pMatrixAblock;
}

int main(int argc, char* argv[]) {

  double* pAMatrix = nullptr;
  double* pBMatrix = nullptr;
  double* pCMatrix = nullptr;
  int Size;
  int BlockSize;
  double *pAblock;
  double *pBblock;
  double *pCblock;
  double *pMatrixAblock;

    setvbuf(stdout, 0, _IONBF, 0);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    GridSize = sqrt((double)ProcNum);

    if (ProcNum != GridSize * GridSize) {
        if (ProcRank == 0) {
            printf("Number of processes must be a perfect square \n");
        }
    } else {
        if (ProcRank == 0)
            printf("Parallel matrix multiplication program\n");

        // Creating the cartesian grid, row and column communicators
        CreateGridCommunicators();
        
        ProcessInitialization(pAMatrix, pBMatrix, pCMatrix, pAblock, pBblock,
                              pCblock, pMatrixAblock, Size, BlockSize);

        ProcessTermination(pAMatrix, pBMatrix, pCMatrix, pAblock, pBblock,
                             pCblock, pMatrixAblock);
        
    }

    MPI_Finalize();
    return 0;
}