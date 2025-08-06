
#nw=0 ne=1 sw=2 se=3
#n=0 e=1 s=2 w=3

import heapq
import math
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


class QuadNode:
    def __init__(self, value=None, children=None, parent=None, sontype=0, coords=None,pathparent=None):
        self.value = value              # white or black
        self.children = children        # list of 4 children
        self.parent = parent            # parent node
        self.sontype = sontype          # index: 0-NW, 1-NE, 2-SW, 3-SE
        self.coords = coords  
        self.pathparent=pathparent          # (row_start, row_end, col_start, col_end)

    def is_leaf(self):
        return self.children is None

    def __repr__(self, level=0):
        indent = "  " * level
        coord_str = f" {self.coords}" if self.coords else ""
        if self.is_leaf():
            return f"{indent}Leaf({self.value}){coord_str}"
        else:
            s = f"{indent}Node:{coord_str}\n"
            directions = ['NW', 'NE', 'SW', 'SE']
            for dir, child in zip(directions, self.children):
                s += f"{indent}  {dir}:\n{child.__repr__(level + 2)}\n"
            return s


def is_uniform(matrix):
    first = matrix[0][0]
    return all(cell == first for row in matrix for cell in row)


def build_quadtree(matrix, parent=None, sontype=0, r_start=0, c_start=0):
    size = len(matrix)
    coords = (r_start, r_start + size - 1, c_start, c_start + size - 1)

    if is_uniform(matrix):
        return QuadNode(value=matrix[0][0], parent=parent, sontype=sontype, coords=coords)

    half = size // 2

    def subregion(matrix, row_start, col_start, size):
        return [row[col_start:col_start + size] for row in matrix[row_start:row_start + size]]


    half = size // 2
    node = QuadNode(parent=parent, sontype=sontype, coords=coords)
    node.children = [
    build_quadtree(subregion(matrix, 0, 0, half), node, 0, r_start, c_start),  # NW
    build_quadtree(subregion(matrix, 0, half, half), node, 1, r_start, c_start + half),  # NE
    build_quadtree(subregion(matrix, half, 0, half), node, 2, r_start + half, c_start),  # SW
    build_quadtree(subregion(matrix, half, half, half), node, 3, r_start + half, c_start + half)  # SE
    ]

    return node




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
    def __init__(self, priority, item):
        self.priority = priority
        self.item = item
        
    def __lt__(self, other):
        return self.priority < other.priority

def SolveWithNFT(Start, End, grid):
    quadtree_root = build_quadtree(grid)
    start_node = find(Start, quadtree_root)
    end_node = find(End, quadtree_root)
    visited = set()
    heap = []
    
    # Wrap the path in our custom class
    heapq.heappush(heap, PriorityItem(0, [start_node]))

    visited.add(start_node)

    while heap:
        current_item = heapq.heappop(heap)
        dist = current_item.priority
        path = current_item.item
        current = path[-1]

        if current == end_node:
            break

        for direction in range(4):
            neighbor = getneighbour(current, direction)
            if neighbor and neighbor not in visited:
                if neighbor.children==None and neighbor.value==1:
                    new_dist = dist + (neighbor.coords[1]-neighbor.coords[0])
                    h=hn((neighbor.coords[1]+neighbor.coords[0])/2,(neighbor.coords[2]+neighbor.coords[3])/2,End[0],End[1])
                    neighbor.pathparent=current
                    visited.add(neighbor)
                    heapq.heappush(heap, PriorityItem(new_dist+h,[neighbor]))
                if neighbor.value==None:
                    a_part=blocks[direction][0]
                    b_part=blocks[direction][1]
                    tlist=[]
                    graytoneighbour(neighbor,a_part,b_part,tlist)
                    for padosi in tlist:
                        if(padosi not in visited):

                            h=hn((padosi.coords[1]+padosi.coords[0])/2,(padosi.coords[2]+padosi.coords[3])/2,End[0],End[1])
                            new_dist=dist+(padosi.coords[1]-padosi.coords[0])
                            padosi.pathparent=current
                            visited.add(padosi)
                            heapq.heappush(heap, PriorityItem(new_dist+h,[padosi]))


    finalpath=[]
    while(end_node.pathparent!=None):
        finalpath.append(end_node)
        end_node=end_node.pathparent        
    if(len(finalpath)!=0):
        finalpath.append(start_node)   

    finalpath.reverse()        
    return finalpath