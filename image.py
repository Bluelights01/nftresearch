import cv2
import numpy as np

def image_to_binary_grid(image_path):
    
    img = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)

   
    img = cv2.resize(img,(512,512))

    
    _, binary = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)

    
    grid = np.where(binary == 255, 0, 1)

    return grid



         
