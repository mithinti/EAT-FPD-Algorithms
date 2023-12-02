#include<iostream>
#include<cassert>
#include<fstream>
#include<queue>
#include<algorithm>
#include<limits.h>
#include<sys/time.h>

// #include<vector>

using namespace std;


typedef struct connection
{
	int u, v, t, lambda, levelNo;
}Graph;


class esdg
{
	int vCount;			// Number of Vertices present in the Original Graph
	int nCount;			// Number of connections in the original Graph or nodes in ESDG Graph
	int dCount;			// Number of dependencies in the original graph or edges in the ESDG Graph
	int lCount;			// Number of levels in the ESDG Graph.

	int *vArray;		// Offset array of ESDG Graph
	int *eArray;		// Neighbouring array of Vertices present in the ESDG Graph

	Graph *cArray;		// Node to connection mapping

	
	//Functions
public: 
	int *src;			// Query array consists of source vertices
	int noOfQueries;

	void readESDGFromFile(char* esdg, char* conn, char *qFile);
	void printESDG();
	void fastestPathDuration(int src, int* duration);
	void printfpd(ofstream &oFile, int *A);

};

void esdg::readESDGFromFile(char* esdg, char* conn, char *qFile)
{
	ifstream fEsdg(esdg);
    ifstream fGraph(conn);
    ifstream fQuery(qFile);

    fEsdg >> nCount;
    fEsdg >> dCount;

    vArray = new int[nCount+1];
    eArray = new int[dCount];

    for(int node=0; node < nCount+1; ++node)
		fEsdg >> vArray[node];

	for(int neighbor=0; neighbor < dCount; ++neighbor)
		fEsdg >> eArray[neighbor];

    fEsdg.close();

    cArray = new Graph[nCount];

    int temp;
    fGraph>>vCount>>temp>>lCount;

    int id;
	for(int index=0; index < nCount; ++index)
		fGraph >> id >> cArray[index].u >> cArray[index].v >> cArray[index].t >> cArray[index].lambda >> cArray[index].levelNo;

	fGraph.close();

	fQuery >> noOfQueries;

	src = new int[noOfQueries];
	for(int index=0; index < noOfQueries; ++index)
		fQuery >> src[index];
}

void esdg:: printESDG()
{
	cout << "\nOffset Array";
	for(int node=0; node < nCount+1; ++node)
		cout << vArray[node] << " ";

	cout << "\nNeighbors Array";
	for(int neighbor=0; neighbor < dCount; ++neighbor)
		cout << eArray[neighbor] << " ";

	cout << "\nConnections Array";
	for(int index=0; index < nCount; ++index)
		cout<<"\n"<< index<< " " << cArray[index].u << " "<< cArray[index].v<< " " << cArray[index].t << " " << cArray[index].lambda << " " << cArray[index].levelNo;
		// cout << "\n" << index << " " << G.u[index] << " " << G.v[index] << " " << G.t[index] << " " << G.lambda[index];

	cout << "\nQuery Array";
	for(int index=0; index < noOfQueries; ++index)
		cout << "\n" << src[index];

	cout<< "\nNumber of Vertices: "<< vCount <<"\nNumber of levels in ESDG Grpah:" <<lCount;

	cout << "\n\n\n";

	
	for(int index=0; index < nCount; ++index)
	{
		cout << "\nNeighbors of "<< index<< " :";
		for(int neighbor=vArray[index]; neighbor < vArray[index+1]; ++neighbor)
			cout<<eArray[neighbor]<<" ";
		cout<<endl;
	}





}

int main(int argc, char*argv[])
{
	char *ip_esdg, *ip_connections, *ip_qfile, *op_fpdfile;
	esdg G;
	int *fpd;				// Array to store the fastest path duration from source to the respective vertex
	

	assert(argc==5);
    ip_esdg=argv[1];
    ip_connections=argv[2];
    ip_qfile=argv[3];
    op_fpdfile=argv[4];


	G.readESDGFromFile(ip_esdg,ip_connections,ip_qfile);
	
	G.printESDG();
	
	// G.neighbours(i);

	// G.bestStarttime(i);
	// G.getconnection(i);
	return 0;
}