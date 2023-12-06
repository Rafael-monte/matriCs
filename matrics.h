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

#ifndef NEGATIVE_LENGTH_TRAP 
    #define NEGATIVE_LENGTH_TRAP 10000000
#endif

#ifndef MAX_ALLOC_SIZE 
    #define MAX_ALLOC_SIZE 10000000
#endif

typedef enum M_TYPE {
    IDENTITY = 0,
    CUSTOM = 1,
    VECTOR_ROW = 2,
    VECTOR_COL = 3
} M_TYPE;

typedef enum INSERTION_STATUS {
    INSERTION_SUCCEED = 1,
    INSERTION_FAILED = 0
} INSERTION_STATUS;

typedef struct MATRIX {
    double** content;
    size_t rows;
    size_t cols;
    M_TYPE matrix_type;
} MATRIX;

void __check_size(size_t size)
{
    if (size <= 0 || size > NEGATIVE_LENGTH_TRAP)
    {
        fprintf(stderr, "Invalid value provided: \"%ld\". Please, give a number that is bigger than 0\n", size);
        exit(EXIT_FAILURE);
    }
}

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
    __check_size(scale);
    double **m_content = (double **) malloc(sizeof(double) * scale);
    if (!m_content) 
    {
        fprintf(stderr, "Cannot allocate matrix content.\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < scale; ++i)
    {
        m_content[i] = (double *) malloc(sizeof(double) * scale);
        if (!m_content[i]) 
        {
            fprintf(stderr, "Cannot allocate matrix content.\n");
            exit(EXIT_FAILURE);
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
        printf(i == m->cols - 1? "%.2lf ]\n": "%.2lf, ", m->content[0][i]);
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
    __check_size(rowSize);
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
    __check_size(colSize);
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
    __check_size(rowSize);
    __check_size(colSize);
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

MATRIX __transpose_row(MATRIX* m)
{
    size_t row_size = m->cols;
    MATRIX res = CreateVectorCol(row_size);
    for (size_t i = 0; i < row_size; ++i)
    {
        res.content[i][0] = m->content[0][i];
    }
    return res;
}

MATRIX __transpose_col(MATRIX* m)
{
    size_t col_size = m->rows;
    MATRIX res = CreateVectorRow(col_size);
    for (size_t i = 0; i < col_size; ++i)
    {
        res.content[0][i]=m->content[i][0];
    }
    return res;
}

MATRIX __transpose_bidimensional(MATRIX* m)
{
    if (m->matrix_type == IDENTITY)
    {
        return CreateIdentity(m->cols);
    }

    size_t row_size = m->cols;
    size_t col_size = m->rows;

    MATRIX res = CreateCustomMatrix(row_size, col_size);
    for (size_t i = 0; i < row_size; ++i)
    {
        for (size_t k = 0; k < col_size; ++k)
        {
            res.content[i][k] = m->content[k][i];
        }
    }

    return res;
}

/*
    Transposes a given matrix.
    i.e:
    Given a matrix like:
    [
        [1, 0, 0]
        [2, 5, 3]
    ]
    Returns the matrix:
    [
        [1, 2]
        [0, 5]
        [3, 0]
    ]
*/
MATRIX Transpose(MATRIX* matrix, bool consumeAfter)
{
    MATRIX res;
    if (matrix->matrix_type == VECTOR_ROW)
    {
        res = __transpose_row(matrix);
    }

    else if (matrix->matrix_type == VECTOR_COL)
    {
        res = __transpose_col(matrix);
    }

    else 
    {
        res = __transpose_bidimensional(matrix); 
    }

    if (consumeAfter)
    {
        Deallocate(matrix);
    }

    return res;
}

/*
    Insert content in matrix at desired position.
    Returns INSERTION_SUCCEED if the value of matrix at row and col was been replaced, INSERCTION_FAILED otherwise.
    i.e:
    MATRIX m = CreateVectorRow(3);
    InsertInMatrix(&m, 0, 2, 3.0);
    PrintMatrix(&m, true);
    The result will be:
    [ 0.00, 0.00, 3.00 ]
*/
INSERTION_STATUS InsertInMatrix(MATRIX* m, size_t row, size_t col, double value)
{
    if (m->content == NULL)
    {
        return INSERTION_FAILED;
    }

    if (row > m->rows || col > m->cols)
    {
        return INSERTION_FAILED;
    }

    m->content[row][col]=value;
    if (m->matrix_type == IDENTITY)
    {
        m->matrix_type=CUSTOM;
    }
    return INSERTION_SUCCEED; 
}