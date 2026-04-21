import os
os.environ["KMP_DUPLICATE_LIB_OK"] = "TRUE"
import cv2
import numpy as np
from paddleocr import PaddleOCR
from PIL import Image, ImageDraw, ImageFont

# PaddleOCR 객체 생성
ocr = PaddleOCR(use_angle_cls=True, lang='korean')

img = cv2.imread("../assets/blogo.png")
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
height,width = img.shape[:2]
y = int(height*0.1)
x = int(width*0.75)
w = int(width*0.25)
h = int(height*0.25)
crop = img[y:y+h, x:x+w]
result = ocr.predict(img)

print(result)
cv2.imshow("img", crop)
cv2.waitKey(0)
cv2.destroyAllWindows()