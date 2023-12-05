//Default headers
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//Constants
#ifndef DEFAULT_DOUBLE_VALUE
    #define DEFAULT_DOUBLE_VALUE 0.0
#endif
#ifndef DEFAULT_VEC_ROW_R_SIZE
    #define DEFAULT_VEC_ROW_R_SIZE 1
#endif
#ifndef DEFAULT_VEC_COL_C_SIZE
    #define DEFAULT_VEC_COL_C_SIZE 1
#endif

typedef enum M_TYPE {
    IDENTITY = 0,
    CUSTOM = 1,
    VECTOR_ROW = 2,
    VECTOR_COL = 3
} M_TYPE;


typedef struct MATRIX {
    double** content;
    size_t rows;
    size_t cols;
    M_TYPE matrix_type;
} MATRIX;

/* 
    Creates a identity (I) matrix given the scale of it.
    i.e:
    MATRIX m = CreateIdentity(3);
    m.content will be:
    [
        [1, 0, 0],
        [0, 1, 0],
        [0, 0, 1]
    ]
*/
MATRIX CreateIdentity(const size_t scale)
{
    double **m_content = (double **) malloc(sizeof(double) * scale);
    if (!m_content) 
    {
        fprintf(stderr, "Cannot allocate matrix content.\n");
    }
    for (size_t i = 0; i < scale; ++i)
    {
        m_content[i] = (double *) malloc(sizeof(double) * scale);
        if (!m_content[i]) 
        {
            fprintf(stderr, "Cannot allocate matrix content.\n");
        }
        m_content[i][i] = 1.0;
    }
    MATRIX res;
    res.content = m_content;
    res.cols = scale;
    res.rows = scale;
    res.matrix_type = IDENTITY;
    return res;
}

/*
    Deallocates the matrix content from heap.
    It's crucial do that when the given matrix its no more useful
    i.e:
        MATRIX m = CreateIdentity(2);
        Deallocate(&m);
        Now m.content will be NULL
*/
void Deallocate(MATRIX* matrix)
{
    for (size_t i = 0; i < matrix->rows; ++i)
    {
        free(matrix->content[i]);
        matrix->content[i] = NULL;
    }
    free(matrix->content);
    matrix->content = NULL;
}

void __print_vector_col(MATRIX* m)
{
    puts("[");
    for (size_t i = 0; i < m->rows; ++i)
    {
        printf(i == m->rows - 1? "  %.2lf": "  %.2lf,\n", m->content[i][0]);
    }
    puts("\n]");
}

void __print_vector_row(MATRIX* m)
{
    printf("[ ");
    for (size_t i = 0; i < m->cols; ++i)
    {
        printf(i == m->cols - 1? "%.2lf ]": "%.2lf, ", m->content[i][0]);
    }
}

void __print_bidimensional_matrix(MATRIX* m)
{
    puts("[");
    for (size_t i = 0; i < m->rows; ++i)
    {
        printf("  [ ");
        for (size_t j = 0; j < m->cols; ++j)
        {
            printf(j == m->cols - 1?"%.2lf ": "%.2lf, ", m->content[i][j]);
        }
        printf("]\n");
    }
    puts("]");
}

/*
    Prints the matrix in the stdout.
    The 'consumeAfter' param means if the matrix should be deallocated after the operation succeed.
*/
void PrintMatrix(MATRIX *matrix, bool consumeAfter)
{

    switch (matrix->matrix_type) {
        case VECTOR_COL:
            __print_vector_col(matrix);
            break;
        case VECTOR_ROW:
            __print_vector_row(matrix);
            break;
        case CUSTOM:
        case IDENTITY:
            __print_bidimensional_matrix(matrix);
            break;
    }
    if (consumeAfter)
    {
        Deallocate(matrix);
    }
}

/*
    Creates an vector row given the row size.
    ie:
    MATRIX vecRow = CreateVectorRow(5);
    PrintMatrix(&vecRow, false);
    It will output:
    [ 0.00, 0.00, 0.00, 0.00, 0.00 ]
*/
MATRIX CreateVectorRow(size_t rowSize)
{
    double** row = (double **) malloc(sizeof(double *));
    for (size_t i = 0; i < rowSize; ++i)
    {
        row[i] = (double *) malloc(sizeof(double) * rowSize);
    }
    
    MATRIX res;
    res.content = row;
    res.rows = DEFAULT_VEC_ROW_R_SIZE;
    res.cols = rowSize;
    res.matrix_type = VECTOR_ROW;
    return res;
}

/*
    Creates an vector column given the column size.
    ie:
    MATRIX vecCol = CreateVectorCol(5);
    PrintMatrix(&vecCol, false);
    It will output:
    [
      0.00,
      0.00,
      0.00,
      0.00,
      0.00
    ]
*/
MATRIX CreateVectorCol(size_t colSize)
{
    double** rows = (double **) malloc(sizeof(double *) * colSize);

    for (size_t i = 0; i < colSize; ++i)
    {
        rows[i] = (double *) malloc(sizeof(double *));
        rows[i][0] = DEFAULT_DOUBLE_VALUE;
    }
    MATRIX m;
    m.rows = colSize;
    m.cols = DEFAULT_VEC_COL_C_SIZE;
    m.content = rows;
    m.matrix_type = VECTOR_COL;
    return m;
}


/*
    Creates a matrix with custom row size and column size.
    ie:
    MATRIX custom = CreateCustomMatrix(2, 3);
    PrintMatrix(&custom, false);
    It will output:
    [
        0.00, 0.00, 0.00,
        0.00, 0.00, 0.00
    ]
*/
MATRIX CreateCustomMatrix(size_t rowSize, size_t colSize)
{
    double** row = (double **) malloc(sizeof(double *) * rowSize);
    for (size_t i = 0; i < rowSize; ++i)
    {
        row[i] = (double *) malloc(sizeof(double *) * colSize);
        for (size_t j = 0; j < colSize; ++j)
        {
            row[i][j] = DEFAULT_DOUBLE_VALUE;
        }
    }

    MATRIX m;
    m.rows = rowSize;
    m.cols = colSize;
    m.content = row;
    m.matrix_type = CUSTOM;
    return m;
}