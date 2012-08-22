#include "SGraph.h"
#define GVERBOSE false

//adds a query
bool SGraph::addquery(int query){
	return addpeer(query);
}

//removes a query
bool SGraph::removequery(int query){
	return removepeer(query);
}

//creates a link between a query and a document
bool SGraph::querydoclink(int query, int doc){
	return publish(query, doc);
}

//removes a link between the specified query and document
bool SGraph::removequerylink(int query, int doc){
	return remove(query, doc);
}

//Returns a set of all the documents linked to by a specified query
set<int> SGraph::getDocsFromQuery(int query){
	return getsDocsOfPeer(query);
}

set<int> SGraph::getDocsFromLink(int d){
	return getDocsFromQuery(d*(-1));
}

set<int> SGraph::getDocsFromLink(int d, int lbl){
	return getsDocsFromLink(d,lbl);
}





/*read a graph from an input stream (supposed to be a file is but not necessary
 * this does not allow multiple links between docs... TODO: add support for multiple links
 * Expected format (example):
 * 3 6 	// number of query, number of documents
 * 0 0 0 1 1 0 //query-doc adjacency matrix :// query 1 is connected to docs 4 and 5
 * 0 1 0 0 1 1 // query 2 is connected to doc 2, doc 5, doc 6
 * 1 0 0 0 0 0 // query 3 is connected to doc 1
 * 0 0 2 0 0 0 // doc-doc adjacency matrix starts here. This graph must be acyclic.
 * 6 0 0 0 0 0 // doc 2 links to doc 1 with label "6"
 * 0 6 0 0 0 0
 * 0 0 0 0 0 0
 * 0 4 0 0 0 0
 * 0 4 0 0 3 0 // doc 6 links to doc 2 with label 4 and to doc 5 with label 3.
 *
 * Here for a query / document server we need a structure where the query and documents are separate numerical intervals.
 * */
void SGraph::read(std::istream & is)
{

	//TODO: make this work for queries / docs
	int i, q, d; //index, number of queries, number of docs

	/* read number of queries */
	is >> q;
	is >> d;

	/* label the queries from 1 to p, insert them */
	for (i = 1; i < q+1; i++)
		addquery(i);

	//insert the docs labeled 1 to d
	for (i = 1; i < d+1; i++)
			adddoc(i);

	//The query-peer adjacency matrix (positive values, 0 means no link):"
	for (i = 1; i < q+1; i++)
	{
		//node_iterator ni = this->find(i);
		for (int j = 1; j < d+1; j++)
		{
			int w;
			is >> w;
			if (w > 0){
				querydoclink(i, j);
			}
		}
	}
	//the doc-doc adjacency matrix
	for (i = 1; i < d+1; i++)
		{
			//node_iterator ni = this->find(i);
			for (int j = 1; j < d+1; j++)
			{
				int w;
				is >> w;
				if (w > 0)
					linkdocs(i, j, w);

			}
		}

}

/*read a graph from an input stream (supposed to be a file is but not necessary
 * expected format (example):
 * 31: 45 56 67  <---document 31 is linked to documents 45,56 and 67
 * 4: 5 6 7
 * 5: 6 7 8
 * 86:           <--- document 86 not linked to any other documents
 * 51: 9 76 32 7
 */
void SGraph::read2(std::istream & is){

	int doc;
	int link;
	string document = ""; //Stores the document numbers taken from file (one at a time)
	string line = "";  //Stores a line from the file
	while(!is.eof()){
		getline(is,line);
		bool firstspace = false;

		//Loop through the string line to get information on documents and there links
		for(int i = 0; i != line.length();i++){

			//On a whitespace, converts the string document into an integer (nbr of the document) then makes the link and clears the string
			if(line[i] == ' '){
				//if(PVERBOSE) cout << "empty space"<<endl;
				if(firstspace == true){
					link = atoi(document.c_str());
					linkdocs(doc, link, 1);
					//document = "";
					while(document.length() > 0){
						document.erase(document.length()-1);
					}
				}
				else{
					firstspace = true;
				}

			}
			//On a colon, converts the string document into an integer (nbr of the document) then adds the document to the graph and clears the string
			else if(line[i] == ':'){
				doc = atoi(document.c_str());
				adddoc(doc);
				while(document.length() > 0){
					document.erase(document.length()-1);
				}
			}
			//On a digit, store the value into the string
			else{
				document += line[i];

			}
		}
		//Because there's no whitespace at the end, need to add the final link.
		if(firstspace){
			link = atoi(document.c_str());
			linkdocs(doc,link,1);
			while(document.length() > 0){
				document.erase(document.length()-1);
			}
		}





	}


}


