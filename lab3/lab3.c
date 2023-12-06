#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

Matrix createMatrix(int rows, int cols) {
    Matrix resultMatrix;
    resultMatrix.rows = rows;
    resultMatrix.cols = cols;
    resultMatrix.data = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        resultMatrix.data[i] = (double *)malloc(cols * sizeof(double));
    }
    return resultMatrix;
}

void destroyMatrix(Matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
}

Matrix resizeMatrix(Matrix *matrix, int newRows, int newCols) {
    Matrix resizedMatrix = createMatrix(newRows, newCols);
    int minRows = (matrix->rows < newRows) ? matrix->rows : newRows;
    int minCols = (matrix->cols < newCols) ? matrix->cols : newCols;

    for (int i = 0; i < minRows; i++) {
        for (int j = 0; j < minCols; j++) {
            resizedMatrix.data[i][j] = matrix->data[i][j];
        }
    }

    destroyMatrix(matrix);
    *matrix = resizedMatrix;

    return resizedMatrix;
}

int getRowCount(Matrix matrix) {
    return matrix.rows;
}

int getColCount(Matrix matrix) {
    return matrix.cols;
}

void setElement(Matrix *matrix, int row, int col, double value) {
    if (row >= 0 && row < matrix->rows && col >= 0 && col < matrix->cols) {
        matrix->data[row][col] = value;
    } else {
        printf("Error: Attempting to set a value outside the matrix.\n");
    }
}

double getElement(Matrix matrix, int row, int col) {
    if (row >= 0 && row < matrix.rows && col >= 0 && col < matrix.cols) {
        return matrix.data[row][col];
    } else {
        printf("Error: Attempting to get a value outside the matrix.\n");
        return 0.0;
    }
}

void printMatrix(Matrix matrix) {
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            printf("%.2f\t", matrix.data[i][j]);
        }
        printf("\n");
    }
}

void saveMatrixToFile(Matrix matrix, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%d %d\n", matrix.rows, matrix.cols);
        for (int i = 0; i < matrix.rows; i++) {
            for (int j = 0; j < matrix.cols; j++) {
                fprintf(file, "%.2f ", matrix.data[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
        printf("The matrix is saved to a file %s\n", filename);
    } else {
        printf("Error opening file for writing.\n");
    }
}

Matrix readMatrixFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        int rows, cols;
        fscanf(file, "%d %d", &rows, &cols);
        Matrix matrix = createMatrix(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                fscanf(file, "%lf", &matrix.data[i][j]);
            }
        }
        fclose(file);
        printf("The matrix is read from the file %s\n", filename);
        return matrix;
    } else {
        printf("Error opening file for reading.\n");
        return createMatrix(0, 0);
    }
}

Matrix addMatrices(Matrix matrixA, Matrix matrixB) {
    if (matrixA.rows == matrixB.rows && matrixA.cols == matrixB.cols) {
        Matrix resultMatrix = createMatrix(matrixA.rows, matrixA.cols);
        for (int i = 0; i < matrixA.rows; i++) {
            for (int j = 0; j < matrixA.cols; j++) {
                resultMatrix.data[i][j] = matrixA.data[i][j] + matrixB.data[i][j];
            }
        }
        return resultMatrix;
    } else {
        printf("Error: Unable to add matrices of different sizes.\n");
        return createMatrix(0, 0);
    }
}

Matrix subtractMatrices(Matrix matrixA, Matrix matrixB) {
    if (matrixA.rows == matrixB.rows && matrixA.cols == matrixB.cols) {
        Matrix resultMatrix = createMatrix(matrixA.rows, matrixA.cols);
        for (int i = 0; i < matrixA.rows; i++) {
            for (int j = 0; j < matrixA.cols; j++) {
                resultMatrix.data[i][j] = matrixA.data[i][j] - matrixB.data[i][j];
            }
        }
        return resultMatrix;
    } else {
        printf("Error: Unable to subtract matrices of different sizes.\n");
        return createMatrix(0, 0);
    }
}

Matrix multiplyMatrixByScalar(Matrix matrix, double scalar) {
    Matrix resultMatrix = createMatrix(matrix.rows, matrix.cols);
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            resultMatrix.data[i][j] = matrix.data[i][j] * scalar;
        }
    }
    return resultMatrix;
}

Matrix multiplyMatrices(Matrix matrixA, Matrix matrixB) {
    if (matrixA.cols == matrixB.rows) {
        Matrix resultMatrix = createMatrix(matrixA.rows, matrixB.cols);
        for (int i = 0; i < matrixA.rows; i++) {
            for (int j = 0; j < matrixB.cols; j++) {
                resultMatrix.data[i][j] = 0;
                for (int k = 0; k < matrixA.cols; k++) {
                    resultMatrix.data[i][j] += matrixA.data[i][k] * matrixB.data[k][j];
                }
            }
        }
        return resultMatrix;
    } else {
        printf("Error: Unable to multiply matrices with these dimensions.\n");
        return createMatrix(0, 0);
    }
}

int main() {
    Matrix firstMatrix = createMatrix(3, 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            firstMatrix.data[i][j] = i * 3 + j + 1;
        }
    }

    printf("First Matrix:\n");
    printMatrix(firstMatrix);

    resizeMatrix(&firstMatrix, 2, 2);

    printf("\nFirst Matrix after resizing:\n");
    printMatrix(firstMatrix);

    Matrix secondMatrix = createMatrix(2, 2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            secondMatrix.data[i][j] = (i + 1) * 0.5 + (j + 1) * 0.25;
        }
    }

    printf("\nSecond Matrix:\n");
    printMatrix(secondMatrix);

    Matrix sumMatrix = addMatrices(firstMatrix, secondMatrix);
    printf("\nThe sum of matrices 1 and 2:\n");
    printMatrix(sumMatrix);

    Matrix scaledFirstMatrix = multiplyMatrixByScalar(firstMatrix, 2.0);
    printf("\nFirst Matrix multiplied by 2.0:\n");
    printMatrix(scaledFirstMatrix);

    Matrix productMatrix = multiplyMatrices(firstMatrix, secondMatrix);
    printf("\nProduct of matrices 1 and 2:\n");
    printMatrix(productMatrix);

    Matrix subtractedMatrix = subtractMatrices(firstMatrix, secondMatrix);
    printf("\nThe difference of matrices 1 and 2:\n");
    printMatrix(subtractedMatrix);

    saveMatrixToFile(firstMatrix, "matrix.txt");

    Matrix matrixFromFile = readMatrixFromFile("matrix.txt");

    printf("\nMatrix read from file:\n");
    printMatrix(matrixFromFile);

    destroyMatrix(&firstMatrix);
    destroyMatrix(&sumMatrix);
    destroyMatrix(&scaledFirstMatrix);
    destroyMatrix(&productMatrix);
    destroyMatrix(&subtractedMatrix);
    destroyMatrix(&matrixFromFile);

    printf("\nFunction check:\n");

    printf("Second Matrix:\n");
    printMatrix(secondMatrix);

    printf("\nThe number of rows of the matrix is 2: %d\n", getRowCount(secondMatrix));
    printf("Number of columns of second matrix: %d\n", getColCount(secondMatrix));

    setElement(&secondMatrix, 0, 0, 10.0);
    printf("Element (0,0) of second matrix: %.2f\n", getElement(secondMatrix, 0, 0));

    printf("\nSecond Matrix:\n");
    printMatrix(secondMatrix);

    destroyMatrix(&secondMatrix);

    return 0;
}

