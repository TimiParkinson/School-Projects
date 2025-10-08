#!/usr/bin/env python3
import argparse
import subprocess
import sys
import os
import re

def read_polynomial_file(filename):
    """Read a .pol file and return degree and coefficients"""
    with open(filename, 'r') as f:
        degree = int(f.readline().strip())
        coeffs = list(map(float, f.readline().strip().split()))
    
    if len(coeffs) != degree + 1:
        raise ValueError(f"Expected {degree + 1} coefficients, got {len(coeffs)}")
    
    return degree, coeffs

def inject_coefficients(cpp_template, coeffs, method, max_iter, init_points, filename):
    """Inject coefficients into the C++ template"""
    
    # Convert coefficients to C++ float literals
    coeff_str = ", ".join([f"{c}f" for c in coeffs])
    
    # Build the FindRoots instantiation
    if len(init_points) == 1:
        init_str = f"{init_points[0]}f, nullopt"
    else:
        init_str = f"{init_points[0]}f, {init_points[1]}f"
    
    injection = f"""        FindRoots solver({method}, {max_iter}, {init_str}, {coeff_str});
        
        solver.writeOutput(filename);
        
        cout << "Results written to " << filename.substr(0, filename.find_last_of('.')) + ".sol" << endl;
        cout << "Root: " << solver.getRoot() << endl;
        cout << "Iterations: " << solver.getIterations() << endl;
        cout << "Outcome: " << solver.getOutcome() << endl;"""
    
    # Replace the injection marker
    modified_cpp = re.sub(
        r'// INJECT_COEFFICIENTS.*?// cout << "Outcome.*?endl;',
        injection,
        cpp_template,
        flags=re.DOTALL
    )
    
    return modified_cpp

def main():
    parser = argparse.ArgumentParser(
        description="Compile and run polRoot with coefficients from .pol file",
        usage="%(prog)s [-newt|-sec|-hybrid] [-maxIter n] initP [initP2] polyFileName"
    )
    
    args = sys.argv[1:]
    
    if not args:
        print("Usage: polRoot [-newt|-sec|-hybrid] [-maxIter n] initP [initP2] polyFileName")
        sys.exit(1)
    
    # Parse arguments
    method = "BISECTION"
    max_iter = 10000
    init_points = []
    pol_file = None
    
    i = 0
    while i < len(args):
        arg = args[i]
        
        if arg == "-newt":
            method = "NEWTON"
        elif arg == "-sec":
            method = "SECANT"
        elif arg == "-hybrid":
            method = "HYBRID"
        elif arg == "-maxIter":
            i += 1
            max_iter = int(args[i])
        elif arg.endswith(".pol"):
            pol_file = arg
        else:
            # Try to parse as float (initial point)
            try:
                init_points.append(float(arg))
            except ValueError:
                print(f"Warning: Unrecognized argument '{arg}'")
        
        i += 1
    
    if not pol_file:
        print("Error: No .pol file provided")
        sys.exit(1)
    
    if not init_points:
        print("Error: No initial points provided")
        sys.exit(1)
    
    # Validate initial points for method
    if method in ["BISECTION", "SECANT", "HYBRID"] and len(init_points) < 2:
        print(f"Error: {method} requires two initial points")
        sys.exit(1)
    
    # Read the polynomial file
    try:
        degree, coeffs = read_polynomial_file(pol_file)
    except FileNotFoundError:
        print(f"Error: File '{pol_file}' not found")
        sys.exit(1)
    except Exception as e:
        print(f"Error reading polynomial file: {e}")
        sys.exit(1)
    
    # Read the C++ template
    cpp_template_file = "find_roots_template.cpp"
    if not os.path.exists(cpp_template_file):
        print(f"Error: C++ template file '{cpp_template_file}' not found")
        sys.exit(1)
    
    with open(cpp_template_file, 'r') as f:
        cpp_template = f.read()
    
    # Inject coefficients
    modified_cpp = inject_coefficients(cpp_template, coeffs, method, max_iter, init_points, pol_file)
    
    # Write the modified C++ file
    cpp_output = "find_roots_generated.cpp"
    with open(cpp_output, 'w') as f:
        f.write(modified_cpp)
    
    # Compile
    binary = "./polRoot"
    print(f"Compiling {cpp_output}...")
    try:
        subprocess.run(
            ["g++", "-std=c++17", "-O2", cpp_output, "-o", binary],
            check=True,
            capture_output=True,
            text=True
        )
    except subprocess.CalledProcessError as e:
        print(f"Compilation failed:")
        print(e.stderr)
        sys.exit(1)
    
    # Run the compiled program
    print(f"Running polRoot...")
    try:
        result = subprocess.run([binary] + sys.argv[1:], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Execution failed with return code {e.returncode}")
        sys.exit(1)
    
    # Clean up generated files
    os.remove(cpp_output)
    os.remove(binary)

if __name__ == "__main__":
    main()