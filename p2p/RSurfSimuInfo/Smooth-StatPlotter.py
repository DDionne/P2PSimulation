import numpy
import matplotlib.pyplot as plt
counter = 0
def getAvg(List, Start, End): #returns the average of all the elements withing the range Start-End
    if End == -1:
        End = 0
    i = Start       #index of the first element to look at in the list
    total = 0       #total of elements, used to find the average
    divisor = 0
    while i != len(List) and i != End+1:    #Loops through the List till we get the total of all elements from Start to End
        total += List[i]
        divisor += 1
        i += 1
    if divisor == 0:
        divisor = 1
    if total/divisor == 0:
        upCount()
    return total/divisor        #average

def upCount():
    global counter
    counter += 1

filein = open("Stats-Table.txt","r")

print "Choose your Y-axis"
print "press 1 for Time"
print "press 2 for Peers Reached"
print "press 3 for QueryHits"
print "press 4 for Average Component Size"
print "press 5 for The Number of Components in the Network"
print "press 6 for The Network Diameter"
print "press 7 for the Number of Documents in the Network"
print "press 8 for the Number of Peers Online"
print "press 9 for the Queryhits per Peer Reached"
Yelement = input()


print "Do you want a percentage Slider graph?"
print "y for yes, n for no"

choice = raw_input()

Yelement = int(Yelement)
plotTitle = ''
Xaxis = []
Yaxis = []
Yaxis2 = []
Yaxis3 = []
Yaxis4 = []
Yaxis5 = []
Yaxis6 = []
Yaxis7 = []
Yaxis8 = []
Yaxis9 = []
Yaxis10 = []
SlideList = []  
total = 0
totalDatapoints = 0
lineNumber = 0
for line in filein:
    currentLine = line.split()
    
    if lineNumber != 0:
        if choice == "n":
            Xaxis.append(float(currentLine[1]))         #place the currentTime of the query into the Xaxis of the plot
            if Yelement != 9:                           #If we're not doing Queryhits per PeersReached
                info = float(currentLine[Yelement])     #get the info from the appropriate column in the file
            else:
                info = float(currentLine[3])/int(currentLine[2]) #we're doing Queryhits/PeersReached
            SlideList.append(info)      #add this info to the SlideList (Keep track of all the elements we're averaging)
            total += info               #update the total
            if len(SlideList) > 1000:        #if we're full, remove the earliest one received
                total -= SlideList.pop(0)   
            Yaxis.append(total/len(SlideList))  #add this info to the Yaxis of the plot
            
        else:
            Xaxis.append(float(currentLine[1])) #We are making 3 Plot sections (therefore we need 3 of each Time point)
            totalDatapoints += 3
            if Yelement != 9:               
                info = float(currentLine[Yelement])
            else:
                info = float(currentLine[3])/int(currentLine[2])
            SlideList.append(info)
            if len(SlideList) > 1000:
                SlideList.pop(0)
            TempList = SlideList[:]          #make a copy so that we don't modify the original list


            
            TempList.sort()                     #Sort it so that we can average the 25% lowest values and so on...
            #Yaxis.append(getAvg(TempList, 0, (len(TempList)/4)-1))  #Average the 25% lowest values
            #Yaxis.append(getAvg(TempList, len(TempList)/4, ((len(TempList)/4)*3)-1))    #Average the values between 25-75%
            #Yaxis.append(getAvg(TempList, (len(TempList)/4)*3,len(TempList)-1 ))    #Average the 25% highest values


            #Yaxis.append(TempList[len(TempList)/4])
            #Yaxis2.append(TempList[len(TempList)/2])
            #Yaxis3.append(TempList[(len(TempList)/4)*3])
            Yaxis.append(TempList[(len(TempList)/10)])
            Yaxis2.append(TempList[(len(TempList)/10)*2])
            Yaxis3.append(TempList[(len(TempList)/10)*3])
            Yaxis4.append(TempList[(len(TempList)/10)*4])
            Yaxis5.append(TempList[(len(TempList)/2)])
            Yaxis6.append(TempList[(len(TempList)/10)*6])
            Yaxis7.append(TempList[(len(TempList)/10)*7])
            Yaxis8.append(TempList[(len(TempList)/10)*8])
            Yaxis9.append(TempList[(len(TempList)/10)*9])
            Yaxis10.append(TempList[len(TempList)-1])
            

    else:
        if Yelement != 9:
            plotTitle += currentLine[Yelement] + " versus Time" #adds the appropriate title depending on which column is used
        else:
            plotTitle += "QueryHits per Peer Reached versus Time"
       
    lineNumber += 1    

print counter
print totalDatapoints
""" Plots the graph with the Title """
plt.plot(Xaxis, Yaxis,color='b',marker='.', linestyle = 'None')
if choice == 'y':
    plt.plot(Xaxis, Yaxis2, color ='g', marker='+',linestyle='None')
    plt.plot(Xaxis, Yaxis3, color ='r', marker='+', linestyle='None')
    plt.plot(Xaxis, Yaxis4, color ='c', marker='+', linestyle='None')
    plt.plot(Xaxis, Yaxis5, color ='k', marker='+', linestyle='None')
    plt.plot(Xaxis, Yaxis6, color ='y', marker='+', linestyle='None')
    plt.plot(Xaxis, Yaxis7, color ='m', marker='+', linestyle='None')
    plt.plot(Xaxis, Yaxis8, color ='w', marker='.', linestyle='None')
    plt.plot(Xaxis, Yaxis9, color ='b', marker='+', linestyle='None')
    plt.plot(Xaxis, Yaxis10, color ='g', marker='+', linestyle='None')
    plt.title(plotTitle)
plt.show()
    

