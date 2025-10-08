/*
Name: Christian Parkinson 
Date: 10/01/25
Version: C++17
Instructions: 
Write a program, in the language of your preference, that computes roots for polynomials. The program should take as input a file which contains description of a polynomial in the following format:

n
a(n) a(n-1) a(n-2) ... a(2) a(1) b

where n is the degree of the polynomial, a(i)  is the coefficient of the monomial of degree i, and b is the constant term. For example, the polynomial:

3x^3 + 5x^2 - 7

would be represented as:

3
3  5  0 -7

The file should have extension .pol, for example, fun1.pol would be a suitable name for a file. The program should use bisection method by default and should place the solution in a file with the same name as the input, but with extension .sol (such as fun1.sol), with format:

root  iterations outcome

where root is the last root approximation, iterations is the total number of iterations performed by the algorithm, and outcome should be one of success if the algorithm reached convergence, or fail if the algorithm didn't converge.

For all algorithms, the error tolerance should be IEEE 754 single precision. You should implement the following algorithms:

Bisection method.
Newton's Method.
Secant Method.
Hybrid: starts with bisection for early iterations and switches to newton's.
The program should use bisection as default and operate as follows:

> polRoot [-newt, -sec, -hybrid] [-maxIt n] initP [initP2] polyFileName

By default the program uses bisection, but this can be modified with -newt for Newton's or -sec for Secant. The program should attempt 10,000 iterations by default, but this can be modified with -maxIter and the number of desired iterations. The initial point is provided (or an extra point for bisection and secant). For example, to run bisection method on file fun1.pol, with initial points 0 and 1:

> polRoot 0 1 fun1.pol

to run newton's with initial point 0:

> polRoot -newt 0 fun1.pol

and to run secant, with initial points 0 and 1, for 100,000 iterations:

> polRoot -sec -maxIter 100000 0 1 fun1.pol
*/

#include <iostream>
#include <iomanip>
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
#include <chrono>
#include <optional>
#include <functional>

using namespace std;

enum Method { BISECTION, NEWTON, SECANT, HYBRID };
static const string EnumStrings[] = { "Bisection", "Newton", "Secant", "Hybrid" };

class FindRoots { 
private:
    optional<float> initialPoint1 = nullopt;
    optional<float> initialPoint2 = nullopt;
    Method method;
    function<float(float)> polynomial;
    function<float(float)> derivative;
    string outcome = "fail";
    int iterations = 0;
    int maxIterations;
    float rootValue = 0;

    template<typename... coef>
    auto createPolynomial(coef... c) {
        return [=](float x) {
            float result = 0;
            ((result = result * x + c), ...);
            return result;
        };
    }

    template<typename... coef>
    auto createDerivative(coef... c) {
        constexpr size_t n = sizeof...(c) - 1;  // degree
        float coeffs[] = {c...};
        
        return [=](float x) {
            float result = 0;
            for (size_t i = 0; i < n; i++) {
                result = result * x + (n - i) * coeffs[i];
            }
            return result;
        };
    }

    float numerator(float x1, float x2) {
        if (method == BISECTION || method == HYBRID) {
            return (x1 + x2);
        } else if (method == SECANT) {
            float f1 = polynomial(x1);
            float f2 = polynomial(x2);
            return (x2 * (f2 - f1) - (x2 - x1) * f2);
        } else if (method == NEWTON) {
            float f2 = polynomial(x2);
            float d2 = derivative(x2);
            return (d2 * x2 - f2);
        }
    }

    float denominator(float x1, float x2) {
        if (method == BISECTION || method == HYBRID) {
            return 2;
        } else if (method == SECANT) {
            float f1 = polynomial(x1);
            float f2 = polynomial(x2);
            return (f2 - f1);
        } else if (method == NEWTON) {
            float d2 = derivative(x2);
            return d2;
        }
    }
    
    float findRoot(float x1, float x2) {
        while (iterations < maxIterations) {
            float num = numerator(x1, x2);
            float den = denominator(x1, x2);

            if (den == 0 || !isfinite(den)) {
                outcome = "fail";
                return x2;
            }

            float x3 = num / den;
            
            // Check for NaN or Inf
            if (!isfinite(x3)) {
                outcome = "fail";
                return x2;
            }

            // Check convergence based on method
            if (method == BISECTION || (method == HYBRID && iterations < 5)) {
                // For bisection: check if new midpoint equals either endpoint
                if (x3 == x1 || x3 == x2) {
                    outcome = "success";
                    return x3;
                }
                // Update interval for bisection
                if (polynomial(x1) * polynomial(x3) < 0) {
                    x2 = x3;
                } else {
                    x1 = x3;
                }
            } else {
                if (x3 == x2) {
                    outcome = "success";
                    return x3;
                }
                x1 = x2;
                x2 = x3;
            }

            // Switch to Newton for hybrid method
            if (method == HYBRID && iterations >= 5) {
                method = NEWTON;
            }

            iterations++;
        }
        
        // Max iterations reached
        outcome = "fail";
        return x2;
    }

public:
    template<typename... coef>
    FindRoots(enum Method m, int maxIter, float init1, optional<float> init2, coef... coeffs) {
        method = m;
        maxIterations = maxIter;
        initialPoint1 = init1;
        initialPoint2 = init2;

        if ((method == BISECTION || method == SECANT || method == HYBRID) 
            && !initialPoint2.has_value()) {
            throw runtime_error(EnumStrings[method] + " expected two initial points; received 1");
        }

        polynomial = createPolynomial(coeffs...);
        derivative = createDerivative(coeffs...);

        if (method == NEWTON) {
            rootValue = findRoot(init1, init1);
        } else {
            rootValue = findRoot(init1, init2.value());
        }
    }

    void writeOutput(const string& filename) {
        string outFilename = filename.substr(0, filename.find_last_of('.')) + ".sol";
        ofstream outFile(outFilename);
        if (!outFile.is_open()) {
            throw runtime_error("Could not create output file: " + outFilename);
        }

        outFile << fixed << setprecision(6);
        outFile << rootValue << "  " << iterations << "  " << outcome << endl;
        outFile.close();
    }

    float getRoot() const { return rootValue; }
    int getIterations() const { return iterations; }
    string getOutcome() const { return outcome; }
};

int main(int argc, char* argv[]) {
    try {
        if (argc < 3) {
            cerr << "Usage: polRoot [-newt|-sec|-hybrid] [-maxIter n] initP [initP2] polyFileName" << endl;
            return 1;
        }

        Method selectedMethod = BISECTION;
        int maxIter = 10000;
        vector<float> initPoints;
        string filename;

        for (int i = 1; i < argc; i++) {
            string arg = argv[i];
            
            if (arg == "-newt") {
                selectedMethod = NEWTON;
            } else if (arg == "-sec") {
                selectedMethod = SECANT;
            } else if (arg == "-hybrid") {
                selectedMethod = HYBRID;
            } else if (arg == "-maxIter") {
                if (i + 1 < argc) {
                    maxIter = stoi(argv[++i]);
                } else {
                    throw runtime_error("-maxIter requires a value");
                }
            } else if (arg[0] == '-' || isdigit(arg[0]) || arg[0] == '.') {
                initPoints.push_back(stof(arg));
            } else if (arg.find(".pol") != string::npos) {
                filename = arg;
            }
        }

        if (filename.empty()) {
            throw runtime_error("No polynomial file provided");
        }

        if (initPoints.empty()) {
            throw runtime_error("No initial points provided");
        }

        optional<float> init2 = nullopt;
        if (initPoints.size() > 1) {
            init2 = initPoints[1];
        }

        // INJECT_COEFFICIENTS - Python will replace this line with actual coefficient instantiation
        // Example: FindRoots solver(selectedMethod, maxIter, initPoints[0], init2, 3.0f, 5.0f, 0.0f, -7.0f);
        
        // solver.writeOutput(filename);
        
        // cout << "Results written to " << filename.substr(0, filename.find_last_of('.')) + ".sol" << endl;
        // cout << "Root: " << solver.getRoot() << endl;
        // cout << "Iterations: " << solver.getIterations() << endl;
        // cout << "Outcome: " << solver.getOutcome() << endl;

        return 0;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}