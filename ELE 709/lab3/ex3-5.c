#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct matrix2D{
    int rows;       //rows in matrix
    int cols;       //columns in matrix
    int *matrix;    //pointer to 1D array that represents flattened 2D matrix
};

//global variables for matrices A, B and C
struct matrix2D A, B, C;

//allocates memory to matrix based on its rows and cols
int matrix2D_create(struct matrix2D *matrixStruct, int rows, int cols){
    //all 2D arrays are stored as big 1D arrays, so a cast to an (int *) pointer is needed
    matrixStruct->matrix = (int *) malloc(rows * cols * sizeof(int));
    
    if(matrixStruct->matrix == NULL){
        printf("malloc() failed to create int array: %d ints, size of %d bytes\n", rows*cols, rows*cols*sizeof(int));
        return -1;
    }
    
    //printf("allocated space for %d ints\n", rows*cols); //for some reason, uncommenting this print statement causes a segmentation fault? Not anymore, mixed up length of row as number of total rows( is actually number of total columns)
    
    matrixStruct->rows = rows;
    matrixStruct->cols = cols;
    
    return 0;
}

//frees memory allocated to matrix
void matrix2D_free(struct matrix2D *matrixStruct){
    free(matrixStruct->matrix);
    matrixStruct->matrix = NULL;
}

struct computeArgs{
    int *A_row_i;       //row i from matrix A, elements of row are adjacent to each other in memory
    int *B_col_j;       //column j from matrix B, must skip a row length (num_elements) to get to the next element of the same column
    int *B_cols;    //number of elements to skip to get to next element in column j (located in the next row), i.e. the length of 1 row = # of total columns
    int *num_elements;  //number of elements in row A, same as number of elements in column B
    int *C_ij;          //element of row i and column j in matrix C
};

void *compute_C_ij(void *args){
    struct computeArgs *cArgs = (struct computeArgs *) args;
    int i, j, k, num_elements, B_cols, sum;
    int *A_row_i, *B_col_j, *C_ij;    
    
    //extract function parameters from cArgs
    A_row_i = cArgs -> A_row_i;
    B_col_j = cArgs -> B_col_j;
    B_cols = *(cArgs -> B_cols);
    C_ij = cArgs -> C_ij;
    num_elements = *(cArgs -> num_elements);
    
    /*char s1[50], s2[50];
    sprintf(s1, "A[%d: ] = %d %d %d", i, A_i[0], A_i[1], A_i[2]);
    sprintf(s2, "B[ :%d] = %d %d %d", j, B_j[0], B_j[B.rows], B_j[2*B.rows]);
    printf("%s x %s\n", s1, s2);*/
    
    //For C = A * B, calculate element in row i and column j of matrix C
    //C[i][j] = A[i][k] + B[k][j], where k loops through the indices of the common dimension shared by matrix A and B
    sum = 0;
    for(k = 0; k < num_elements; k++){
        //elements are adjacent in row i
        //must skip row length of matrix B (num_elements) to get to next element in column j of matrix B
        sum +=  A_row_i[k] * B_col_j[k * B_cols];
        //printf("\tk = %d -> (A index %d, B index %d) -> add %d x %d = %d\n", k, k, k*B_cols, A_row_i[k], B_col_j[k * B_cols], sum);
    }
    *C_ij = sum;
    
    //pthread_exit(NULL);
}

//multiples 2 matrices together such that C = A * B
//will allocate the appropriate memory for C based on the dimensions of A and B
int matrix2D_multiply(struct matrix2D *C_ptr, struct matrix2D *A_ptr, struct matrix2D *B_ptr){
    //check if dimensions of A and B allow them to be multiplied together as A * B
    if(A_ptr->cols != B_ptr->rows){
        printf("Can't multiply matrices A and B\nA has %d columns and B has %d rows\n", A.cols, B.rows);
        return -1;
    }
    
    //set dimensions of matrix C, based on dimensions of matrices A and B
    //if cols of A and rows of B are the same, then:
    // C = A * B = (A rows x A cols) * (B rows x B cols) = A rows * B cols
    int rows = A_ptr->rows;
    int cols = B_ptr->cols;
    
    //allocate memory for C matrix, and store its matrix dimensions
    if(matrix2D_create(C_ptr, rows, cols) != 0){
        return -1;
    }   
    
    int i,j;
    
    //create an argument struct for each thread's call to compute_C_ij()
    struct computeArgs args[rows][cols];
    //pthread_t thread[rows][cols];
    int *A_row_i;
    for(i = 0; i < rows; i++){
        A_row_i = A_ptr->matrix + (i * A_ptr->cols);
        for(j = 0; j < cols; j++){
            //setup args for compute_C_ij()
            args[i][j].A_row_i = A_row_i;
            args[i][j].B_col_j = B_ptr->matrix + j;
            args[i][j].B_cols = &(B_ptr->cols);
            args[i][j].num_elements = &(A_ptr->cols); //same as B_ptr->rows, i.e. the common dimension between A and B
            args[i][j].C_ij = C_ptr->matrix + (i * cols) + j;
            //printf("row %d, col %d -> (A index %d, B index %d) -> C_ij index %d\n", i, j, i*rows, j, i*rows + j);
            //*(compute_C_ij)(&(args[i][j]));
            /*if(pthread_create(&(thread[i][j]), NULL, &compute_C_ij, &(args[i][j])) != 0){
                printf("Error in creating thread for element[%d][%d] of matrix C\n", i, j);
            }
            pthread_join(thread[i][j], NULL);*/
        }
    }
    
    //create a unique thread for computing each element in the matrix of A*B
    pthread_t thread[rows][cols];
    for(i = 0; i < rows; i++){
        for(j = 0; j < cols; j++){
            //initialize thread to run compute_C_ij with appropriate i,j args
            if(pthread_create(&(thread[i][j]), NULL, &compute_C_ij, &(args[i][j])) != 0){
                printf("Error in creating thread for element[%d][%d] of matrix C\n", i, j);
            }
        }
    }
    
    //wait for each thread to finish before returning result
    for(i = 0; i < rows; i++){
        for(j = 0; j < cols; j++){
            //wait for thread i j to finish
            pthread_join(thread[i][j], NULL);
        }
    }
    
    return 0;
}

int main(){
    int i, j;
    int rows = 18, cols = 16; //issues with freeing or allocating non-square matrices?
    
    //allocate memory for A and B matrices, and store their respective matrix dimensions
    if(matrix2D_create(&A, 2, 3) != 0){
        exit(-1);
    }
    
    if(matrix2D_create(&B, 3, 5) != 0){
        exit(-1);
    }
    
    //initialize elements of matrix A
    printf("Elements of Matrix A (%d rows by %d cols)\n", A.rows, A.cols);
    for (i = 0; i < A.rows; i++){
        for(j = 0; j < A.cols; j++){
            A.matrix[i * A.cols + j] = (i+1) + (j+1);
            printf("%2d ", A.matrix[i * A.cols + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    //initialize elements of matrix B
    printf("Elements of Matrix B (%d rows by %d cols)\n", B.rows, B.cols);
    for(i = 0; i < B.rows; i++){
        for(j = 0; j < B.cols; j++){
            B.matrix[i * B.cols + j] = (i+1) + 2*(j+1);
            printf("%2d ", B.matrix[i * B.cols + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    //stop program if multiplication failed
    if(matrix2D_multiply(&C, &A, &B) != 0){
        exit(-1);
    }
    
    //print out diagonal elements in C matrix
    printf("Diagonal Elements of Matrix C (%d elements)\n", C.rows);
    int smallestDim;
    if(C.rows < C.cols){
        smallestDim = C.rows;
    }
    else{
        smallestDim = C.cols;
    }
    
    for(i = 0; i < smallestDim; i++){
        for(j = 0; j < i; j++){
            printf("      ");
        }
        printf("%5d\n", C.matrix[i * (C.cols + 1)]); //C.matrix[i * C.cols + i]
    }
    
    //print out each element of C
    printf("\nElements of Matrix C (%d rows by %d cols)\n", C.rows, C.cols);
    for(i = 0; i < C.rows; i++){
        for(j = 0; j < C.cols; j++){
            printf("%5d ", C.matrix[i * C.cols + j]);
        }
        printf("\n");
    }
    
    //free memory allocated for each matrix
    matrix2D_free(&A);
    matrix2D_free(&B);
    matrix2D_free(&C);
}
