#!/usr/bin/env python3

def part_one():
    with open("input.txt", "r") as f:
        positions = [int(pos) for pos in f.readline().split(",")]

        max_pos = max(positions)
        min_pos = min(positions)
        
        min_fuel = -1

        for valid_pos in range(min_pos, max_pos + 1):
            # -- try to move all crabs to `valid_pos` and compute total cost
            fuel_spent = 0
            for pos in positions:
                fuel_spent += abs(pos - valid_pos)

            # -- update min_fuel if necessary
            if min_fuel == -1 or fuel_spent < min_fuel:
                min_fuel = fuel_spent

        print(f"Solution to part one: {min_fuel}")

# ------    
    
def part_two():
    with open("input.txt", "r") as f:
        positions = [int(pos) for pos in f.readline().split(",")]

        max_pos = max(positions)
        min_pos = min(positions)
        
        min_fuel = -1

        for valid_pos in range(min_pos, max_pos + 1):
            # -- try to move all crabs to `valid_pos` and compute total cost
            fuel_spent = 0
            for pos in positions:
                n = abs(pos - valid_pos)
                # add to fuel 1 + 2 + 3 + ... + n = n * (n + 1) / 2
                fuel_spent += int(n * (n + 1) / 2)

            # -- update min_fuel if necessary
            if min_fuel == -1 or fuel_spent < min_fuel:
                min_fuel = fuel_spent

        print(f"Solution to part one: {min_fuel}")    

# ------
    
if __name__ == "__main__":
    part_one()
    part_two()
