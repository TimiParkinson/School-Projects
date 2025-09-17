/*
Name: Christian Parkinson 
Date: 09/16/25
Instructions: 
    Create a program to compute the IEEE 754 single precision version of a given number, then use the program to compute the values. The program should behave like so:

    Upon start, the program prompts the user to input a number int he following format: a.b x 10^(e).
    The program then outputs the number in IEEE 754 single precision format like so: s e1e2e3e4e5e6e7e8 m1m2m3 ... m23
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <ctype.h>
#include <regex>
#include <tuple>


using namespace std;

class IEEE754Converter {
    private:
        bool binaryFloat[32];
        double coefficient;
        double baseTwoCoefficient;
        int exponent;
        int baseTwoExponent;
        bool isNegative;
        double value;

        void normalizeNotation() {
            // Normalize decimal coefficient to be in the range [1, 10)
            if (coefficient == 0) throw invalid_argument("Coefficient cannot be zero");

            double abs_coefficient = coefficient;
            int shift = floor(log10(abs_coefficient));
            coefficient /= pow(10, shift);
            exponent += shift;
        }
        bool* to_binary(int n, int precision) {
            // Convert integer n to binary representation as an array of booleans
            bool* binary = new bool[precision];
            if (n > 1) {
                for (int i = 0; i < precision; i++) {
                    binary[i] = n % 2;
                    n /= 2;
                }
            } else if (n < 0) {
                throw invalid_argument("n must be positive");
            }
            return binary;
        }
        bool* to_binary(double n, int precision) {
            // Convert double n to binary representation as an array of booleans
            bool* binary = new bool[precision];
            if (n > 1) {
                throw invalid_argument("n must be less than 1");
            } else if (n < 1) {
                for (int i = 0; i < precision; i++) {
                    n *= 2;
                    if (n >= 1) {
                        binary[i] = 1;
                        n -= 1;
                    } else {
                        binary[i] = 0;
                    }
                }
            }
            return binary;
        }

        auto parse(string str) {
            // Given string of the form a.b x 10^(e), parse into a, b, and e
            regex pattern(R"(([-+]?\d+)\.(\d+)\s*x\s*10\^(\(?[-+]?\d+\)?))");
            smatch matches;
            int a, b, e;
            if (regex_match(str, matches, pattern)) {
                a = stoi(matches[1]);
                b = stoi(matches[2]);
                e = stoi(matches[3]);
            } else {
                throw invalid_argument("Input string not in correct format");
            }
            return make_tuple(a, b, e);
        }
        void input(int a, int b, int e) {
            if (a < 0) {
                isNegative = true;
                a = abs(a);
            }

            coefficient = a + (b / pow(10, floor(log10(b)) + 1));
            normalizeNotation();
            exponent = e;
            value = coefficient * pow(10, exponent);
        }
        
        string findSign() {
            if (value == 0) {
                fill(begin(binaryFloat), end(binaryFloat), 0);
            }
            // Determine sign bit (0 for positive, 1 for negative)
            if (isNegative) {
                binaryFloat[0] = 1;
            } else {
                binaryFloat[0] = 0;
            }
            return string(binaryFloat, binaryFloat + 1);
        }
        string findExponent() {
            // Handle special case of zero
            if (value == 0) {
                fill(begin(binaryFloat), end(binaryFloat), 0);
            }
            // Calculate binary exponent
            int k = floor(log2(value));
            double floorDistance = abs(pow(2, k) - value);
            double ceilDistance = abs(pow(2, k + 1) - value);
            //The closer of the two powers of 2 to the value determines the exponent
            baseTwoExponent = (floorDistance < ceilDistance) ? k : k + 1;
            //Bias the exponent by adding 127
            baseTwoExponent += 127;
            //Convert to binary and store in the appropriate bits
            bool* temp = to_binary(baseTwoExponent, 8);
            copy(temp, temp + 8, binaryFloat + 1);

            return string(binaryFloat + 1, binaryFloat + 8);
        }
        string findMantissa() {
            if (value == 0) {
                fill(begin(binaryFloat), end(binaryFloat), 0);
            }
            //Calculate new coefficient for base 2
            baseTwoCoefficient = value / pow(2, baseTwoExponent);
            if (baseTwoCoefficient < 1 || baseTwoCoefficient >= 2) {
                throw invalid_argument("Base 2 coefficient out of range");
            }
            baseTwoCoefficient -= 1; //Remove leading 1
            //Convert to binary and store in the appropriate bits
            bool* temp = to_binary(baseTwoCoefficient, 23);
            copy(temp, temp + 23, binaryFloat + 9);

            return string(binaryFloat + 9, binaryFloat + 32);
        }
    public:
        IEEE754Converter() : coefficient(0), exponent(0), isNegative(false), value(0) {
            fill(begin(binaryFloat), end(binaryFloat), 0);
        }
        
        void convert(int a, int b, int e) {
            input(a, b, e);
            findSign();
            findExponent();
            findMantissa();
        }
        void convert(string str) {
            auto [a, b, e] = parse(str);  // unpack tuple
            convert(a, b, e);
        }
        string output() {
            string output = "";
            output += findSign() + " ";
            output += findExponent() + " ";
            output += findMantissa();
            return output;
        }
};


int main() {
    IEEE754Converter converter;
    cout << "---------------------------------------" << endl;
    cout << "| IEEE 754 Single Precision Converter |" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Would you like to enter parameters manually (y/n)? ";
    char choice;
    cin >> choice;
    if (tolower(choice) == 'y') {
        int a, b, e;
        cout << "Enter integer part, a: ";
        cin >> a;
        cout << "Enter fractional part, b (do not enter as a decimal): ";
        cin >> b;
        cout << "Enter exponent, e: ";
        cin >> e;
        converter.convert(a, b, e);
        cout << "IEEE 754 representation: " << converter.output() << endl;
    } else if (choice != 'n' && choice != 'N') {
        string input;
        cout << "Enter a number in the format a.b x 10^(e): ";
        cin >> input;
        converter.convert(input);
        cout << "IEEE 754 representation: " << converter.output() << endl;
    } 
}