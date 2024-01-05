#!/usr/bin/env python3

def slow_exponentiation(a, b, n):
    assert b > 0 and n > 0, "both n and b must > 0 in mod exp!"

    result = 1
    for i in range(0, b):
        result = (result * a) % n
    return result

if __name__ == "__main__":
    result = slow_exponentiation(54555555555555, 545555512323, 1000000012301230123021302030213123123)
    print(result)
