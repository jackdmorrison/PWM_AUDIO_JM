import math
f=open("output.txt","w")
Frequency=256
samplerate=11000
amplitude=1
period=1/Frequency
B=2*math.pi/period
interval=1/samplerate
x=interval
d=1
vals="["
v=amplitude*(math.sin(B*x))
vals+=str(int((v+amplitude)/(amplitude*2)*255))
x+=interval
print(d)
while x<(period+(interval)):
    vals+=', '
    v=amplitude*(math.sin(B*x))
    vals+=str(int((v+amplitude)/(amplitude*2)*255))
    x+=interval
    d+=1
    print(d)
vals+=']'
f.write(vals)
f.close()

