f= open("wave.txt")
line=f.readline()
arr=line.strip('\n').split(',')
arr2=""
for x in range(0,172):
    arr2+=arr[x]
    arr2+=','
print(arr2)