class student:
    def __init__(self,name ,age):
        self.name=name
        self.age=age



a=student("ayush",5)
print(a.age)

y=int(input("enter number"))
if(y<0):
    raise ValueError("enter posi")

try:
   x=5/2
   x=int(input())
except ZeroDivisionError as e:
    print(e)
except ValueError as e:
    print(e)