import cv2
import numpy as np
from glob import glob

files = glob("./*.png")

for file in files:
    img = cv2.imread(file)
    cv2.imwrite(file.replace(".png", ".webp"), img)
