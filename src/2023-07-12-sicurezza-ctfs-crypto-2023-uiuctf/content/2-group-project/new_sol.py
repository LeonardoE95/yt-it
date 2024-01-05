#!/usr/bin/env python3

from Crypto.Util.number import getPrime, long_to_bytes
from random import randint
import hashlib
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad

# S = Bk^a mod p
#   = (B^k mod p)^a mod p
#   = (B^k)^a mod p
#   = B^(k * a) mod p
#   = (g^b mod p)^(k * a) mod p
#   = (g^b)^(k * a) mod p
#   = g^(a * b * k) mod p

def main():
    S = 1
    c = long_to_bytes(229787086409168537247108367753977616869)
    
    key = hashlib.md5(long_to_bytes(S)).digest()
    cipher = AES.new(key, AES.MODE_ECB)
    p = unpad(cipher.decrypt(c), 16)
    
    print(p)

if __name__ == "__main__":
    main()
