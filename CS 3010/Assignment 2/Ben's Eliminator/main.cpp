#include <iomanip>
#include <iostream>

#include "Eliminator.h"
#include "Matrix.h"


int main(int argc, char* argv[]) {

    bool prec = false, spp = false;
    std::cout << std::fixed << std::setprecision(60);
    for (int i = 1; i < argc; i++) {
        if (static_cast<std::string>(argv[i]) == "--double")
            prec = true;
        else if (static_cast<std::string>(argv[i]) == "--spp")
            spp = true;
    };

    if (prec) {
        Matrix<double>matrix(argv[argc - 1]);
        std::vector<int> indices;
        indices.resize(matrix.getN());
        Eliminator<double>::solve(matrix, indices, spp);
        matrix.printSolution(indices);
    }
    else {
        Matrix<float>matrix(argv[argc - 1]);
        std::vector<int> indices;
        indices.resize(matrix.getN());
        Eliminator<float>::solve(matrix, indices, spp);
        matrix.publish(indices);
    };

    return 0;

};