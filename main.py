
import image
import grid
import cv2
import AN
import numpy as np
mygrid=image.image_to_binary_grid('mynift2.png')


np.savetxt("binary_grid.txt", mygrid, fmt='%d', delimiter='')

start=[0,0]
goal=[60,60]
ans=AN.findpath(mygrid,start,goal)

for i in ans:
    mygrid[i[0]][i[1]]=2
img=grid.grid_to_image(mygrid)

cv2.imwrite("grid_image.png", img)

