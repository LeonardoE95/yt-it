#!/usr/bin/python3

def check_primeness(n):
    result = True
    for j in range(2, n):
        if n % j == 0:
            result = False
    return result


list_of_numbers = list(range(2, 10000))
# print(list_of_numbers)

list_of_prime_numbers = []
for elem in list_of_numbers:
    if check_primeness(elem) is True:
        list_of_prime_numbers.append(elem)

# print(list_of_prime_numbers)
print(len(list_of_prime_numbers))
