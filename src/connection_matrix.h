#ifndef CONNECTION_MATRIX_H
#define CONNECTION_MATRIX_H
#include <vector>
struct ConnectionMatrix{
    std::vector<bool> matrix;
    unsigned int rows;

    ConnectionMatrix(unsigned int rows);
    bool get(unsigned int x, unsigned int y) const;
    void set(unsigned int x, unsigned int y, bool value);
};
#endif
