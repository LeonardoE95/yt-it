#!/usr/bin/env python3

# Author: Leonardo Tamiano
# Based on: http://secgroup.dais.unive.it/wp-content/uploads/2012/11/Practical-Padding-Oracle-Attacks-on-RSA.html#eq1
#
# Description: This script showcases an offline implementation of the
# bleichenbacher's oracle.  For more information I suggest you to look
# at the following video
#

from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_v1_5
from Crypto.Util import asn1
from base64 import b64decode
import pickle
import socket
import binascii
import sys
from sympy import nextprime, prevprime, randprime

from secret import FLAG

# global variables

VALID_SIZES = ["128", "256", "512", "1024", "2048", "3072", "4096"]

DEFAULT_KEY_BIT_SIZE = 1024
KEY_BYTE_SIZE = 0
PADDING_VALUE = 0

B, B2, B3 = 0, 0, 0

E, D, N = 0, 0, 0
ENCRYPTED_FLAG = ""

TOTAL_REQUESTS = 0

# -------------------------------------------

def ceil(x, y):
    return x // y + (x % y != 0)

def floor(x, y):
    return x // y

# -------------------------------------------

def oracle(msg):
    global D, N

    encrypted_msg = int("0x" + msg, 0)

    # raw decrypt using RSA
    decrypted_msg = pow(encrypted_msg, D, N)
    str_hex = f"%0{PADDING_VALUE}x" % decrypted_msg

    # check for padding
    if str_hex[0:4] != "0002":
        return False
    else:
        return True

def bleichenbacher_step_1(s):
    global E, N, TOTAL_REQUESTS
    
    s = s + 1
    while True:
        new_ciphertext = (pow(s, E, N) * ENCRYPTED_FLAG) % N
        encrypted_hex = f"%0{PADDING_VALUE}x" % new_ciphertext
        if oracle(encrypted_hex) == True:
            return s
        s = s + 1
        TOTAL_REQUESTS += 1

def bleichenbacher_step_2(s, old_M):
    new_M = set([])
    
    for (a, b) in old_M:
        r1 = ceil((a * s - B3 + 1), N)
        r2 = floor((b * s - B2), N) + 1

        for r in range(r1, r2):
            aa = ceil(B2 + r*N, s)
            bb = floor(B3 - 1 + r*N, s)

            newa = max(a, aa)
            newb = min(b, bb)

            if newa <= newb:
                new_M |= set([ (newa, newb) ])

    return new_M

def bleichenbacher_opt_1(s, old_M):
    global TOTAL_REQUESTS
        
    fst = old_M.pop()
    old_M.add(fst)
    a = fst[0]
    b = fst[1]
    r = ceil((b * s - B2) * 2, N)

    while True:
        for s in range(ceil((B2 + r * N), b), ceil((B3 - 1 + r * N), a) + 1):
            new_ciphertext = (pow(s, E, N) * ENCRYPTED_FLAG) % N
            str_hex = f"%0{PADDING_VALUE}x" % new_ciphertext
            
            if oracle(str_hex):
                return s

            TOTAL_REQUESTS += 1

        r += 1

# -------------------------------------------

if __name__ == "__main__":
    if len(sys.argv) == 1:
        print(f"Using default KEY_BIT_SIZE = {DEFAULT_KEY_BIT_SIZE}")
        KEY_BIT_SIZE = DEFAULT_KEY_BIT_SIZE
    elif len(sys.argv) >= 2:
        bit_size = sys.argv[1]
        if bit_size in VALID_SIZES:
            KEY_BIT_SIZE = int(bit_size)
        else:
            print(f"[ERROR]: Usage {sys.argv[0]} <key_bit_size>")

    KEY_BYTE_SIZE = KEY_BIT_SIZE // 8
    PADDING_VALUE = KEY_BIT_SIZE // 4
    B = 2 ** (8 * (KEY_BYTE_SIZE - 2))
    B2, B3 = 2*B, 3*B
    
    # generate RSA key of the given KEY_BIT_SIZE
    print(f"Generating RSA key of {KEY_BIT_SIZE} bits")

    # using sympy
    P = randprime(2 ** (KEY_BIT_SIZE // 2 - 1) , 2 ** (KEY_BIT_SIZE // 2))
    Q = randprime(2 ** (KEY_BIT_SIZE // 2 - 1) , 2 ** (KEY_BIT_SIZE // 2))
    N = P * Q
    
    PHI = (P-1)*(Q-1)
    E = 65537
    D = pow(E, -1, PHI)

    KEY = RSA.construct((N, E, D))

    print("Key generated: ")
    print(f"E = {E}")
    print(f"D = {D}")
    print(f"N = {N}")
    print(f"P = {P}")
    print(f"Q = {Q}")
    
    print("============================================================")

    # encrypt flag
    cipher_rsa = PKCS1_v1_5.new(KEY)
    ENCRYPTED_FLAG = cipher_rsa.encrypt(FLAG)
    ENCRYPTED_FLAG = int("0x" + ENCRYPTED_FLAG.hex(), 0)

    print(f"DECRYPTED_FLAG = {FLAG.hex()}")
    print(f"ENCRYPTED_FLAG = {ENCRYPTED_FLAG}")
    
    print("============================================================")
    
    TOTAL_REQUESTS = 0

    # start the attack to decrypt the flag
    s = ceil(N, B3)        
    M = set([ (B2, B3 - 1) ])
    phase = 0

    print(f"Phase: {phase}")
    print(f"Starting s: {s}")
    print(f"Starting M: {M}")
    print("============================================================")
    
    while True:
        if len(M) > 1 or TOTAL_REQUESTS == 0:
            s = bleichenbacher_step_1(s)
        else:
            interval = M.pop()

            if interval[0] == interval[1]:
                plaintext = hex(interval[0])
                j = plaintext[4:].find("00")
                msg = plaintext[j + 4 + 2:]
                done = True
                
                print(f"[TERMINATION] Found (padded) plaintext: {plaintext}")
                print(f"[TERMINATION] Total Requests: {TOTAL_REQUESTS}")
                print(f"[TERMINATION] Total phases: {phase}")

                res = plaintext.find("00" + FLAG.hex())
                if res != -1:
                    print(f"[SUCCESS] Actual (unpadded) plaintext: {plaintext[res + 1:]}")
                else:
                    print(f"[FAILURE] Retrieved (unpadded) plaintext: {plaintext[res + 1:]} does not match the expected output: {FLAG.hex()}")
                    
                print("============================================================")
                
                break

            else:
                M.add(interval)
                s = bleichenbacher_opt_1(s, M)

        M = bleichenbacher_step_2(s, M)
        
        print(f"Phase: {phase}")
        print(f"New s value: {s}")
        print(f"New M value: {M}")
        print(f"We have {len(M)} new intervals!")
        print("============================================================")
        
        if len(M) == 0:
            print("[ERROR]: Set M became empty!")
            break
        
        phase += 1
