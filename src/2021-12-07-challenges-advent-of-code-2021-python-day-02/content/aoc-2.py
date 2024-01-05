#!/usr/bin/env python3

def part_one():
    horizontal_pos = 0
    depth = 0
    
    with open("input.txt", "r") as f:
        for line in f.readlines():
            splitted_l = line.split()

            if len(splitted_l) == 2:
                val = int(splitted_l[1])
            else:
                print("[ERROR] - Invalid input")
                return
            
            if line.startswith("forward"):
                horizontal_pos += val
            elif line.startswith("up"):
                depth -= val
            elif line.startswith("down"):
                depth += val
            else:
                print("[ERROR] - Unkown instruction!")
                return

        print(f"Result of part one: {horizontal_pos * depth}")

# --------
        
def part_two():
    horizontal_pos = 0
    depth = 0
    aim = 0
    
    with open("input.txt", "r") as f:
        for line in f.readlines():
            splitted_l = line.split()

            if len(splitted_l) == 2:
                val = int(splitted_l[1])
            else:
                print("[ERROR] - Invalid input")
                return
            
            if line.startswith("forward"):
                horizontal_pos += val
                depth += aim * val
            elif line.startswith("up"):
                aim -= val
            elif line.startswith("down"):
                aim += val
            else:
                print("[ERROR] - Unkown instruction!")
                return

        print(f"Result of part one: {horizontal_pos * depth}")    
    
# --------

if __name__ == "__main__":
    part_one()
    part_two()
