#!/usr/bin/env python3

import math
import sys

# Definizione ricorsiva successione di Fibonacci
# 
# F_1 = F_2 = 1                         (caso base)
# F_n = F_{n-1} + F_{n-2}, n >= 3       (definizione ricorsiva)
#

# ----------------------------------------

def fibo_recursive(n):
    if n == 1 or n == 2:
        return 1
    elif n > 2:
        return fibo_recursive(n-1) + fibo_recursive(n-2)
    else:
        # caso n <=0
        print(f"[ERROR]: n deve essere > 0 in fibo_recursive (n={n})")
        exit(1)

# ----------------------------------------

def fibo_recursive_with_cache(n, cache=None):
    if cache is None:
        return fibo_recursive_with_cache(n, {1:1, 2:1})
    else:
        # cache is already initialized
        if n not in cache:
            cache[n] = fibo_recursive_with_cache(n-1, cache) + fibo_recursive_with_cache(n-2, cache)
        return cache[n]

# ----------------------------------------

def fibo_iterative(n):
    a = 1 # F_1
    b = 1 # F_2

    for i in range(0, n-1):
        t = b
        b = a + b
        a = t
        
    return a

# ----------------------------------------

# Qua sto utilizzando la formula chiusa per i numeri di Fibonacci
#
def fibo_approximation(n):
    phi_1 = (1 + math.sqrt(5)) / 2
    phi_2 = (1 - math.sqrt(5)) / 2
    return 1/math.sqrt(5) * (phi_1 ** n - phi_2 ** n)

# ----------------------------------------

def matrix_dot_prod(A, B):
    assert len(A) > 0 and len(B) > 0
    
    n_A, m_A = len(A), len(A[0])
    n_B, m_B = len(B), len(B[0])

    assert m_A == n_B, f"Cannot multiply {n_A}x{m_A} with {n_B}x{m_B} matrices together"

    result = [0] * n_A
    for row in range(0, n_A):
        column = [0] * m_B
        for col in range(0, m_B):
            res = 0
            for i in range(0, m_A):
                res += A[row][i] * B[i][col]
            column[col] = res
        result[row] = column
    return result

def matrix_exp(A, n):
    if n <= 1:
        return A
    else:
        M = matrix_exp(A, n // 2)
        M = matrix_dot_prod(M, M)
        if n % 2 == 1:
            M = matrix_dot_prod(M, A)
        return M

def fibo_linear(n):
    A = [[1, 1], [1, 0]]
    M = matrix_exp(A, n)
    return M[0][1]

# ----------------------------------------

def read_fib_sequence(filename):
    with open(filename, "r") as f:
        return list(map(int, f.read().split("\n\n")))

def test_impl(fibo_seq, impl):
    print(f"About to test: {impl.__name__}()")

    failed = False
    fail_case = None
    exception = None
    
    try:
        for i in range(0, len(fibo_seq)):
            expected = fibo_seq[i]
            computed = impl(i + 1)

            if not computed == expected:
                failed = True
                fail_case = (i, computed, expected)
                break

    except Exception as e:
        failed = True
        exception = e

    if failed:
        if fail_case:
            (n, computed, expected) = fail_case
            print(f"  [ERROR]: n = {n}, (computed = {computed}) != ({expected} = expected)")

        elif exception:
            print(f"  [ERROR]: Exception = {exception}")
    else:
        print(f"  [OK]: all cases passed correctly!")


def test():
    TEST_DATA = "./fibonacci.txt"

    fibo_seq = read_fib_sequence(TEST_DATA)

    fibo_impl = [
        fibo_approximation,
        fibo_recursive_with_cache,
        fibo_iterative,
        fibo_linear,
        fibo_recursive
    ]

    for f in fibo_impl:
        test_impl(fibo_seq, f)

    exit(0)
        
# ----------------------------------------

def main():
    best_impl = fibo_linear

    if len(sys.argv) != 2:
        print(f"Usage: python3 {sys.argv[0]} [<n> | test]")
        exit(1)

    arg = sys.argv[1]

    if arg == "test":
        test()
    else:

        # read and process input
        try:
            n = int(arg)
            if n < 0:
                print(f"[ERROR]: input ({arg}) must be a positive number!")
                exit(1)
        except ValueError as e:
                print(f"[ERROR]: input ({arg}) must be a positive number!")
                exit(1)

        # compute F_n
        try:
            print(f"F_{n} = {best_impl(n)}")
        except Exception as e:
                print(f"[ERROR]: Could not compute F_{n}")
                print(e)
                exit(1)            
        
if __name__ == "__main__":
    main()
