//
// Created by benja on 9/21/2025.
//

#ifndef MATRIXSOLVER_MATRIX_CPP
#define MATRIXSOLVER_MATRIX_CPP

#include "Matrix.h"


template<typename T>
Matrix<T>::Matrix(const std::string &find) {

    for (int i = 0; find[i] != '.' && i < find.length(); i++)
        filename.push_back(find[i]);
    // filename.push_back('\0');
    std::ifstream ifs(find);
    if (ifs.is_open()) {
        std::string line;
        std::getline(ifs, line);
        n = std::stoi(line);
        matrix.resize(n);
        for (int i = 0; i < n; i++)
            matrix[i].resize(n + 1);
        for (int i = 0; i < n; i++) {
            std::getline(ifs, line);
            std::stringstream ss(line);
            std::string temp;
            for (int j = 0; j < n; j++) {
                ss >> temp;
                matrix[i][j] = std::stod(temp);
            };
        };
        std::getline(ifs, line);
        std::stringstream ss(line);
        std::string temp;
        for (int i = 0; i < n; i++) {
            ss >> temp;
            matrix[i][n] = std::stod(temp);
        };
    };
    ifs.close();
};

template<typename T>
void Matrix<T>::print() const {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++)
            std::cout << matrix[i][j] << " ";
        std::cout << std::endl;
    };
}

template<typename T>
void Matrix<T>::printSolution(const std::vector<int> &indices) const {
    for (int i = 0; i < n; i++)
        std::cout << matrix[indices[i]][n] << std::endl;

}

template<typename T>
void Matrix<T>::publish(const std::vector<int> &indices) const {
    std::ofstream ofs(filename + ".sol");
    ofs << std::fixed << std::setprecision(60);
    if (ofs.is_open())
        for (int i = 0; i < n; i++)
            ofs << matrix[indices[i]][n] << ' ';
    ofs.close();
};

template<typename T>
int Matrix<T>::getN() const {
    return n;
};


#endif