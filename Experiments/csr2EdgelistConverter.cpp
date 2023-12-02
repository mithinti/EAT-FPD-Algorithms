#include <iostream>
#include<utility>
#include<cassert>
#include<fstream>
#include<limits.h>
#include<vector>
#include<set>
#include<algorithm>
using namespace std;

class esdg
{
	int vCount;			// Number of Vertices present in the Original Graph
	int nCount;			// Number of connections in the original Graph or nodes in ESDG Graph
	int dCount;			// Number of dependencies in the original graph or edges in the ESDG Graph
	int lCount;			// Number of levels in the ESDG Graph.

	int *vArray;		// Offset array of ESDG Graph
	int *eArray;		// Neighbouring array of Vertices present in the ESDG Graph

	public:
		void readESDGFromFile(char* esdgCSR, char* esdgEdgeList);
};

void esdg::readESDGFromFile(char* esdgCSR, char* esdgEdgeList)
{
    ifstream fEsdgCSR(esdgCSR);
    ofstream fEsdgEdgeList(esdgEdgeList);
    
    int n;
    fEsdgCSR >> nCount;
    fEsdgCSR >> dCount;
    fEsdgCSR >> n;

    vArray = new int[nCount+1];
    eArray = new int[dCount];

	for(int node=0; node < nCount; ++node)
		fEsdgCSR >> vArray[node];
	vArray[nCount]=dCount;
	for(int neighbor=0; neighbor < dCount; ++neighbor)
		fEsdgCSR >> eArray[neighbor];

    fEsdgCSR.close();

    for(int node=0; node < nCount+1; ++node)
		for(int neighbor=vArray[node]; neighbor < vArray[node+1]; ++neighbor)
			fEsdgEdgeList << node <<","<<eArray[neighbor] << endl;
	fEsdgEdgeList.close();
}
int main(int argc, char*argv[])
{
	char *ip_esdg, *op_esdg;
	esdg G;
	assert(argc==3);
    ip_esdg=argv[1];
    op_esdg=argv[2];
    G.readESDGFromFile(ip_esdg,op_esdg);
    return 0;
}

	
