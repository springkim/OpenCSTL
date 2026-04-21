import os
from glob import glob

def remove_file(path: str):
    if os.path.exists(path):
        os.remove(path)

files = glob("*.exe")
for file in files:
    remove_file(file)

remove_file("a.out")
remove_file("a.exe")
remove_file("main.exe")
remove_file("test/a.out")
remove_file("perftest/a.out")
remove_file("assets/opencstl.h")
