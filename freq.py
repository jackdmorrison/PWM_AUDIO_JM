f=open("frequencies.txt")
freqs=open("freqs.txt","w")
str="{"
b=256
c=0
for line in f:
    l=line.split("\t")
    if(20000>float(l[1])>20):
        print(float(l[1])/256*2)
        str+=l[1]
        str+=", "
        c+=1
str+="};"
freqs.write(str)
freqs.close()
#print(str)
#print(c)