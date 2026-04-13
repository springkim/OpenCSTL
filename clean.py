import os


def remove_file(path: str):
    if os.path.exists(path):
        os.remove(path)


remove_file("a.out")
remove_file("a.exe")
remove_file("test/a.out")
remove_file("perftest/a.out")
remove_file("assets/opencstl.h")
