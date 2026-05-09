import os
from glob import glob


def remove_files(s):
    def remove_file(path: str):
        if os.path.exists(path):
            os.remove(path)

    files = glob(s)
    for file in files:
        remove_file(file)


remove_files("*.exe")
remove_files("*.out")
remove_files("*.pdb")
remove_files("*.tds")
remove_files("*.obj")