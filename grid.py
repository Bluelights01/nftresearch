import numpy as np
import cv2

def grid_to_image(grid):
    h, w = grid.shape
    image = np.zeros((h, w, 3), dtype=np.uint8)  # RGB image

    # Map values to colors
    for i in range(h):
        for j in range(w):
            if grid[i, j] == 0:
                image[i, j] = [255, 255, 255]  # White
            elif grid[i, j] == 1:
                image[i, j] = [0, 0, 0]        # Black
            elif grid[i, j] == 2:
                image[i, j] = [0, 0, 255]      # Red (in BGR for OpenCV)

    return image


