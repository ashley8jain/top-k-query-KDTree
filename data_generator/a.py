import random
import sys

numOfArgs = len(sys.argv)-1
numOfPoints = 100000

def generatePoints(dim):
	fileName = "dataset-" + str(dim) + ".txt"
	file = open("%s"%fileName,"w")
	for i in xrange(numOfPoints):
		for j in xrange(dim):
			file.write(str(random.uniform(0,1)))
			file.write(" ")
		file.write("\n")
	file.close()

def generateForAll():
	dims = [2,3,5,10,15,20]
	for dim in dims:
		generatePoints(dim)

if(numOfArgs==0):
	generateForAll()
elif(numOfArgs==1):
	generatePoints(int(sys.argv[1]))