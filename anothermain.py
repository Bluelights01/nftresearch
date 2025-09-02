import temp
import image
import grid
import cv2
import n
import time
import numpy as np



mygrid=image.image_to_binary_grid('gemini.png')
np.savetxt("binary_grid.txt", mygrid, fmt='%d', delimiter='')

start=[0,0]
goal=[500,500]

start_time=time.time()
fgrid=n.SolveWithNFT(start,goal,mygrid)
end_time=time.time()

print(end_time-start_time)

img=grid.grid_to_image(fgrid)

cv2.imwrite("grid_image.png", img)