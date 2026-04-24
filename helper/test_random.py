import random


print(random.random()) # 0 ~ 1
print(random.randint(-100, 100))
print(random.choice([1, 2, 3, 4, 5]))
print(random.sample([1, 2, 3, 4, 5], 3))
print(random.choices([1, 2, 3, 4, 5], k=3))
print(random.shuffle([1, 2, 3, 4, 5]))

s = "aadsdad"

s