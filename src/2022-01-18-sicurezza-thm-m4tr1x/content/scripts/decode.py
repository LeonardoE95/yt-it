#!/usr/bin/python

if __name__ == "__main__":
    encoded_msg = "1 16 5 18 13 21 20 1 20 9 15 14 15 6 15 14 12 25 20 8 5 5 14 7 12 9 19 8 12 5 20 20 5 18 19 23 9 12 12 15 16 5 14 20 8 5 12 15 3 11 19"
    decoded_msg = ""
    
    for x in encoded_msg.split():
        decoded_msg += chr(ord('a') + (int(x) - 1))
        
    print(decoded_msg)
