#!/usr/bin/env python3

import sympy

def main():
    primes = list(sympy.primerange(0, 15))

    print(primes)
    
    euclidean_number = 1
    for i in range(0, len(primes)):
        euclidean_number = euclidean_number * primes[i]
    euclidean_number = euclidean_number + 1

    print(f"{euclidean_number=}")
    

if __name__ == "__main__":
    main()
