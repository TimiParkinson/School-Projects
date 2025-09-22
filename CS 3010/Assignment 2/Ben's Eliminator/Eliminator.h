//
// Created by benja on 9/21/2025.
//

#ifndef MATRIXSOLVER_ELIMINATOR_H
#define MATRIXSOLVER_ELIMINATOR_H

#include <string>
#include <vector>
#include <fstream>

#include "Matrix.h"

template <typename T>
class Eliminator {
    public:
    static void solve(Matrix<T> &matrix, std::vector<int> &indices, bool spp);
    static void forwardElimination(Matrix<T> &matrix, std::vector<int> &indices, bool spp);
    static void backSubstitution(Matrix<T> &matrix, std::vector<int> &indices);
};


#include "Eliminator.cpp"
#endif //MATRIXSOLVER_ELIMINATOR_H
