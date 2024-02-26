#!/bin/python3

matrix = []
idlist = []
timelist = []
idset = []

timesum = []

file = open("output.txt", 'r')
lines=file.readlines()

for y in lines:
    result=y.split(' ')
    idlist.append(result[2])
    timelist.append(result[5])

for x in idlist:
    if x not in idset:
        idset.append(x)

timerange = []

for h in idset:
    timesum.append(timerange)

v = 0
while v < len(idset):
    d = 0
    while d < len(idlist):
        if (idset[v] == idlist[d]):
                timesum[v].append(int(timelist[d]))
        d += 1

    v += 1

avg = []
small = []
big = []

print("\n-------------Client Data RTT Summary------------\n")
p = 0
while p < len(timesum):
    u = 0
    print ("client id: " + idset[p])
    print("Mean: " + str(sum(timesum[p]) / len(timesum[p])) + "us | " + 
            "Min: " + str(min(timesum[p])) + "us | " + 
            "Max: " + str(max(timesum[p])) + "us \n")
    avg.append(sum(timesum[p]) / len(timesum[p]))
    small.append(min(timesum[p]))
    big.append(max(timesum[p]))
    p += 1

 
print("\n-------------Total Data  Summary------------\n")
print("Clients: " + str(len(idset)) + "\n")
print("Bytes sent: " + str(len(lines) * 80) + "\n")
print("Mean: " + str(sum(avg)/len(avg)) + "us\n")
print("Min: " + str(min(avg)) + "us\n")
print("Max: " + str(max(avg)) + "us\n")
