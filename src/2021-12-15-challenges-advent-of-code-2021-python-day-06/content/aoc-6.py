#!/usr/bin/env python3

def part_one():
    population = [0] * 9

    with open("input.txt", "r") as f:
        # -- read input
        for n in f.read().split(","):
            population[int(n)] += 1

        days_gone = 0
        while days_gone < 80:
            new_population = [0] * 9
            
            new_fish = population[0]
            for i in range(1, 9):
                new_population[i - 1] = population[i]

            new_population[6] += new_fish
            new_population[8] += new_fish

            population = new_population
            days_gone += 1

        # -- count how many fish
        print(f"Result of part one: {sum(population)}")
    
# ------
    
def part_two():
    population = [0] * 9

    with open("input.txt", "r") as f:
        # -- read input
        for n in f.read().split(","):
            population[int(n)] += 1

        days_gone = 0
        while days_gone < 256:
            new_population = [0] * 9
            
            new_fish = population[0]
            for i in range(1, 9):
                new_population[i - 1] = population[i]

            new_population[6] += new_fish
            new_population[8] += new_fish

            population = new_population
            days_gone += 1

        # -- count how many fish
        print(f"Result of part one: {sum(population)}")    

# ------
    
if __name__ == "__main__":
    part_one()
    part_two()
