#!/usr/bin/env python3

'''This file contains a trivial implementation of Netwon Method
written in python3 for educational purposes.

Author: Leonardo Tamiano.
Date: 2021-08-15.

'''

import math

# --------------------------------
# Globals variables

PRECISION = 0.0000000000001

# --------------------------------
# General math procedures

def square(x):
    return x * x

def abs(x):
    if x >= 0:
        return x
    else:
        return -x

def average(x, y):
    return (x + y) / 2

# --------------------------------
# (basic) implementation of netwons method

def good_enough(approx, x):
    if abs(square(approx) - x) < PRECISION:
        return True
    else:
        return False
    
def improve_approximation(approx, x):
    return average(approx, x / approx)

def square_root(x):
    approx = 1

    while not good_enough(approx, x):
        approx = improve_approximation(approx, x)

    return approx

# --------------------------------

def test():
    numbers_to_test = list(range(1, 100))

    for number in numbers_to_test:
        # -- testing number
        our_square_root = square_root(number)
        python_square_root = math.sqrt(number)

        difference = abs(our_square_root - python_square_root)
        
        if difference > PRECISION:
            print(f"[ERROR]: number={number}, our_square_root={our_square_root}, python_square_root={python_square_root}, difference={difference}")
        else:
            print(f"[INFO]: number={number}, our_square_root={our_square_root}, python_square_root={python_square_root}, difference={difference}")
            
if __name__ == "__main__":
    n = 2
    print(f"La radice di {n} è {square_root(n)}")
