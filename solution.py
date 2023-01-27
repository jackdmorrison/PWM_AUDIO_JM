import math
f=open("output.txt","w")
Frequency=256
samplerate=44000
amplitude=1
period=1/Frequency
B=2*math.pi/period
interval=1/samplerate
x=interval
arr=[]
vals="{"
v=amplitude*(math.sin(B*x))
arr.append(int((v+amplitude)/(amplitude*2)*255))
vals+=str(int((v+amplitude)/(amplitude*2)*255))
x+=interval
while x<(period+(interval)):
    vals+=', '
    v=amplitude*(math.sin(B*x))
    vals+=str(int((v+amplitude)/(amplitude*2)*255))
    arr.append(int((v+amplitude)/(amplitude*2)*255))
    x+=interval
print(arr)
print(len(arr))
vals+='};'
f.write(vals)
f.close()

