#!/usr/bin/env python3

# Solves: https://leetcode.com/problems/roman-to-integer/

def solve(s):
        roman2int = {
            "I": 1,
            "V": 5,
            "X": 10,
            "L": 50,
            "C": 100,
            "D": 500,
            "M": 1000
        }

        i = 0
        result = 0
        while i < len(s):
            # sottrazione
            if i + 1 < len(s) and roman2int[s[i]] < roman2int[s[i+1]]:
                result += roman2int[s[i+1]] - roman2int[s[i]]
                i += 2
            else:
                result += roman2int[s[i]]
                i += 1

        return result
    
if __name__ == "__main__":
    if solve("MCMXCIV") == 1994:
        print("OK")
    else:
        print("ERROR")
