/*
Name: Christian Parkinson 
Date: 09/16/25
Version: C++17
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
            if (n >= 1) {
                for (int i = 0; i < precision; i++) {
                    binary[precision - i - 1] = n % 2;
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
            } else if (n <= 1) {
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
        string bitsToString(const bool* arr, int len) {
            string s;
            s.reserve(len);
            for (int i = 0; i < len; i++) {
                s.push_back(arr[i] ? '1' : '0');
            }
            return s;
        }
        auto parse(string str) {
            // Given string of the form a.b x 10^(e), parse into a, b, and e
            regex pattern(R"(([-+]?\d+)\.(\d+)\s*x\s*10\^\(?([-+]?\d+)\)?)");
            smatch matches;
            string a, b, e;
            if (regex_search(str, matches, pattern)) {
                a = matches[1];
                b = matches[2];
                e = matches[3];
            } else {
                throw invalid_argument("Input string not in correct format");
            }
            return make_tuple(a, b, e);
        }
        void input(string a, string b, string e) {
            //Keep leading 0s
            int a_int = stoi(a);
            double b_double = 0;
            for (size_t i = 0; i < b.size(); i++) {
                if (!isdigit(b[i])) throw invalid_argument("Fractional part contains non-digit");
                b_double += (b[i] - '0') * pow(10, -(int)(i + 1));
            }            
            int e_int = stoi(e);
            
            if (a_int < 0) {
                isNegative = true;
                a_int = abs(a_int);
            }
            
            //debugging
            /*cout << "a: " << a_int << endl;
            cout << "b: " << b_double << endl;
            cout << "e: " << e_int << endl;*/
            
            coefficient = a_int + b_double;
            //normalizeNotation();
            exponent += e_int;
            value = coefficient * pow(10, exponent);
            
            //debugging
            /*cout << "isNegative: " << isNegative << endl;
            cout << "coefficient: " << coefficient << endl;
            cout << "exponent: " << exponent << endl;
            cout << "value: " << value << endl;*/
        }
        
        void findSign() {
            if (value == 0) {
                fill(begin(binaryFloat), end(binaryFloat), 0);
            }
            // Determine sign bit (0 for positive, 1 for negative)
            if (isNegative) {
                binaryFloat[0] = 1;
            } else {
                binaryFloat[0] = 0;
            }
        }
        void findExponent() {
            // Handle special case of zero
            if (value == 0) {
                fill(begin(binaryFloat), end(binaryFloat), 0);
            }
            // Calculate binary exponent
            baseTwoExponent = floor(log2(abs(value)));
            //debugging
            //cout << "baseTwoExponent: " << baseTwoExponent << endl;
            //Bias the exponent by adding 127
            baseTwoExponent += 127;
            //Convert to binary and store in the appropriate bits
            bool* temp = to_binary(baseTwoExponent, 8);
            copy(temp, temp + 8, binaryFloat + 1);
        }
        void findMantissa() {
            if (value == 0) {
                fill(begin(binaryFloat), end(binaryFloat), 0);
            }
            //Calculate new coefficient for base 2
            baseTwoCoefficient = value / pow(2, baseTwoExponent-127);
            if (baseTwoCoefficient < 1 || baseTwoCoefficient >= 2) {
                throw invalid_argument("Base 2 coefficient out of range");
            }
            //debugging
            //cout << "baseTwoCoefficient: " << baseTwoCoefficient << endl;
            baseTwoCoefficient -= 1; //Remove leading 1
            //Convert to binary and store in the appropriate bits
            bool* temp = to_binary(baseTwoCoefficient, 23);
            copy(temp, temp + 23, binaryFloat + 9);

        }
    public:
        IEEE754Converter() : coefficient(0), baseTwoCoefficient(0), exponent(0),  baseTwoExponent(0), isNegative(false), value(0) {
            fill(begin(binaryFloat), end(binaryFloat), 0);
        }
        
        void convert(string a, string b, string e) {
            coefficient = 0;
            baseTwoCoefficient = 0;
            exponent = 0;
            baseTwoExponent = 0;
            isNegative = false;
            value = 0;
            fill(begin(binaryFloat), end(binaryFloat), 0);
            
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
            output += bitsToString(binaryFloat, 1) + " ";
            output += bitsToString(binaryFloat + 1, 8) + " ";
            output += bitsToString(binaryFloat + 9, 23);
            return output;
        }
};


int main(int argc, char* argv[]) {
    IEEE754Converter converter;
    bool running = true;
    while (running) {
        cout << "---------------------------------------" << endl;
        cout << "| IEEE 754 Single Precision Converter |" << endl;
        cout << "---------------------------------------" << endl;
        cout << "Would you like to enter parameters manually (y/n)? ";
        char choice;
        cin >> choice;
        if (tolower(choice) == 'y') {
            string a, b, e;
            cout << "Enter integer part, a: ";
            cin >> a;
            cout << "Enter fractional part, b (do not enter as a decimal): ";
            cin >> b;
            cout << "Enter exponent, e: ";
            cin >> e;
            try {
                converter.convert(a, b, e);
                cout << "IEEE 754 representation: " << converter.output() << endl;
            } catch (const exception& ex) {
                cout << "Error: " << ex.what() << endl;
            }
        } else if (tolower(choice) == 'n') {
            string input;
            cout << "Enter a number in the format a.b x 10^(e): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            getline(cin, input);
            try {
                converter.convert(input);
                cout << "IEEE 754 representation: " << converter.output() << endl;
            } catch (const exception& ex) {
                cout << "Error: " << ex.what() << endl;
            }
        } 
        cout << "Would you like to convert another number (y/n)? ";
        cin >> choice;
        if (tolower(choice) == 'n') {
            running = false;
        }
    }
}
