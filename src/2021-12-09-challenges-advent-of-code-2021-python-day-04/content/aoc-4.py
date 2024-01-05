#!/usr/bin/env python3

class Board:
    def __init__(self, board):
        # NOTE: assume board is given as a string
        self.board = board

    def mark(self, n):
        # mark 'n' within board
        if int(n) < 10:
            self.board = self.board.replace(f" {n} ", " X ")
            self.board = self.board.replace(f" {n}\n", " X\n")
        else:
            self.board = self.board.replace(f"{n}", " X")

    def check_win(self):
        # -- check rows
        rows = self.board.split("\n")
        for row in rows:
            if row.count("X") == 5:
                return 1              # we have won!

        # -- check columns
        columns = [[rows[j].split()[i] for j in range(0, 5)]
                   for i in range(0, 5)]
        for col in columns:
            if col.count("X") == 5:
                return 1

        return 0

    def compute_score(self, n):
        data = " ".join(self.board.split()).replace("X", "").split()
        res = 0
        for x in data:
            res += int(x)
        return res * int(n)    
        
# -----
            
def part_one():
    with open("input.txt", "r") as f:
        lines = f.read().split("\n\n")
        numbers_drawn = lines[0].split(",")
        number_of_boards = len(lines) - 1

        boards = [Board(lines[i + 1]) for i in range(0, number_of_boards)]
        
        for n in numbers_drawn:
            for board in boards:
                board.mark(n)
                if board.check_win():
                    print(f"Result of part one: {board.compute_score(n)}")
                    return
        
# ------    
    
def part_two():
    with open("input.txt", "r") as f:
        lines = f.read().split("\n\n")
        numbers_drawn = lines[0].split(",")
        number_of_boards = len(lines) - 1
        boards = [Board(lines[i + 1]) for i in range(0, number_of_boards)]

        boards_finished = set()
        
        for n in numbers_drawn:
            for j, board in enumerate(boards):
                board.mark(n)
                if board.check_win():
                    if j not in boards_finished:
                        boards_finished.add(j)
                        if len(boards_finished) == number_of_boards:
                            print(f"Result of part two: {board.compute_score(n)}")
                            return

# ------
    
if __name__ == "__main__":
    part_one()
    part_two()
