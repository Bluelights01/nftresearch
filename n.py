#nw=0 ne=1 sw=2 se=3
#n=0 e=1 s=2 w=3
import heapq
import math
import time
import quadtree
import line
Adj=[
    [1,1,0,0],
    [0,1,0,1],
    [0,0,1,1],
    [1,0,1,0]
]

blocks=[
    [2,3],
    [0,2],
    [0,1],
    [1,3]
]
Reflect=[
  [2,3,0,1],
  [1,0,3,2],
  [2,3,0,1],
  [1,0,3,2]
]

def hn(x,y,X,Y):
     return math.sqrt((X - x)**2 + (Y - y)**2)

def center(coords):
    x1, y1, x2, y2 = coords
    return ((x1 + x2) / 2, (y1 + y2) / 2)

def dist_between_squares(coords1, coords2):
    cx1, cy1 = center(coords1)
    cx2, cy2 = center(coords2)
    return math.sqrt((cx1 - cx2)**2 + (cy1 - cy2)**2)


def bresenham_line(x0, y0, x1, y1,mygrid):
    steps=0
    dx = abs(x1 - x0)
    dy = abs(y1 - y0)
    x, y = x0, y0
    sx = -1 if x0 > x1 else 1
    sy = -1 if y0 > y1 else 1
    if dx > dy:
        err = dx / 2.0
        while x != x1:
            mygrid[x][y]=2
            steps+=1
            err -= dy
            if err < 0:
                y += sy
                err += dx
            x += sx
    else:
        err = dy / 2.0
        while y != y1:
            mygrid[x][y]=2
            steps+=1
            err -= dx
            if err < 0:
                x += sx
                err += dy
            y += sy
    mygrid[x1][y1]=2
    steps+=1
    return steps

def getneighbour(P,D):
    Q=None
    if(P.parent!=None and Adj[D][P.sontype]==1):
        Q=getneighbour(P.parent,D)
    else:
        Q=P.parent

    
    if(Q!=None and Q.children!=None):
        temp=Reflect[D][P.sontype]
        return Q.children[temp]
    return Q  
def graytoneighbour(P,a,b,ans):
    if(P.value==1):
        ans.append(P)
        return
    if(P.value==0):
        return
    else:
        graytoneighbour(P.children[a],a,b,ans)
        graytoneighbour(P.children[b],a,b,ans)


def find(start, root):
    x, n, y, m = root.coords

    for i in range(x, n + 1):
        for j in range(y, m + 1):
            if (i == start[0] and j == start[1]):
                if root.children is None:
                    return root
                else:
                    for temp in range(4):
                        result = find(start, root.children[temp])
                        if result:
                            return result
    return None


class PriorityItem:
    def __init__(self, priority, item,dist):
        self.priority = priority
        self.item = item
        self.dist=dist
        
    def __lt__(self, other):
        return self.priority < other.priority

def SolveWithNFT(Start, End, grid):
    quadtree_root = quadtree.build_quadtree(grid)
    start_node = find(Start, quadtree_root)
    end_node = find(End, quadtree_root)
    visited = set()
    heap = []
    
    heapq.heappush(heap, PriorityItem(0, [start_node],0))

    visited.add(start_node)

    while heap:
        current_item = heapq.heappop(heap)
        dist = current_item.dist
        path = current_item.item
        current = path[-1]

        if current == end_node:
            break

        for direction in range(4):
            neighbor = getneighbour(current, direction)
            if neighbor and neighbor not in visited:
                if neighbor.children==None and neighbor.value==1:
                    new_dist = dist + dist_between_squares(neighbor.coords,current.coords)
                    h=hn((neighbor.coords[1]+neighbor.coords[0])/2,(neighbor.coords[2]+neighbor.coords[3])/2,End[0],End[1])
                    neighbor.pathparent=current
                    visited.add(neighbor)
                    heapq.heappush(heap, PriorityItem(new_dist+h,[neighbor],new_dist))
                if neighbor.value==None:
                    a_part=blocks[direction][0]
                    b_part=blocks[direction][1]
                    tlist=[]
                    graytoneighbour(neighbor,a_part,b_part,tlist)
                    for padosi in tlist:
                        if(padosi not in visited):

                            h=hn((padosi.coords[1]+padosi.coords[0])/2,(padosi.coords[2]+padosi.coords[3])/2,End[0],End[1])
                            new_dist=dist+dist_between_squares(padosi.coords,current.coords)
                            padosi.pathparent=current
                            visited.add(padosi)
                            heapq.heappush(heap, PriorityItem(new_dist+h,[padosi],new_dist))


    finalpath=[]
    while(end_node.pathparent!=None):
        finalpath.append(end_node)
        end_node=end_node.pathparent        
    if(len(finalpath)!=0):
        finalpath.append(start_node)   

    finalpath.reverse()
    



    newpath=[Start]

    for temp in finalpath:
        co=temp.coords
        x=co[0]
        n=co[1]
        y=co[2]
        m=co[3]
        ptx=(n+x)//2
        pty=(m+y)//2
        newpath.append([ptx,pty])
    
    newpath.append(End)     
    newpath=line.line_join(newpath,Start,End,grid)
    print(len(newpath)-2)
    
    steps=0
    for temp in newpath:
        steps+=bresenham_line(Start[0],Start[1],temp[0],temp[1],grid)
        Start=[temp[0],temp[1]]  

    steps+=bresenham_line(Start[0],Start[1],End[0],End[1],grid)
    print(steps)
    return grid