In this coupled model, the behaviour of the peer is slightly different. When all the queryhits for a specific link have been received, the peer does
not simply choose one of those links at random. Instead, there are multiple different things he can do, depending on what parameters are given.


There are 4 parameters for this method, (integer n, list Queryhits, list FriendDocuments, boolean OnlyFromFriends)

The parameters do the following:




 * the first parameter, n, decides how many ranks of the most queryhit documents are considered
 * example: receives the following:
 *
 * 		doc 5: 5 qhits -> rank 2 (second most qhit, tied with doc 4)
 *  	doc 4: 5 qhits -> rank 2 (second most qhit, tied with doc 5)
 *  	doc 3: 2 qhits -> rank 3 (third most qhit)
 *  	doc 2: 8 qhits -> rank 1 (most qhit)
 *  	doc 1: 1 qhit  -> rank 4 (fourth most qhit, tied with doc 0)
 *  	doc 0: 1 qhit  -> rank 4 (fourth most qhit, tied with doc 1)
 *
 *  if n would be 1, it would only consider the rank 1 documents (in this case the peer would choose doc 2)
 *  if n would be 2, it would consider docs 2, 4, and 5, all docs from ranks 1 and 2, randomly choosing one of them
 *  ..............3, it would consider docs 2, 4, 5, and 3
 *  ..............4, etc
 *
 *  the list taken as the second parameter is a list of all the queryhits received. (in the case of using it for
 *  the randomsurfer model, this list would correspond to linkchoices.
 *
 *  the second list taken as a parameter (friendDocs), is a list of all the documents that got queryhit by a friend.
 *  This is used to seperate the documents if the 4th parameter (friendDocsOnly) is set to true.
 *
 *  This parameter tells us that we only want to choose documents that one of our friends has previously published.


 



				