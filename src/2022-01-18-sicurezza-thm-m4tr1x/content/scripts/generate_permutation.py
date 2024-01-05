#!/usr/bin/env python3

import itertools

if __name__ == "__main__":
    symbols = "ofqxvg"

    for x in itertools.permutations(symbols):
        print("".join(x))
    
