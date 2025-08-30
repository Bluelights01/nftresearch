def check(x0, y0, x1, y1, mygrid):
    dx = abs(x1 - x0)
    dy = abs(y1 - y0)
    x, y = x0, y0
    sx = -1 if x0 > x1 else 1
    sy = -1 if y0 > y1 else 1

    if dx > dy:
        err = dx / 2.0
        while x != x1:
            if mygrid[x][y] == 0:   
                return 0
            err -= dy
            if err < 0:
                y += sy
                err += dx
            x += sx
    else:
        err = dy / 2.0
        while y != y1:
            if mygrid[x][y] == 0:   
                return 0
            err -= dx
            if err < 0:
                x += sx
                err += dy
            y += sy

    
    if mygrid[x1][y1] == 0:
        return 0

    return 1  





def line_join(path,Start,End,mygrid):
    ans=[path[0]]
    i=0
    while(i<len(path)-2):

        
        s=i+1
        e=len(path)-1

        while(s+1<e):
            mid=(s+e)//2
            if(check(path[i][0],path[i][1],path[mid][0],path[mid][1],mygrid)):
                s=mid
            else:
                e=mid-1
        
        i=e
        ans.append(path[i])
    
    if(path[len(path)-1]!=End):
        path.append(End)

    return ans
            