/*
Name: Christian Parkinson 
Date: 09/19/25
Version: C++17
Instructions: 
    Write a program, in the language of your preference, that computes solutions for linear systems. The program should take as input a file which contains data for a linear system in the following format:

n
a11 a12 ... a1n
a21 a22 ... a2n
...
an1 an2 ... ann
b1   b2   ... bn

The file should have extension .lin, for example, sys1.lin could a suitable file name. The program should use Naive Gaussian Elimination by default and should place the solution in a file with the same name as the input, but with extension .sol and with the following format:

x1 x2 ... xn

Additionally, the user should be able to modify the programs behavior with optional flag --spp, in which case the program will use Scaled Partial Pivoting to produce the solution. For example, for a system placed in file sys1.lin, the user could run:

> gaussian sys1.lin

or,

> gaussian --spp sys1.lin

An additional flag: --double, will specify the precision used. By default, the program runs using single precision, but when the flag --double is used, it runs using double precision.

In the first case the program will use NGE, and in the second it will use SPP. In both cases, the solution will be placed in file sys1.sol.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <numeric>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <stdexcept>
#include <ctype.h>
#include <regex>
#include <tuple>


using namespace std;

template <typename T>
class GaussianElimination {
    private:
        int numVariables;
        vector<vector<T>> coefficientMatrix;
        vector<T> constants;
        vector<T> solutions;
        vector<T> scalingFactors;
        vector<int> index;
        string fileName;
        bool useSPP;

        void populateSystem() {
            // Implementation for reading the file and populating numVariables, coefficientMatrix, constants, solutions, scalingFactors, and index
            ifstream problemFile(fileName);
            if (!problemFile.is_open()) {
                throw runtime_error("Could not open file");
            } 

            string line;
            int i = 0;
            int n;
            vector<stringstream> tempMatrix;
            while(getline(problemFile, line)) {
                if (i == 0){
                    n = stoi(line);
                } else {
                    tempMatrix.emplace_back(stringstream(line));
                }
                i++;
            }

            problemFile.close();

            if (tempMatrix.size() != n + 1) {
                throw runtime_error("File format incorrect; number of rows does not match specified size");
            }
            numVariables = n;
            
            T val;
            coefficientMatrix.resize(n);
            constants.resize(n);
            for (i = 0; i < n+1; i++) {
                vector<T>& temp = (i == n) ? constants : coefficientMatrix[i];
                temp.resize(n);
                int tempCount = 0;
                while (tempMatrix[i] >> val) {
                    temp.emplace_back(val);
                    tempCount++;
                }
                if (tempCount != n) {
                    cout << temp.size() << endl;
                    throw runtime_error("File format incorrect; number of columns does not match specified size");
                }
            }
            solutions.resize(n, 0);
            scalingFactors.resize(n, 1);
            index.resize(n);
            iota(index.begin(), index.end(), 0); // Fill index with 0, 1, ..., n-1
        }
        void elimination() {
            for (int i = 0; i < numVariables - 1; i++) {
                if (useSPP) {
                    swap(index[scaledPartialPivoting(i)], index[i]);
                }
                for (int j = i + 1; j < numVariables; j++) {
                    T multiplier = coefficientMatrix[j][i] / coefficientMatrix[i][i];
                    for (int k = i; k < numVariables; k++) {
                        coefficientMatrix[j][k] -= multiplier * coefficientMatrix[i][k];
                    }
                    constants[j] -= multiplier * constants[i];  
                }
            }
        }
        void backSubstitution() {
            solutions[numVariables - 1] = constants[numVariables - 1] / coefficientMatrix[numVariables - 1][numVariables - 1];
            for (int i = numVariables - 2; i >= 0; i--) {
                T sum = constants[i];
                for (int j = i + 1; j < numVariables; j++) {
                    sum -= coefficientMatrix[i][j] * solutions[j];
                }
                solutions[i] = sum / coefficientMatrix[i][i];
            }
        }
        int scaledPartialPivoting(int i) {
            // check if scaling factors have been calculated
            //if (!end(scalingFactor) == find(begin(scalingFactor), end(scalingFactor), 1)) {
            if (i == 0) {
                for (int i = 1; i < numVariables; i++) {
                    T smax = 0;
                    for (int j = 0; j < numVariables; j++) {
                        smax = max(smax, abs(coefficientMatrix[i][j]));

                    }
                    scalingFactors[i] = smax;
                }
            }
            T rmax = 0;
            int pivotIndex = i;
            for (int j = i; j < numVariables; j++) {
                T r = abs(coefficientMatrix[index[j]][i]/scalingFactors[index[j]]);
                if (r > rmax) {
                    rmax = r;
                    pivotIndex = j;
                }
            }
            
            return pivotIndex;
        }

    public:
        GaussianElimination(string fileName, bool spp) : fileName(fileName), useSPP(spp) { populateSystem(); }

        void solve() {
            elimination();
            backSubstitution();
            // Implementation for writing the solution to a file
            ofstream solutionFile(fileName.substr(0, fileName.find_last_of('.')) + ".sol");
            if (!solutionFile.is_open()) {
                throw runtime_error("Could not open solution file for writing");
            }
            for (const T& sol : solutions) {
                solutionFile << sol << " ";
            }
            solutionFile.close();
        }
};

int main(int argc, char* argv[]) {
    char** begin = argv;
    char** end = argv + argc;
    bool spp = false;
    string fileName;
    char** pointerFileName = find_if(begin, end, [](const char* arg) {
        return regex_match(arg, regex(".*\\.lin$"));
    });
    if (pointerFileName == end) {
        cerr << "Error: No .lin file provided" << endl;
        return 1;
    } else {
        fileName = *pointerFileName;
    }
    if (find(begin, end, "--spp") != end) {
        spp = true;
    }
    try {
        if (find(begin, end, "--double") != end) {
            GaussianElimination<double> solver(fileName, spp);
            solver.solve();
        } else {
            GaussianElimination<float> solver(fileName, spp);
            solver.solve();
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}
