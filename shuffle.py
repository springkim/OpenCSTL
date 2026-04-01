import random
import os

with open("words.txt", "rt", encoding="ansi") as f:
    lines = f.read().strip().splitlines()

random.shuffle(lines)

with open("words_random.txt", "wt", encoding="ansi") as f:
    f.write("\n".join(lines))