#!/usr/bin/env python3

def part_one():
    result = 0
    with open("input.txt", "r") as f:
        lines = f.readlines()
        last_val = int(lines[0])

        for line in lines[1:]:
            curr_val = int(line)

            if last_val < curr_val:
                # got an increment
                result += 1

            last_val = curr_val

        print(f"Result of part one: {result}")

# ---------------------        

def part_two():
    result = 0

    with open("input.txt" ,"r") as f:
        lines = f.readlines()
        length = len(lines)
        i = 0

        last_val = -1
        if length < 3:
            print("[INFO] - Could not compute starting value\n")
            return
        else:
            last_val = int(lines[0]) + int(lines[1]) + int(lines[2])

        for i in range(1, length):
            curr_val = -1

            if i + 2 < length:
                curr_val = int(lines[i]) + int(lines[i + 1]) + int(lines[i + 2])
            else:
                break

            if last_val < curr_val:
                result += 1

            last_val = curr_val
            
        print(f"Result of part two: {result}")

# ------

if __name__ == "__main__":
    part_one()
    part_two()
