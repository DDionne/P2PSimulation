
;;;;;;;;;;;;;;;;;;;;;;;;
;;;; PEER VARIABLES ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;
turtles-own [
  
  online? ; tells us whether or not the peer is online
  peerID ; ID of the peer (gets assigned on setup)
  currentFriends ;
  happiness;
  explored?;
  searchHits;
  lastSearchedDoc;
  possibleResults ;; number of peers that might give results
  similaritySum ;
  timeSpentOffline;
  offlineTime;
  onlineTimeTotal;
  onlineTime;
  currentDocs;
  DocsToPublish;
  searchChance;
  changeFriendsChance;
  componentSize;
  
  
  ]


;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; GLOBAL VARIABLES ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;
globals [
  
  PeerNumber ; Number that gets assigned to peers (gets incremented by 1 each time it gets assigned)
  MAXDOCS ; Maximum amount of documents for each peer
  docList;
  onlinePeerList
  layout?
  totalHitsThisTick;
  searches
  peersOnline
  VERBOSE?
  toFile?
  msgID
  numPeers
  
  ]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Setup button, clears the simulation, resets the ticks and call all other setup methods ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to setup
  clear-all
  set VERBOSE? false
  set toFile? false
  set-default-shape turtles "circle"
  set totalHitsThisTick 0
  set msgID 0
  set searches 0
  setup-documents
  setup-turtles
  setup-friends
  set layout? true
  if file-exists? "fileout.txt" [ file-delete "fileout.txt" ]
;;  setup-friends <--Currently very innefficient
  set MAXDOCS 50
  set onlinePeerList []
  reset-ticks
  reset-timer
end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Sets up all the peers ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to setup-turtles
  create-turtles NumPeers
  foreach sort-by [[who] of ?1 < [who] of ?2] turtles [
    ask ?[ 
    set color red
    set online? false
    set componentSize 1
    set explored? false
    set offlineTime 50 + random 500
    set timeSpentOffline 0
    set onlineTimeTotal 180 + random 180
    set onlineTime 0
    set currentFriends []
    set searchHits 0
    set similaritySum 0
    set peerID PeerNumber
    set PeerNumber PeerNumber + 1
    set currentDocs []
    set DocsToPublish []
    set DocsToPublish item peerID docList
    set lastSearchedDoc -1
    set searchChance 95
    set changeFriendsChance (100 - searchChance)
    layout-circle turtles max-pxcor - 1  
    hide-turtle
    ]
  ]
end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Sets up the peers initial set of documents from a file names MultiPeer.txt ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to setup-documents
  set docList []
  file-open "MultiPeer.txt"
  let docStart false
  while [not file-at-end?][
    if-else docStart = false [
      let line file-read-line
      if-else line != "docs" [
        let number read-from-string line
        set numPeers number  
      ]
      [ set docStart true ] 
    ]
    [
      let tempo file-read-line
    
      if-else tempo != "end"[
        let temp read-from-string tempo
        if-else length docList = 0
        [
          set docList lput [] docList
          let a item 0 docList
          set a lput temp a
          set a remove-duplicates a
          let b ((length docList) - 1)
          set docList (replace-item b docList a)
        ]
        [
          let b ((length docList) - 1)
          let a item b docList
          set a lput temp a
          set a remove-duplicates a
          set docList (replace-item b docList a)
        ]
      ]
      [
        set docList lput [] docList 
      ]
    ]
  ]
  file-close
end
    

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; continuous loop of a peer behaviour (1 loop = 1 tick) ;;;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to go
 
 file-open "fileoutLearn.txt"
 set totalHitsThisTick 0
 set searches 0
 foreach sort-by [[who] of ?1 < [who] of ?2] turtles [            ;Loop through each peer exactly once
   ask turtles [set explored? false]
   ask ?[
     do-something
     ifelse show-IDs?
       [ set label happiness ]
       [ set label "" ]
   ]
 ]
 layout
 file-close
 tick
end
    

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Does one tick (works with the button) ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to step 
  go
end



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; What peers do in the network ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to do-something
  ;ask turtle peer[
    if not online? [
      if-else timeSpentOffline >= offlineTime [                        ;He reached the point where he has been offline for a long enough period of time
        if random 100 < 15 [                                           ;Other little random factor (might remove this)
          set online? true                                             ;We're going online
          
          while [length DocsToPublish != 0 ]                           ;Used so that his initial set of documents gets published once
          [
            publish first DocsToPublish
            set DocsToPublish remove first DocsToPublish DocsToPublish
          ]   
          
          if VERBOSE? [ Type "online:" print peerID ]                 
          if toFile? [type (ticks * 10000) file-type ":" file-Type "online:" file-print peerID ]     ;outputs the online message to "fileout.txt"
          set peersOnline peersOnline + 1                                                            
          set onlinePeerList lput who onlinePeerList                                                 ;add the peer to the list of all online peers in the network
          show-turtle                                                                                ;show the peer on the visualisation screen
          set timeSpentOffline 0          ;;hes online now, so this gets reset to 0
          set offlineTime 50 + random 500 ;;Next time he goes offline, it will be for this amount of time
          if length currentFriends != 0[                ;If he already has friends on his friends list, then connect to them if there online (see "show-friend-connections")
            show-friend-connections peerID
          ]
        ]  
      ]
      [     ;timeSpentOffline is less than the offlineTime, so we simply increment the timeSpent by 1 and move on
        set timeSpentOffline timeSpentOffline + 1 ;;update his time spend offline by 1 tick
      ]
      
    ]
    if online?[
      if-else onlineTime >= onlineTimeTotal[                ;If time spent online reaches onlineTime, then we go offline
        set online? false                                  ;We're going offline
        set peersOnline peersOnline - 1
        set onlinePeerList remove who onlinePeerList       ;remove the peer from the list of all online peers in the network
        hide-turtle                                        ;remove the peer from the visualisation screen
        set onlineTime 0 ;;reset his amount of time online to 0 (hes offline)
        set onlineTimeTotal 200 + random 200 ;;Next time he goes online, he will be online for this amount of time
        if length currentFriends != 0[                     ;if he had friends, then disconnect from them
          hide-friend-connections peerID
        ]
        if VERBOSE? [ Type "offline:" print peerID ]
        if toFile? [ file-type (ticks * 10000) file-type ":" file-type "offline" file-print peerID ]      ;output the offline message to "fileout.txt"
      ]
      [
        ;;Peer stays online for the time being, therefore he needs to do something
        set onlineTime onlineTime + 1  ;;update his time spent online by 1 tick
       ;; ifelse length currentFriends = 0
       ;; [ find-new-similar-friend who ]
        decide-action                      ;decides on what action to take (search, publish, delete, etc)
        
        
      ]
    ]
  ;]
end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Method that decides what the best course of action is for a specific peer ;;;;
;;;; Possible actions are :                                                    ;;;;
;;;;                                                                           ;;;; <--Need to make this work with happiness (need to do that first though)
;;;; -Search for a document                                                    ;;;;
;;;; -Add/Remove a friend                                                      ;;;;
;;;; -Publish/Delete a document                                                ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to decide-action
  let actionDecider random 100
  if-else actionDecider < changeFriendsChance [
    manage-surroundings 
  ]
  [
    let docToSearch random 965
    search peerID docToSearch msgID
    set msgID msgID + 1
    if searchHits > 0 [ publish docToSearch ]
  ]
  
  set happiness checkHappiness
  set changeFriendsChance precision ((100 - happiness) / 10) 0
  set searchChance 100 - changeFriendsChance
  ;Type "Peer: " type who type " SearchChance: " type searchChance type " otherChance: " print changeFriendsChance
  
end


to manage-surroundings 
  
  
  
end



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Does what the name implies (must be in the instance of a peer, can't be in the instance of an observer) ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to delete-document [document]
  if VERBOSE? [ type "remove:" type who type ":" print document]
  if toFile? [ file-type (ticks * 10000) file-type ":" file-type "remove" file-type ":" file-print document ] ;outputs the remove message to "fileout.txt"
  set currentDocs remove document currentDocs  
end








;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Searches through all peer connections for the specified document ;;;; <--Some credit goes to the Uri Wilensky (took and modified the explore method)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to search [peer document messageID] 
    set searchHits 0
    explore peer document msgID 
    unexplore                      ;Used to set all peers explored variable to false (can't do ask turtles [] because we're not in observer mode here)
    set totalHitsThisTick totalHitsThisTick + searchHits
end


to explore [peer document messageID] ;; node procedure
  if explored? [ stop ]
  if peer != who [ 
    
    if VERBOSE? [ Type "queryreachespeer:" type who type ":" print messageID ]
    if toFile? [ file-type "queryreachespeer:" file-type who file-type ":" file-print messageID ]
  ]  
  set explored? true
  if hasDocument? document and who != peer [  
    ask turtle peer [ 
      set searchHits searchHits + 1 
      set lastSearchedDoc document
      
    ]
    if VERBOSE? [ Type "queryhit:" type peer type ":" type document type":" print messageID ]
    if toFile? [ file-type (ticks * 10000) file-type ":" file-type "queryhit" file-type ":" file-type peer file-type ":" file-type document file-type ":" file-type messageID ]
  ]
  ask link-neighbors [ explore peer document messageID]
end

to unexplore                                     ;Simply travels through the network in order to set all the "explored?" variables to false
  if not explored? [ stop ]
  set explored? false
  ask link-neighbors [ unexplore ]
end

to-report hasDocument? [ document ]
  if-else member? document currentDocs  [report true]
  [report false]
end

to-report peer-popularity [peer]
  
  report count [link-neighbors] of turtle peer
end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; finds a new friend randomly from a set of all online peers thats not already a friend ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to find-new-random-friend [peer]
  let peer1 turtle peer
  let availablePeerList list-difference onlinePeerList [currentFriends] of peer1        ;Get the difference between online peers and current friends
  set availablePeerList remove peer availablePeerList                                   ;Remove yourself from the list (don't want to add yourself, that would be weird)
  if length availablePeerList > 0[                                                      ;If there are available people
    let peer2 turtle getRandomItem availablePeerList                                    ;pick one at random and add that person!
    ask peer1 [
      addFriend who [who] of peer2
    ]
  ]
end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; finds and connects to the most similar peer in the network ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to find-new-similar-friend [peer]
  let peer1 turtle peer
  let availablePeerList list-difference onlinePeerList [currentFriends] of peer1       ;Get the difference between online peers and current friends
  set availablePeerList remove peer availablePeerList                                  ;Remove yourself
  if length availablePeerList > 0[                                                     ;If there are available people
    let bestPeer item 0 availablePeerList                                              ;set the initial peer as the first one in the list
    let similarity 0                                                                   ;set the similarity to 0 (anything would beat this except for other 0's)
    foreach availablePeerList[                                                         ;Loop through all the available peers and pick the most similar one
      let a checkSimilarity peer ?
      if a > similarity[
        set bestPeer ?
        set similarity a
      ]
    ]
    ask peer1 [addFriend who [who] of turtle bestPeer                                  ;Add that Peer!
      
      set similaritySum similaritySum + similarity
      ]
  ]
end



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Removes the friends that has the least document similarity ;;;; <--Will always remove a friend 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;    Does the same thing as find-new-similar-friend, but uses the currentFriends list and deletes the lowest similarity
to remove-least-similar-friend [peer]
  let peer1 turtle peer
  ask peer1[
    let worstFriend item 0 currentFriends
    let similarity 100
    foreach currentFriends[
      let a checkSimilarity peer ?
      if a < similarity [
        set worstFriend ?
        set similarity a   
      ]    
    ]  
    remove-friend peer worstFriend
    set similaritySum similaritySum - similarity
  ]

end



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Returns the average similarity between a peer and all his friends ;;;; <--- Currently not used
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to-report similarityAvg [peer]
  let a similaritySum / length currentFriends
  report a
end
  



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Do list1 - list2 (returns a list which is the difference between list1 and list2 ;;;;
;;;;                                                                                  ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to-report list-difference [list1 list2]
  let a list1
  foreach list2 [
    set a remove ? a
  ]
  report a
end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Returns a random value from a list ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to-report getRandomItem [aList]
  if length aList > 0[
    report item random length aList aList
  ]
  
end
  

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Checks for similarity in repositories between 2 peers ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to-report checkSimilarity [node1 node2]
  ;;if-else node1 != node2
    ;;[
    let a [currentDocs] of turtle node1                  ;list of documents from node1 
    ;;print a                     ;temporary
    let b [currentDocs] of turtle node2                  ;list of documents from node2
    ;print b                     ;temporary
    let numDocs 0                                        ;initialize the total number of similar documents as 0
    foreach a[
      let elementA ?                                     
      if member? elementA b [                            ;If the document is in both lists A and B, then increase numDocs by 1
        set numDocs numDocs + 1  
      ]
    ]
    let percentageA 0                                    ;Similarity percentage initialised as 0
    if length a > 0
    [set percentageA (numDocs / (length a))]             ;If the length of the list isn't 0 (don't want to divide by 0)
    ;;print percentageA
    ;print node2
    let percentageB 0
    if length b > 0
    [set percentageB (numDocs / (length b))]
    ;;print percentageB
    let average (((percentageA + percentageB) / 2) * 100) 
    report average

end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Adds a link between 2 peers and adds them as friends on there currentFriends list ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to addFriend [peer1 peer2]
  
  
  ;let temp sorted-turtle-set
  
  let T-peer1 turtle peer1
  let T-peer2 turtle peer2
  ;let T-peer2 item peer2 temp
  
  
  ask T-peer1 [
    if not link-neighbor? T-peer2 or T-peer1 != T-peer2
    [
      create-link-with T-peer2                        ;Create a link between peer1 and peer2
      ;;ask T-peer2[create-link-with T-peer1]
      set currentFriends lput peer2 currentFriends    ;add peer2 to peer1's friendList
      
      if VERBOSE? [ Type "connect:" type peer1 type ":" print peer2 ]
      if toFile? [file-type (ticks * 10000) file-type ":" file-type "connect" file-type ":" file-type peer1 file-type ":" file-print peer2 ] ;output the connect message
      
      set currentFriends remove-duplicates currentFriends       ;if friend was already there (i.e your going online and your just want to reconnect, we need to remove the second instance of that friend
      ask T-peer2[
         set currentFriends lput peer1 currentFriends
         
         if VERBOSE? [ Type "connect:" type peer2 type ":" print peer1 ]
         if toFile? [file-type (ticks * 10000) file-type ":" file-type "connect" file-type ":" file-type peer2 file-type ":" file-print peer1 ]
         
         set currentFriends remove-duplicates currentFriends
      ]
    ]
    
  ]
end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Adds links between a peer and all his friends (that are currently online) ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

to show-friend-connections [peer-ID] ;
  ;let temp sorted-turtle-set
  ;let peer item peer-ID temp
  let peer turtle peer-ID
  
  ask peer[
    foreach currentFriends[      ;Go through each peer in the friends List
      if is-online? ? [          ;If that friend is online, connect 
        
        addFriend peerID ? 
      
      ] 
    ] 
  ]
end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Remove all the links associated with a peer (i.e that peer is going offline) ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to hide-friend-connections [peer-ID]
  ;let temp sorted-turtle-set
  ;let peer item peer-ID temp
  let peer turtle peer-ID
  
  ask peer[
    ask my-links [  ;output the disconnect message and kill all the links (we're going offline, meaning we disconnect from everything)
      
      if VERBOSE? [Type "disconnect:" type peer-ID type ":" print [peerID] of other-end]
      if VERBOSE? [Type "disconnect:" type [peerID] of other-end type ":" print peer-ID]
      if toFile? [ file-type (ticks * 10000) file-type ":" file-type "disconnect" file-type ":" file-type peer-ID file-type ":" file-print [peerID] of other-end]
      if toFile? [ file-type (ticks * 10000) file-type ":" file-type "disconnect" file-type ":" file-type [peerID] of other-end file-type ":" file-print peer-ID]
      die 
      
      ] 
  ]
end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Removes the connection between two specific peers   ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to remove-friend [peer-ID otherID] 
  ;let temp sorted-turtle-set
  
  ;let peer1 item peer-ID temp
  ;let peer2 item otherID temp
  let peer1 turtle peer-ID
  let peer2 turtle otherID
  
  ask peer1 [
    ask my-links with [other-end = peer2 ] [ die ]      ;Kill the link between peer-ID and otherID, then output the disconnect messages
    set currentFriends remove otherID currentFriends
    if VERBOSE? [Type "disconnect:" type peer-ID type ":" print otherID]
    if toFile? [ file-type (ticks * 10000) file-type ":" file-type "disconnect" file-type ":" file-type peerID file-type ":" file-print otherID ]
    ask peer2 [
      set currentFriends remove peer-ID currentFriends
      if VERBOSE? [Type "disconnect:" type otherID type ":" print peer-ID]
      if toFile? [ file-type (ticks * 10000) file-type ":" file-type "disconnect" file-type ":" file-type otherID file-type ":" file-print peer-ID ]
    ] 
    
  ]
end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Returns a list of all the turtles sorted by there peerID  ;;;; <-- currently not needed (turtles "who" value == peerID)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to-report sorted-turtle-set
  let temp sort-by [[peerID] of ?1 < [peerID] of ?2] turtles
  report temp
end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Returns a boolean value of the current status of the peer (returns TRUE for online and FALSE for offline)  ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to-report is-online? [peer-ID] 
  ;let temp sorted-turtle-set
  let online false
  ask turtle peer-ID[
    set online online?
  ]
  report online
end







to publish [ document ]
  if VERBOSE? [ Type "publish:" type who type ":" print document ] ;prints this out (info on what's happening) (publish:peer:doc)
  set currentDocs fput document currentDocs
  set currentDocs remove-duplicates currentDocs
  if length currentDocs > MAXDOCS [ delete-document last currentDocs ]
end


  


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  
;;;; LAYOUT : credit goes to the maker of the giant component model ;;;; <--- Credit goes to Uri Wilensky, the one who made the Giant Component Model in the models library
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  
to layout
  if not layout? [ stop ]
  ;; the number 10 here is arbitrary; more repetitions slows down the
  ;; model, but too few gives poor layouts
  repeat 10 [
    do-layout
    display  ;; so we get smooth animation
  ]
end

to do-layout
  layout-spring (turtles with [any? link-neighbors]) links 0.4 6 1
end
  
  
  


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Finds the shortest path between 2 nodes, if the nodes aren't connected in anyway (no path) then it returns -1 ;;;;
;;;; Recursively explores the network, finding all possible paths between the start peer and the end peer, then    ;;;;
;;;; returns the shortest path length                                                                              ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  
to-report peer-distance [start finish] 
  let currentPath 0
  let shortestPath -1
  report find-shortest-path start finish currentPath shortestPath 
  
  
end

to-report find-shortest-path [start finish currentPath shortestPath]
  let result shortestPath
  let pathLength currentPath
  ifelse start = finish 
    [
       ifelse result = -1 or shortestPath >= pathLength
        [ report pathLength ]
        [ report result ]
    ]
    
    [ let peer1 turtle start
      ask peer1 [set explored? true
        foreach sort-by [[who] of ?1 < [who] of ?2] link-neighbors [
        
             ask ?[
                if not explored? [
                set result find-shortest-path peerID finish (pathLength + 1) result
                ]
             ]
            
            
        ]
      set explored? false
  
      ]
      report result
    ]
  
end
    



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Setups an initial friends List for each peer                   ;;;;
;;;; Selected Friends have a 15% chance of being completely random  ;;;;
;;;; The other 85% chance simply picks the most similar peers       ;;;;
;;;; This is highly influenced by the initial document distribution ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;    
to setup-friends
  ask turtles[
    let howMany? precision (random (numPeers / 10)) 0      ;Number of peers to add to friendList
    while [howMany? > 0] [                                 ;Loop until that many friends were added 
      
      if-else random 100 < 15                              ;if less than 15, add a random peer, else pick a similar one
      [ pick-random-peer ]                                 
      [ pick-similar-peer ]
      set howMany? howMany? - 1
        
    ]
  ]
end

to pick-random-peer        
  let person random NumPeers                               ;random peer
  if-else member? person CurrentFriends                    ;If that peer isn't in the friends list, add that peer
  [pick-random-peer]                                       ;if that peer is there, than try again
  [set currentFriends lput person currentFriends]
end 

to pick-similar-peer
  let similarity 0
  let most-similar-peer -1
  foreach [who] of turtles[
    if not member? ? currentFriends and ? != who
    [
      let a checkSimilarity-atStart who ?
      if a > similarity 
      [
        set similarity a
        set most-similar-peer ?  
      ]
    ] 
    
  ]
  if most-similar-peer != -1
    [set currentFriends lput most-similar-peer currentFriends]
end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Checks for similarity in the start repositories (before publishing them) between 2 peers ;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
to-report checkSimilarity-atStart [node1 node2]
  ;;if-else node1 != node2
    ;;[
    let a [DocsToPublish] of turtle node1
    ;;print a                     ;temporary
    let b [DocsToPublish] of turtle node2
    ;print b                     ;temporary
    let numDocs 0
    foreach a[
      let elementA ?
      if member? elementA b [
        set numDocs numDocs + 1  
      ]
    ]
    let percentageA 0
    if length a > 0
    [set percentageA (numDocs / (length a))]
    ;;print percentageA
    ;print node2
    let percentageB 0
    if length b > 0
    [set percentageB (numDocs / (length b))]
    ;;print percentageB
    let average (((percentageA + percentageB) / 2) * 100)
    report average

end


to-report checkHappiness
  
  let temp 0
  if length currentFriends > 0 [set temp (peer-popularity peerID / length currentFriends) ]
  let docPop searchHits
  let temp2 0
  if peersOnline > 1 [  set temp2 docPop / (peersOnline - 1)  ]
  report precision (((temp + temp2) / 2) * 100) 0
  
end
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
@#$#@#$#@
GRAPHICS-WINDOW
420
19
859
479
16
16
13.0
1
10
1
1
1
0
1
1
1
-16
16
-16
16
1
1
1
ticks
30.0

BUTTON
20
24
83
57
NIL
setup
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
92
25
155
58
NIL
go
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
169
23
232
56
NIL
step
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SWITCH
20
74
134
107
Show-IDs?
Show-IDs?
0
1
-1000

PLOT
18
241
403
476
plot 1
Ticks
Hits
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -16777216 true "" "plot totalHitsThisTick"

PLOT
900
30
1253
269
plot 2
Ticks
Searches
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -16777216 true "" "plot searches"

MONITOR
253
24
332
69
NIL
peersOnline
17
1
11

@#$#@#$#@
## WHAT IS IT?

This model is a simulation of a peer-to-peer network where each peer learns in a sense that they improve there surroundings to increase their results.

## HOW IT WORKS

-needs work

## HOW TO USE IT

Press the setup button, then press go or step, which ever the user prefers.
-make sure to have the correct input files (MultiPeer.txt is the current File Name)

## THINGS TO NOTICE

(suggested things for the user to notice while running the model)

## THINGS TO TRY

(suggested things for the user to try to do (move sliders, switches, etc.) with the model)

## EXTENDING THE MODEL

(suggested things to add or change in the Code tab to make the model more complicated, detailed, accurate, etc.)

## NETLOGO FEATURES

(interesting or unusual features of NetLogo that the model uses, particularly in the Code tab; or where workarounds were needed for missing features)

## RELATED MODELS

P2PSimulationRS is a similar model.

## CREDITS AND REFERENCES

(a reference to the model's URL on the web if it has one, as well as any other necessary credits, citations, and links)
@#$#@#$#@
default
true
0
Polygon -7500403 true true 150 5 40 250 150 205 260 250

airplane
true
0
Polygon -7500403 true true 150 0 135 15 120 60 120 105 15 165 15 195 120 180 135 240 105 270 120 285 150 270 180 285 210 270 165 240 180 180 285 195 285 165 180 105 180 60 165 15

arrow
true
0
Polygon -7500403 true true 150 0 0 150 105 150 105 293 195 293 195 150 300 150

box
false
0
Polygon -7500403 true true 150 285 285 225 285 75 150 135
Polygon -7500403 true true 150 135 15 75 150 15 285 75
Polygon -7500403 true true 15 75 15 225 150 285 150 135
Line -16777216 false 150 285 150 135
Line -16777216 false 150 135 15 75
Line -16777216 false 150 135 285 75

bug
true
0
Circle -7500403 true true 96 182 108
Circle -7500403 true true 110 127 80
Circle -7500403 true true 110 75 80
Line -7500403 true 150 100 80 30
Line -7500403 true 150 100 220 30

butterfly
true
0
Polygon -7500403 true true 150 165 209 199 225 225 225 255 195 270 165 255 150 240
Polygon -7500403 true true 150 165 89 198 75 225 75 255 105 270 135 255 150 240
Polygon -7500403 true true 139 148 100 105 55 90 25 90 10 105 10 135 25 180 40 195 85 194 139 163
Polygon -7500403 true true 162 150 200 105 245 90 275 90 290 105 290 135 275 180 260 195 215 195 162 165
Polygon -16777216 true false 150 255 135 225 120 150 135 120 150 105 165 120 180 150 165 225
Circle -16777216 true false 135 90 30
Line -16777216 false 150 105 195 60
Line -16777216 false 150 105 105 60

car
false
0
Polygon -7500403 true true 300 180 279 164 261 144 240 135 226 132 213 106 203 84 185 63 159 50 135 50 75 60 0 150 0 165 0 225 300 225 300 180
Circle -16777216 true false 180 180 90
Circle -16777216 true false 30 180 90
Polygon -16777216 true false 162 80 132 78 134 135 209 135 194 105 189 96 180 89
Circle -7500403 true true 47 195 58
Circle -7500403 true true 195 195 58

circle
false
0
Circle -7500403 true true 0 0 300

circle 2
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240

cow
false
0
Polygon -7500403 true true 200 193 197 249 179 249 177 196 166 187 140 189 93 191 78 179 72 211 49 209 48 181 37 149 25 120 25 89 45 72 103 84 179 75 198 76 252 64 272 81 293 103 285 121 255 121 242 118 224 167
Polygon -7500403 true true 73 210 86 251 62 249 48 208
Polygon -7500403 true true 25 114 16 195 9 204 23 213 25 200 39 123

cylinder
false
0
Circle -7500403 true true 0 0 300

dot
false
0
Circle -7500403 true true 90 90 120

face happy
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 255 90 239 62 213 47 191 67 179 90 203 109 218 150 225 192 218 210 203 227 181 251 194 236 217 212 240

face neutral
false
0
Circle -7500403 true true 8 7 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Rectangle -16777216 true false 60 195 240 225

face sad
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 168 90 184 62 210 47 232 67 244 90 220 109 205 150 198 192 205 210 220 227 242 251 229 236 206 212 183

fish
false
0
Polygon -1 true false 44 131 21 87 15 86 0 120 15 150 0 180 13 214 20 212 45 166
Polygon -1 true false 135 195 119 235 95 218 76 210 46 204 60 165
Polygon -1 true false 75 45 83 77 71 103 86 114 166 78 135 60
Polygon -7500403 true true 30 136 151 77 226 81 280 119 292 146 292 160 287 170 270 195 195 210 151 212 30 166
Circle -16777216 true false 215 106 30

flag
false
0
Rectangle -7500403 true true 60 15 75 300
Polygon -7500403 true true 90 150 270 90 90 30
Line -7500403 true 75 135 90 135
Line -7500403 true 75 45 90 45

flower
false
0
Polygon -10899396 true false 135 120 165 165 180 210 180 240 150 300 165 300 195 240 195 195 165 135
Circle -7500403 true true 85 132 38
Circle -7500403 true true 130 147 38
Circle -7500403 true true 192 85 38
Circle -7500403 true true 85 40 38
Circle -7500403 true true 177 40 38
Circle -7500403 true true 177 132 38
Circle -7500403 true true 70 85 38
Circle -7500403 true true 130 25 38
Circle -7500403 true true 96 51 108
Circle -16777216 true false 113 68 74
Polygon -10899396 true false 189 233 219 188 249 173 279 188 234 218
Polygon -10899396 true false 180 255 150 210 105 210 75 240 135 240

house
false
0
Rectangle -7500403 true true 45 120 255 285
Rectangle -16777216 true false 120 210 180 285
Polygon -7500403 true true 15 120 150 15 285 120
Line -16777216 false 30 120 270 120

leaf
false
0
Polygon -7500403 true true 150 210 135 195 120 210 60 210 30 195 60 180 60 165 15 135 30 120 15 105 40 104 45 90 60 90 90 105 105 120 120 120 105 60 120 60 135 30 150 15 165 30 180 60 195 60 180 120 195 120 210 105 240 90 255 90 263 104 285 105 270 120 285 135 240 165 240 180 270 195 240 210 180 210 165 195
Polygon -7500403 true true 135 195 135 240 120 255 105 255 105 285 135 285 165 240 165 195

line
true
0
Line -7500403 true 150 0 150 300

line half
true
0
Line -7500403 true 150 0 150 150

pentagon
false
0
Polygon -7500403 true true 150 15 15 120 60 285 240 285 285 120

person
false
0
Circle -7500403 true true 110 5 80
Polygon -7500403 true true 105 90 120 195 90 285 105 300 135 300 150 225 165 300 195 300 210 285 180 195 195 90
Rectangle -7500403 true true 127 79 172 94
Polygon -7500403 true true 195 90 240 150 225 180 165 105
Polygon -7500403 true true 105 90 60 150 75 180 135 105

plant
false
0
Rectangle -7500403 true true 135 90 165 300
Polygon -7500403 true true 135 255 90 210 45 195 75 255 135 285
Polygon -7500403 true true 165 255 210 210 255 195 225 255 165 285
Polygon -7500403 true true 135 180 90 135 45 120 75 180 135 210
Polygon -7500403 true true 165 180 165 210 225 180 255 120 210 135
Polygon -7500403 true true 135 105 90 60 45 45 75 105 135 135
Polygon -7500403 true true 165 105 165 135 225 105 255 45 210 60
Polygon -7500403 true true 135 90 120 45 150 15 180 45 165 90

sheep
false
0
Rectangle -7500403 true true 151 225 180 285
Rectangle -7500403 true true 47 225 75 285
Rectangle -7500403 true true 15 75 210 225
Circle -7500403 true true 135 75 150
Circle -16777216 true false 165 76 116

square
false
0
Rectangle -7500403 true true 30 30 270 270

square 2
false
0
Rectangle -7500403 true true 30 30 270 270
Rectangle -16777216 true false 60 60 240 240

star
false
0
Polygon -7500403 true true 151 1 185 108 298 108 207 175 242 282 151 216 59 282 94 175 3 108 116 108

target
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240
Circle -7500403 true true 60 60 180
Circle -16777216 true false 90 90 120
Circle -7500403 true true 120 120 60

tree
false
0
Circle -7500403 true true 118 3 94
Rectangle -6459832 true false 120 195 180 300
Circle -7500403 true true 65 21 108
Circle -7500403 true true 116 41 127
Circle -7500403 true true 45 90 120
Circle -7500403 true true 104 74 152

triangle
false
0
Polygon -7500403 true true 150 30 15 255 285 255

triangle 2
false
0
Polygon -7500403 true true 150 30 15 255 285 255
Polygon -16777216 true false 151 99 225 223 75 224

truck
false
0
Rectangle -7500403 true true 4 45 195 187
Polygon -7500403 true true 296 193 296 150 259 134 244 104 208 104 207 194
Rectangle -1 true false 195 60 195 105
Polygon -16777216 true false 238 112 252 141 219 141 218 112
Circle -16777216 true false 234 174 42
Rectangle -7500403 true true 181 185 214 194
Circle -16777216 true false 144 174 42
Circle -16777216 true false 24 174 42
Circle -7500403 false true 24 174 42
Circle -7500403 false true 144 174 42
Circle -7500403 false true 234 174 42

turtle
true
0
Polygon -10899396 true false 215 204 240 233 246 254 228 266 215 252 193 210
Polygon -10899396 true false 195 90 225 75 245 75 260 89 269 108 261 124 240 105 225 105 210 105
Polygon -10899396 true false 105 90 75 75 55 75 40 89 31 108 39 124 60 105 75 105 90 105
Polygon -10899396 true false 132 85 134 64 107 51 108 17 150 2 192 18 192 52 169 65 172 87
Polygon -10899396 true false 85 204 60 233 54 254 72 266 85 252 107 210
Polygon -7500403 true true 119 75 179 75 209 101 224 135 220 225 175 261 128 261 81 224 74 135 88 99

wheel
false
0
Circle -7500403 true true 3 3 294
Circle -16777216 true false 30 30 240
Line -7500403 true 150 285 150 15
Line -7500403 true 15 150 285 150
Circle -7500403 true true 120 120 60
Line -7500403 true 216 40 79 269
Line -7500403 true 40 84 269 221
Line -7500403 true 40 216 269 79
Line -7500403 true 84 40 221 269

wolf
false
0
Polygon -7500403 true true 135 285 195 285 270 90 30 90 105 285
Polygon -7500403 true true 270 90 225 15 180 90
Polygon -7500403 true true 30 90 75 15 120 90
Circle -1 true false 183 138 24
Circle -1 true false 93 138 24

x
false
0
Polygon -7500403 true true 270 75 225 30 30 225 75 270
Polygon -7500403 true true 30 75 75 30 270 225 225 270

@#$#@#$#@
NetLogo 5.0.1
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
default
0.0
-0.2 0 1.0 0.0
0.0 1 1.0 0.0
0.2 0 1.0 0.0
link direction
true
0
Line -7500403 true 150 150 90 180
Line -7500403 true 150 150 210 180

@#$#@#$#@
0
@#$#@#$#@
