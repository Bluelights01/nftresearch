
import queue
import math
import time

def hn(x,y,X,Y):
     return math.sqrt((X - x)**2 + (Y - y)**2)
def findpath(matrix,start,goal):
    n=len(matrix)
    m=len(matrix[0])
    maxint=1000000
    path=[]
    parent=[]
    
    for i in range(n):
         list=[]
         temp=[]
         for j in range(m):
              list.append(maxint)
              temp.append([-1,-1])
         path.append(list)
         parent.append(temp)   
           
    q=queue.PriorityQueue()
    
    q.put([0,start[0],start[1],0])
    path[start[0]][start[1]]=0
    while(q.empty()==False):
          
          a=q.get()
          x=a[1]
          y=a[2]
          dist=a[3]
          
          if(x+1<n and (path[x+1][y]>dist+1) and matrix[x+1][y]!=0):
               parent[x+1][y]=[x,y]
               path[x+1][y]=dist+1
               h=hn(x+1,y,goal[0],goal[1])
               q.put([h+dist+1,x+1,y,dist+1])
          if(y+1<m and (path[x][y+1]>dist+1) and matrix[x][y+1]!=0):
               parent[x][y+1]=[x,y]
               path[x][y+1]=dist+1
               h=hn(x,y+1,goal[0],goal[1])
               q.put([h+dist+1,x,y+1,dist+1])
          if(x-1>=0 and (path[x-1][y]>dist+1) and matrix[x-1][y]!=0):
               parent[x-1][y]=[x,y]
               path[x-1][y]=dist+1
               h=hn(x-1,y,goal[0],goal[1])
               q.put([h+dist+1,x-1,y,dist+1])
          if(y-1>=0 and (path[x][y-1]>dist+1) and matrix[x][y-1]!=0):
               parent[x][y-1]=[x,y]
               path[x][y-1]=dist+1
               h=hn(x,y-1,goal[0],goal[1])
               q.put([h+dist+1,x,y-1,dist+1])
          

    
    
    ans=[]
    count=0
    
    while(True):
         ans.append(goal)
         count+=1
         goal=parent[goal[0]][goal[1]]
         if(goal[0]==-1):
              break
    
    return ans


