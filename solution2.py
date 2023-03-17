V=0.6
d=0
x=0
while d<3:
    print(d)
    d+=0.03125
for i in range(0,13):
    print(str(x)+":"+str(V*i/12))
    x+=1
x=0
for i in range(0,13):
    print(str(x)+":"+str((V*i/12)+0.6))
    x+=1
x=0
for i in range(0,13):
    print(str(x)+":"+str((V*i/12)+1.2))
    x+=1
x=0
for i in range(0,13):
    print(str(x)+":"+str((V*i/12)+1.8))
    x+=1
x=0
for i in range(0,13):
    print(str(x)+":"+str((V*i/12)+2.4))
    x+=1
