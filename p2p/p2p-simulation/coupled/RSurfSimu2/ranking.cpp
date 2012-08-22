#include "ranking.h"


/**
 * The parameters determine the actions done by this method
 *
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
 */
set<int> MostQhitfromFriends(int n, const list<int> &Hitlist, const set<int> &friendDocs, bool friendDocsOnly)
{
	map<int,int> docFreq;
	set<int> temp;
	for(list<int>::const_iterator it = Hitlist.begin(); it != Hitlist.end(); it++){

		if(docFreq.find(*it) == docFreq.end()){
			docFreq.insert(pair<int,int>(*it,1));
		}
		else{
			map<int,int>::iterator i = docFreq.find(*it);
			(*i).second += 1;
		}

	}

	map<int,int>::iterator b = docFreq.begin();
	if(friendDocsOnly)
	{
		set<int>::iterator friendIt;
		for(b ; b != docFreq.end(); b++)
		{
			friendIt = friendDocs.find((*b).first);
			if(friendIt == friendDocs.end())
			{
				docFreq.erase(b);
			}
		}
		b = docFreq.begin();
	}


	int currentMax = (*b).second;
	int previousMax = -1;
	int doc = (*b).first;
	for(int loops = n; loops != 0; loops--){

		for(b;b != docFreq.end();b++){
			if(previousMax == -1){
				if((*b).second > currentMax){
					currentMax = (*b).second;
				}
			}
			else{
				if((*b).second > currentMax && (*b).second < previousMax){
					currentMax = (*b).second;
				}
			}
		}
		for(b = docFreq.begin();b!=docFreq.end();b++)
		{
			if((*b).second == currentMax){
				temp.insert((*b).first);
			}
		}
		previousMax = currentMax;
		b = docFreq.begin();
		currentMax = (*b).second;
	}
//	for(set<int>::iterator a = temp.begin(); a != temp.end(); a++){
//		cout << "doc: " << *a << endl;
//	}
	return temp;
}
