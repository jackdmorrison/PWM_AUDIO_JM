f=open("frequencies.txt")
freqs=open("freqs.txt","w")
str="{"
c=0
for line in f:
    l=line.split("\t")
    if(20000>float(l[1])>20):
        str+=l[1]
        str+=", "
        c+=1
str+="};"
freqs.write(str)
freqs.close()
print(str)
print(c)