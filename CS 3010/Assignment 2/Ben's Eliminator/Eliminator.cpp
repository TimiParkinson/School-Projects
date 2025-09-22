//
// Created by benja on 9/21/2025.
//
#ifndef MATRIXSOLVER_ELIMINATOR_CPP
#define MATRIXSOLVER_ELIMINATOR_CPP



#include "Eliminator.h"

#include "Matrix.h"


template<typename T>
void Eliminator<T>::solve(Matrix<T> &matrix, std::vector<int> &indices, bool spp) {
    for (int i = 0; i < matrix.n; i++)
        indices[i] = i;
    forwardElimination(matrix, indices, spp);
    backSubstitution(matrix, indices);
};

template<typename T>
void Eliminator<T>::forwardElimination(Matrix<T> &matrix, std::vector<int> &indices, const bool spp) {

    std::vector<T> scalers;

    if (spp) {
        scalers.resize(matrix.n);
        for (int i = 0; i < matrix.n; i++) {
            scalers[i] = 0;
            for (int j = 0; j < matrix.n; j++)
                scalers[i] = std::max(scalers[i], std::abs(matrix.matrix[i][j]));
        };
    };
    for (int k = 0; k < matrix.n - 1; k++) {
        if (spp) {
            T rmax = 0;
            int maxIndex = k;
            for (int i = k; i < matrix.n; i++) {
                const T r = std::abs(matrix.matrix[indices[i]][k] / scalers[indices[i]]);
                if (r > rmax) {
                    rmax = r;
                    maxIndex = i;
                };
            };
            std::swap(indices[k], indices[maxIndex]);
        };
        for (int i = k + 1; i < matrix.n; i++) {
            const T mult = matrix.matrix[indices[i]][k] / matrix.matrix[indices[k]][k];
            for (int j = k; j <= matrix.n; j++)
                matrix.matrix[indices[i]][j] -= mult * matrix.matrix[indices[k]][j];
        };
    };

};

template<typename T>
void Eliminator<T>::backSubstitution(Matrix<T> &matrix, std::vector<int> &indices) {
    for (int i = matrix.n - 1; i >= 0; --i) {
        for (int j = i + 1; j < matrix.n; ++j)
            matrix.matrix[indices[i]][matrix.n] -= matrix.matrix[indices[i]][j] * matrix.matrix[indices[j]][matrix.n];
        matrix.matrix[indices[i]][matrix.n] /= matrix.matrix[indices[i]][i];
    };
};

// template<typename T>
// void Eliminator<T>::publish(const std::vector<T> &solutions, const std::string &filename) {
//
// };


#endif //MATRIXSOLVER_ELIMINATOR_CPP
