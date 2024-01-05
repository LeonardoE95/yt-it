#!/usr/bin/env python3
 

def compute_ones_list(lines):
    # returns a list ones such that
    # 
    # ones[i] := number of 1s in i-th column with respect to binary
    # numbers in lines.
    
    bit_length = len(lines[0]) - 1 # NOTE: do not consider '\n'
    ones = [0] * bit_length

    for line in lines:
        for i in range(0, bit_length):
            if line[i] == '1':
                ones[i] += 1

    return ones

def compute_ones_by_column(lines, i):
    # returns a number which represents the number of 1s in the i-th
    # column of the given input lines.
    
    bit_length = len(lines[0]) - 1 # NOTE: do not consider '\n'
    ones = 0

    for line in lines:
        if line[i] == '1':
            ones += 1

    return ones    

def part_one():
    gamma_rate = ""
    epsilon_rate = ""
    
    with open("input.txt", "r") as f:
        lines = f.readlines()
        tot_lines = len(lines)
        bit_length = len(lines[0]) - 1 # NOTE: do not consider '\n'
        ones = compute_ones_list(lines)

        for i in range(0, bit_length):
            if ones[i] > int(tot_lines / 2):
                # there are more 1s than 0s in the i-th column
                gamma_rate += '1'
                epsilon_rate += '0'
            else:
                # there are more 0s than 1s in the i-th column
                gamma_rate += '0'
                epsilon_rate += '1'

        print(f"Result of part one: {int(gamma_rate, 2) * int(epsilon_rate, 2)}")

# ------


def filter_bits(lines, j, criteria):
    if len(lines) == 1:
        # base case
        return lines[0]
    else:
        # recursive case
        new_lines = []
        ones = compute_ones_by_column(lines, j)
        zeroes = len(lines) - ones

        for number in lines:

            if criteria == 'oxygen':
                if ones > zeroes and number[j] == '1':
                    new_lines.append(number)
                elif zeroes > ones and number[j] == '0':
                    new_lines.append(number)
                elif zeroes == ones and number[j] == '1':
                    new_lines.append(number)

            elif criteria == 'co2-scrubber':
                if zeroes < ones and number[j] == '0':
                    new_lines.append(number)
                elif ones < zeroes and number[j] == '1':
                    new_lines.append(number)
                elif zeroes == ones and number[j] == '0':
                    new_lines.append(number)                

        return filter_bits(new_lines, j+1, criteria)
    
def part_two():
    with open("input.txt", "r") as f:
        lines = f.readlines()
        oxygen_rating = filter_bits(lines, 0, 'oxygen')
        co2_rating = filter_bits(lines, 0, 'co2-scrubber')
        
        print(f"Result of part two: {int(oxygen_rating, 2) * int(co2_rating, 2)}")

# ------
    
if __name__ == "__main__":
    part_one()
    part_two()
