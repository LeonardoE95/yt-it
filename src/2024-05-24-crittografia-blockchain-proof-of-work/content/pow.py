# Reference: https://github.com/bitcoinbook/bitcoinbook/tree/develop

import hashlib
import time
import traceback

from dataclasses import dataclass

# -----------------------------------------------------

MAX_NONCE = 2 ** 32 # Nonce is a 32-byte value
MAX_DIFFICULTY = 256

@dataclass
class BlockHeader:
    version:     int
    timestamp:   float
    target:      int
    nonce:       int
    previous:    str
    merkle_root: str

    def __str__(self):
        try:
            value = str(self.version) + str(self.timestamp) +\
                str(self.target) + str(self.nonce) +\
                self.previous + self.merkle_root
        except Exception:
            print("[ERROR]")
            print(traceback.format_exc())
            value = ""

        return value
            
# -----------------------------------------------------

def proof_of_work(block_header):    
    for nonce in range(MAX_NONCE):

        # compute hash with given nonce
        block_header.nonce = nonce
        value = str(block_header).encode()        
        hash_result = hashlib.sha256(value).hexdigest()

        # is the result valid?
        if int(hash_result, 16) < block_header.target:
            print(f"Success with nonce {nonce}")
            print(f"Hash is {hash_result}")
            return (hash_result, nonce)

    print(f"Failed after {nonce} (max_nonce) tries")
    return nonce

def main():
    new_block_header = BlockHeader(version=1, timestamp=time.time(), target=1, nonce=0,
                                   previous="0x5891b5b522d5df086d0ff0b110fbd9d21bb4fc7163af34d08286a2e846f6be03",
                                   merkle_root="0xe50ab0839519c9c2c7766e0cd40727c9ebbcdae90921025610da122007d3d924")

    for difficulty_bits in range(0, MAX_DIFFICULTY):
        print(f"[INFO] – Starting search for difficulty_bits = {difficulty_bits}")

        target = 2 ** (256 - difficulty_bits)
        new_block_header.target = target
        
        start_time = time.time()
        hash_result, nonce = proof_of_work(new_block_header)
        end_time = time.time()
        
        elapsed_time = end_time - start_time
        print(f"Elapsed Time: {elapsed_time:4f} seconds")
        if elapsed_time > 0:
            hash_power = float(int(nonce)/elapsed_time)
            print(f"Hashing Power: {hash_power} hashes per second")

# -----------------------------------------------------
            
if __name__ == "__main__":
    main()


