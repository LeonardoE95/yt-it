#!/usr/bin/env python3

def decrypt(c, key):
    p = [0] * len(c)
    for i in range(0, len(c)):
        p[i] = c[i] ^ key[i % len(key)]
    return p

def main():
    c1 = open("./c1", "rb").read()
    p2 = open("./p2", "rb").read()
    c2 = open("./c2", "rb").read()
    c3 = open("./c3", "rb").read()

    # recover key
    key = decrypt(c2, p2)

    # decrypt ciphertext
    p1 = bytes(decrypt(c1, key))
    p2 = bytes(p2)
    p3 = bytes(decrypt(c3, key))

    print(f"{p1=}")
    print(f"{p2=}")
    print(f"{p3=}")

if __name__ == "__main__":
    main()
