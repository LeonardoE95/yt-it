#!/usr/bin/env python3

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __repr__(self):
        return f"({self.x}, {self.y})"

# generates all points that go from p1 -> p2
def generate_points(p1, p2):
    # -- vertical line
    if p1.x == p2.x:
        if p1.y <= p2.y:
            return [Point(p1.x, y) for y in range(p1.y, p2.y + 1)]
        else:
            return [Point(p1.x, y) for y in range(p2.y, p1.y + 1)]

    # -- horizontal line
    elif p1.y == p2.y:
        if p1.x <= p2.x:
            return [Point(x, p1.y) for x in range(p1.x, p2.x + 1)]
        else:
            return [Point(x, p1.y) for x in range(p2.x, p1.x + 1)]

    # -- diagonal line (per dopo)
    else:
        if p1.x < p2.x and p1.y < p2.y:
            return [Point(p1.x + i, p1.y + i) for i in range(0, abs(p2.x - p1.x) + 1)]
        elif p1.x < p2.x and p1.y > p2.y:
            return [Point(p1.x + i, p1.y - i) for i in range(0, abs(p2.x - p1.x) + 1)]
        elif p1.x > p2.x and p1.y < p2.y:
            return [Point(p1.x - i, p1.y + i) for i in range(0, abs(p2.x - p1.x) + 1)]
        elif p1.x > p2.x and p1.y > p2.y:
            return [Point(p1.x - i, p1.y - i) for i in range(0, abs(p2.x - p1.x) + 1)]

def compute_score(points_freq):
    result = 0
    for v in points_freq.values():
        if v >= 2:
            result += 1
    return result

def part_one():
    # points_freq[(x, y)] := number of times a point (x, y) appears
    # when "decompressing" all points.    
    points_freq = {}
    with open("input.txt", "r") as f:
        lines = f.readlines()
        for line in lines:
            splitted_l = line.split(" -> ")
            p1, p2 = splitted_l[0], splitted_l[1]

            p1 = Point(int(p1.split(",")[0]), int(p1.split(",")[1]))
            p2 = Point(int(p2.split(",")[0]), int(p2.split(",")[1]))

            # Exclude diagonal cases
            if p1.x == p2.x or p1.y == p2.y:
                for p in generate_points(p1, p2):
                    points_freq[str(p)] = 1 if str(p) not in points_freq else points_freq[str(p)] + 1

        print(f"Result of part one: {compute_score(points_freq)}")

# ------    
    
def part_two():
    points_freq = {}
    with open("input.txt", "r") as f:
        lines = f.readlines()
        for line in lines:
            splitted_l = line.split(" -> ")
            p1, p2 = splitted_l[0], splitted_l[1]

            p1 = Point(int(p1.split(",")[0]), int(p1.split(",")[1]))
            p2 = Point(int(p2.split(",")[0]), int(p2.split(",")[1]))

            for p in generate_points(p1, p2):
                points_freq[str(p)] = 1 if str(p) not in points_freq else points_freq[str(p)] + 1

        print(f"Result of part two: {compute_score(points_freq)}")

# ------
    
if __name__ == "__main__":
    part_one()
    part_two()
