#include"opencstl.h"

int main() {
    VECTOR(int*) matrix = new_vector(int*);
    const size_t sz = 10;
    assign(matrix, sz);
    for (int i = 0; i < size(matrix); i++) {
        matrix[i] = new_vector(int);
        assign(matrix[i], sz);
    }

    for (int i = 0; i < size(matrix); i++) {
        for (int j = 0; j < size(matrix[i]); j++) {
            matrix[i][j] = i * j;
        }
    }
    for (int i = 0; i < size(matrix); i++) {
        for (int j = 0; j < size(matrix[i]); j++) {
            printf("[%3d]", matrix[i][j]);
        }
        puts("");
    }

    for (int i = 0; i < size(matrix); i++) {
        destroy(matrix[i]);
    }
    destroy(matrix);
}
