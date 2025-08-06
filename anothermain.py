import temp
import image
import grid
import cv2
import n
import numpy as np


def bresenham_line(x0, y0, x1, y1,mygrid):
    dx = abs(x1 - x0)
    dy = abs(y1 - y0)
    x, y = x0, y0
    sx = -1 if x0 > x1 else 1
    sy = -1 if y0 > y1 else 1
    if dx > dy:
        err = dx / 2.0
        while x != x1:
            mygrid[x][y]=2
            err -= dy
            if err < 0:
                y += sy
                err += dx
            x += sx
    else:
        err = dy / 2.0
        while y != y1:
            mygrid[x][y]=2
            err -= dx
            if err < 0:
                x += sx
                err += dy
            y += sy
    mygrid[x1][y1]=2



mygrid=image.image_to_binary_grid('mynift2.png')
np.savetxt("binary_grid.txt", mygrid, fmt='%d', delimiter='')

start=[0,0]
goal=[63,63]
ans=n.SolveWithNFT(start,goal,mygrid)
for temp in ans:
    co=temp.coords
    x=co[0]
    n=co[1]
    y=co[2]
    m=co[3]
    ptx=(n+x)//2
    pty=(m+y)//2
    bresenham_line(start[0],start[1],ptx,pty,mygrid)
    start=[ptx,pty]
    
bresenham_line(start[0],start[1],goal[0],goal[1],mygrid)

img=grid.grid_to_image(mygrid)

cv2.imwrite("grid_image.png", img)
#print(ans)