#include<iostream>
#include<vector>

int main() {
    std::vector<std::vector<int>> matrix;
    
    matrix.assign(10, std::vector<int>(10));
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = i * j;
        }
    }
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            std::cout << "[" << matrix[i][j] << "]";
        }
        std::cout << std::endl;
    }
}