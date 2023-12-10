from pathlib import Path
import os

DEBUG = 0
folder = os.path.dirname(os.path.realpath(__file__))
file = "\\sample.txt" if DEBUG else "\\part_one_input.txt"

# Read file in
with open(folder+file, 'r') as f:
    lines = f.readlines()  # f.read()

if DEBUG:
    print(lines)

answer = 0
for line in lines:
    if (DEBUG):
        print(line)
    first = 0
    second = 0
    for char in line:
        if char.isdigit():
            if first == 0:
                first = char
            second = char

    answer += int(first) * 10 + int(second)
print(f"Part One {answer} {54601 == answer}")

# PART TWO
convert = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]
answer = 0
for line in lines:
    line = line.replace('\n', "")
    if(DEBUG):
        print(line)
    
    # Replace all the words, ensure that any additonal words that could be made are still true
    # nineight for example nine9nineeight8eight.
    for pos, word in enumerate(convert):
        line = line.replace(word, word+str(pos+1)+word) # one1one
    if (DEBUG):
        print(f"{line}\n")

    # Same logic from part 1
    first = 0
    second = 0
    for char in line:
        if char.isdigit():
            if not first:
                first = char
            second = char

    answer += int(first) * 10 + int(second)
print(f"Part two {answer}  {54078 == answer}")