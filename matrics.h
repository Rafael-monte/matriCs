//Default headers
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//Constants
#ifndef DEFAULT_FLOAT_VALUE
    #define DEFAULT_DOUBLE_VALUE 0.0
#endif

typedef enum MATRIX_TYPE {
    IDENTITY = 0,
    CUSTOM = 1,
    VECTOR_ROW = 2,
    VECTOR_COL = 3
} MATRIX_TYPE;


typedef struct MATRIX {
    double** content;
    int rows;
    int cols;
    MATRIX_TYPE matrix_type;
} MATRIX;

MATRIX CreateIdentity(const unsigned int scale)
{
    double **m_content = (double **) malloc(sizeof(double) * scale);
    if (!m_content) 
    {
        fprintf(stderr, "Cannot allocate matrix content.\n");
    }
    for (int i = 0; (unsigned int) i < scale; ++i)
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

void Deallocate(MATRIX* matrix)
{
    for (int i = 0; i < matrix->rows; ++i)
    {
        free(matrix->content[i]);
    }
}

void PrintMatrix(MATRIX *matrix, bool consumeAfter)
{
    puts("[");
    for (int i = 0; i < matrix->rows; ++i)
    {
        printf("  [");
        for (int j = 0; j < matrix->cols; ++j)
        {
            printf("%.2lf, ", matrix->content[i][j]);
        }
        printf("]\n");
    }
    puts("]");
    if (consumeAfter)
    {
        Deallocate(matrix);
    }
}

MATRIX CreateVectorRow(size_t rowSize)
{
    double** row = (double **) malloc(sizeof(double *) * rowSize);
    for (size_t i = 0; i < rowSize; ++i)
    {
        row[i] = (double *) malloc(sizeof(double));
        row[i][0] = DEFAULT_DOUBLE_VALUE;
    }
    
    MATRIX res;
    res.content = row;
    res.rows = 1;
    res.cols = rowSize;
    res.matrix_type = VECTOR_ROW;
    return res;
}

MATRIX CreateVectorCol(size_t colSize)
{
    double** row = (double **) malloc(sizeof(double *));
    row[0] = (double *) malloc(sizeof(double) * colSize);
    for (size_t i = 0; i < colSize; ++i)
    {
        row[0][i] = DEFAULT_DOUBLE_VALUE;
    }
    MATRIX m;
    m.rows = 1;
    m.cols = colSize;
    m.content = row;
    m.matrix_type = VECTOR_COL;
    return m;
}

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