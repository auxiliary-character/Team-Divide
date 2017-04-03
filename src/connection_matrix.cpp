#include "connection_matrix.h"

ConnectionMatrix::ConnectionMatrix(unsigned int rows) : matrix(rows * rows), rows(rows) {}

bool ConnectionMatrix::get(unsigned int x, unsigned int y) const{
    return matrix[x*rows + y];
}

void ConnectionMatrix::set(unsigned int x, unsigned int y, bool value){
    matrix[x*rows + y] = matrix[x + y*rows] = value;
}
