import queue
import math
def hn(x,y,X,Y):
     return math.sqrt((X - x)**2 + (Y - y)**2)

def Solve(matrix,x,y,goal,n,m,path,parent,dist,ans):
     
     
     if(x==goal[0] and y==goal[1]):
          return 1
     q=[]
     if(x+1<n and (path[x+1][y]>dist+1) and matrix[x+1][y]!=0):
               parent[x+1][y]=[x,y]
               path[x+1][y]=dist+1
               h=hn(x+1,y,goal[0],goal[1])
               q.append([h,x+1,y,dist+1])
     if(y+1<m and (path[x][y+1]>dist+1) and matrix[x][y+1]!=0):
               parent[x][y+1]=[x,y]
               path[x][y+1]=dist+1
               h=hn(x,y+1,goal[0],goal[1])
               q.append([h,x,y+1,dist+1])
     if(x-1>=0 and (path[x-1][y]>dist+1) and matrix[x-1][y]!=0):
               parent[x-1][y]=[x,y]
               path[x-1][y]=dist+1
               h=hn(x-1,y,goal[0],goal[1])
               q.append([h,x-1,y,dist+1])
     if(y-1>=0 and (path[x][y-1]>dist+1) and matrix[x][y-1]!=0):
               parent[x][y-1]=[x,y]
               path[x][y-1]=dist+1
               h=hn(x,y-1,goal[0],goal[1])
               q.append([h,x,y-1,dist+1])

     q.sort(key=lambda x: x[0])
     for ft in q:
           ans.append([ft[1],ft[2]])
           val=Solve(matrix,ft[1],ft[2],goal,n,m,path,parent,dist+1,ans)
           if(val==1):
                 return 1
           ans.pop()

     return 0
     

def Astar(matrix,start,goal):
      
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
         path[start[0]][start[1]]=0
      ans=[]
      Solve(matrix,start[0],start[1],goal,n,m,path,parent,0,ans)

      return ans
     
