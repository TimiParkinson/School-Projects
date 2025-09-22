//
// Created by benja on 9/21/2025.
//

#ifndef MATRIXSOLVER_MATRIX_H
#define MATRIXSOLVER_MATRIX_H

#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

template <typename T>
class Eliminator;

template <typename T>
class Matrix {
    int n;
    std::string filename;
    std::vector<std::vector<T>> matrix;
public:

    friend class Eliminator<T>;
    explicit Matrix(const std::string &find);
    void print() const;
    void printSolution(const std::vector<int> &indices) const;
    void publish(const std::vector<int> &indices) const;
    [[nodiscard]] int getN() const;
};



#include "Matrix.cpp"

#endif //MATRIXSOLVER_MATRIX_H
