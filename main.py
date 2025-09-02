
import image
import grid
import cv2
import AN
import numpy as np
import time
mygrid=image.image_to_binary_grid('gemini.png')


np.savetxt("binary_grid.txt", mygrid, fmt='%d', delimiter='')

start=[0,0]
goal=[500,500]
start_time=time.time()
ans=AN.findpath(mygrid,start,goal)
end_time=time.time()

xp=-2
yp=-2
turns=0
for i in range(0,len(ans)):
    mygrid[ans[i][0]][ans[i][1]]=2
    p=ans[i][0]
    q=ans[i][1]
    
    if(i-1>0):
        r=ans[i-1][0]
        s=ans[i-1][1]
        if(i-1>0 and (r-p!=xp or s-q!=yp)):
            turns+=1
            xp=r-p
            yp=s-q


    
img=grid.grid_to_image(mygrid)
print(end_time-start_time)
print(turns)
cv2.imwrite("grid_image.png", img)

