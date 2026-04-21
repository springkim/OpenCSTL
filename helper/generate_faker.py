from faker import Faker
from tqdm import tqdm
import random
gen = Faker()

names= set()

for i in tqdm(range(1000000)):
    name = gen.name()
    names.add(name)

gen2 = Faker("ko_KR")

for i in tqdm(range(100000)):
    name = gen2.name()
    names.add(name)

names2 = [name for name in names]
random.shuffle(names2)
with open("names.txt", "w", encoding="utf-8") as f:
    for name in names2:
        f.write(name + "\n")