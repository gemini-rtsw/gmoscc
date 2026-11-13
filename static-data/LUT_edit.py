# This will change the third colum of numbers in a GMOS LUT by an arbitray amount

print("Enter the file to change: ")
file1 = raw_input()

print("What is the change? ")
change = float(raw_input())


inFile = open(file1, 'r')
entry = inFile.readlines()
inFile.close()

outFile = open("templut", 'w')

for line in entry:
  colum = line.split()
#print colum[2]
#  print float(colum[2]) + change
  outFile.write(str(colum[0]) + "  " + str(colum[1]) + "      " + str(float(colum[2]) + change) + "\n")
  
outFile.close()



#for i in range(100):
#  entry = inFile.readline()
#  colum = entry.split()
#  colum3 = colum[2] + change
#  print(colum3)
  


#while (file1):
#  entry = inFile.readline()
#  colum = entry.split()
#  colum3 = colum[2] + change
#  print(colum3)
  


  

#for i in range(1, len(file1)+1):
  
  
#print(colum)
#print(colum3)

#while (entry):
#  print(entry)
  


#col3 = readFile.split()
#print col3[2]



#inFile = open(file1)

