import cv2
import argparse
import numpy as np


def resize(img: np.ndarray, width=None, height=None, interpolation=cv2.INTER_AREA, return_size=False):
    """
    Given both width and height, choose the smaller of the resulting sizes.
    :param img: opencv image
    :param width: width to change
    :param height: height to change
    :param interpolation: interpolation
    :return: opencv image
    """
    h, w = img.shape[:2]
    dims = []
    if height is not None:
        ratio = height / h
        dims.append((int(w * ratio), height))
    if width is not None:
        ratio = width / w
        dims.append((width, int(h * ratio)))
    if len(dims) == 2 and dims[0] > dims[1]:
        dims = dims[1:]
    if len(dims) == 0:
        return img if not return_size else (w, h)

    return cv2.resize(img, dims[0], interpolation=interpolation) if not return_size else dims[0]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--name", help="name")
    args = parser.parse_args()
    name = args.name

    c_img = f'{name}.c.png'
    cpp_img = f'{name}.cpp.png'

    img1 = cv2.imread(c_img)
    img2 = cv2.imread(cpp_img)

    height = max(img1.shape[0], img2.shape[0])
    img1 = resize(img1, height=height)
    img2 = resize(img2, height=height)

    hconcat = cv2.hconcat([img1, img2])

    concat_img = f'{name}.jpg'
    cv2.imwrite(concat_img, hconcat)


if __name__ == '__main__':
    main()
