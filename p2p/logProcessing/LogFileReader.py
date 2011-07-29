#edit here the name of the input log file

def timetomillisec(timewithcolumns):
    timebits = timewithcolumns.split(':')
    timemillisec = int(timebits[0]) * 3600000 + int(timebits[1])*60000 + int(timebits[2])*1000 + int(timebits[3])
    return timemillisec

def decodemsg(encodedmsg):
    left = int(encodedmsg/1000000)#left 3 digits
    middle = int(encodedmsg/1000)%1000 #middle 3 digits
    right = encodedmsg%1000 #right 3 digits
    return (left,middle,right)

def parselog(lfname):
    logfile = open(lfname,"r")
    count = 0
    outlines = []
    print "reading "+ lfname+ "..."
    for line in logfile:
        words = line.split()
        count = count +1
    #msg type is words[1], time is words[3], whofrom is words[5],
    #port is words[7] value is words[9] and whoto is words[11]
#fishing for :
#Mensaje Y / 00:00:30:908 / session73(228) / query / 73002.000000000 para peer73(227)
    # (peer 73 outputs query 2)
#Mensaje Y / 00:00:35:990 / session73(228) / publish / 73012.000000000 para peer73(227)
    # (peer 73 publishes document 12)
#Mensaje Y / 01:31:18:175 / peer56(176) / out_online / 56.000000000 para top(01)
    # (peer 56 goes online)    
#Mensaje Y / 01:30:00:482 / peer26(86) / out_offline / 26.000000000 para top(01)
    # (peer 26 goes offline)
#Mensaje Y / 00:00:33:773 / connector23(07) / out_disconnect / 23021.000000000 para peer23(02)
    # (peer 23 disconnects from peer 21)    
#Mensaje Y / 00:00:33:773 / netwk(07) / out_connect / 1038021.000000000 para network(02)
    # (peer 38 connects to peer 21 (successful connection))    
#Mensaje X / 00:00:36:848 / peer16(56) / queryhit / 16012007.000000000 para session16(57)
    # (peer 16 sends queryhit for document 12)
#Mensaje Y / 00:12:24:024 / gnut(06) / route_out / 19020.000000000 para network(02)
    # (peer 19 receives query 20)
        if (words[1]=='Y'): #possibly interesting
            if(words[5].startswith('peer')):
                if (words[7].startswith('out_online') | words[7].startswith('out_offline') |words[7].startswith('query')|words[7].startswith('publish')):
                    peernumber = int(words[9][0:words[9].index('.')])#left of the period
                    if (words[7].startswith('out_online')|words[7].startswith('out_offline')):
                        outlines.append(str(timetomillisec(words[3]))+ ':'+words[7][4:]+':'+str(peernumber)+ "\n")
                    if (words[7].startswith('query')|words[7].startswith('publish')):
                        (nothing,querypeer,queryterm) = decodemsg(peernumber)
                        outlines.append(str(timetomillisec(words[3]))+':'+ words[7]+ ':'+ str(querypeer)+ ':'+ str(queryterm) + '\n')

            elif (words[5].startswith('netwk') & words[7].endswith('_connect')):
                mvalue=int(words[9][0:words[9].index('.')])#left of the period  
                (one, peer2, peer1) = decodemsg(mvalue)
                outlines.append(str(timetomillisec(words[3]))+ ':connect:'+str(peer1)+':'+str(peer2)+"\n")

            elif (words[5].startswith('connector') & words[7].endswith('_disconnect')):
                mvalue=int(words[9][0:words[9].index('.')])#left of the period
                (one, peer2, peer1) = decodemsg(mvalue)
                outlines.append(str(timetomillisec(words[3]))+ ':disconnect:'+str(peer1)+':'+str(peer2)+"\n")

                
            elif (words[5].startswith('gnut')& words[7].startswith('route_out')):
                mvalue=int(words[9][0:words[9].index('.')])#left of the period  
                (nah, peer, msgid) = decodemsg(mvalue)
                outlines.append(str(timetomillisec(words[3]))+ ':queryreachespeer:'+str(peer)+':'+str(msgid)+"\n")
            
        elif ((words[1]=='X') & (words[5].startswith('peer')) & (words[7].startswith('queryhit'))):
            encodedmsg = int(words[9][0:words[9].index('.')])#left of the period
            (peernumber,docnumber,whatever) =decodemsg(encodedmsg)
            receivingpeer = int(words[5][4:words[5].index('(')])#get the peer number from a string like 'peer43(122)'
            if (peernumber != receivingpeer): outlines.append(str(timetomillisec(words[3]))+':'+ words[7]+ ':'+ str(peernumber)+ ':'+ str(docnumber) + '\n')
        
        if (count%100000==0): print str(count/1000)+'K', "lines..."
    print count,"lines read."
    logfile.close()
    return outlines


lfname = raw_input("Please enter the name of the log file to process:\n")
if (lfname == "") :
    lfname ="P2P90LOG.log"
    print "Using default input file : P2P90LOG.log"
pfname = raw_input("Please enter the name of output file (def:'ProcessedLog.txt'):\n")
if (pfname == "") :
    pfname ="ProcessedLog.txt"
    #print "Using default output file :ProcessedLog.txt"

myoutput = parselog(lfname)
print "Writing", len(myoutput), "lines to",pfname
processedfile= open(pfname, 'w', 4096)
processedfile.write(str(len(myoutput))+"\n")
for ll in myoutput:
    processedfile.write(ll)
    
print "Done."
processedfile.close()

