#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int ProcNum = 0; // Number of available processes
int ProcRank = 0; // Rank of current process
int GridSize; // Size of virtual processor grid
int GridCoords[2]; // Coordinates of current processor in grid
MPI_Comm GridComm; // Grid communicator
MPI_Comm ColComm; // Column communicator
MPI_Comm RowComm; // Row communicator

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

int main(int argc, char* argv[]) {
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
        
        
    }

    MPI_Finalize();
    return 0;
}