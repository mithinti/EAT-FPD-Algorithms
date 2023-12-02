#include<iostream>
#include<cassert>
#include<fstream>
#include<queue>
#include<stack>
#include<algorithm>
#include<limits.h>
#include<sys/time.h>

// #include<vector>

using namespace std;


class esdg
{
	int vCount;			// Number of Vertices present in the Original Graph
	int nCount;			// Number of connections in the original Graph or nodes in ESDG Graph
	int dCount;			// Number of dependencies in the original graph or edges in the ESDG Graph
	// int lCount;			// Number of levels in the ESDG Graph.

	int *vArray;		// Offset array of ESDG Graph
	int *eArray;		// Neighbouring array of Vertices present in the ESDG Graph

	// Graph *cArray;		// Node to connection mapping

	
	//Functions
public: 
	int *src;			// Query array consists of source vertices
	int noOfQueries;
	// int vCount;			// Number of Vertices present in the Original Graph

	void readESDGFromFile(char* esdg, char *qFile);
	void printESDG();
	void getVertexCount(int &n);
	// void fastestPathDuration(int src, ofstream &oFile);
	// void fastestPathDuration(int src, int *duration,int &count);
	void esdg_bfs(int src);
	void esdg_dfs(int src);
	// void printfpd(ofstream &oFile, int *A);


};
void esdg::getVertexCount(int &n)
{
	n=vCount;
}

/*void esdg::printfpd(ofstream &oFile, int *A)
{
	for(int i=0;i<vCount;i++)
		oFile<<i<<" "<<A[i]<<endl;
}*/

void esdg::esdg_bfs(int src)
{
	queue<int> q;
	bool *visited;
	visited= new bool[nCount];

	for(int node=0; node < nCount; ++node)
		visited[node]=false;

	q.push(src);
	visited[src]=true;
	while(!q.empty())
	{
		int currConnection; 
		currConnection= q.front(); q.pop();
		// cout<<currConnection<<" ";
		for(int node=vArray[currConnection]; node < vArray[currConnection+1]; ++node)
		{
			if(visited[eArray[node]]==false)
			{
				q.push(eArray[node]);
				visited[eArray[node]]=true;
			}
		
		}
		
	}
	// cout<<endl;
}


void esdg::esdg_dfs(int src)
{
	stack<int> S;
	bool *visited;
	visited= new bool[nCount];

	for(int node=0; node < nCount; ++node)
		visited[node]=false;

	S.push(src);
	while(!S.empty())
	{
		int currConnection; 
		currConnection= S.top(); S.pop();
		if(visited[currConnection]==false)
		{
			// cout<<currConnection<<" ";
			visited[currConnection]=true;
		}
		for(int node=vArray[currConnection]; node < vArray[currConnection+1]; ++node)
		{
			if(visited[eArray[node]]==false)
			{
				S.push(eArray[node]);
			}
		
		}
		
	}
	// cout<<endl;
}


void esdg::readESDGFromFile(char* esdg, char *qFile)
{
    ifstream fEsdg(esdg);
    // ifstream fGraph(conn);
    ifstream fQuery(qFile);

    int n;
    fEsdg >> nCount;
    fEsdg >> dCount;
    fEsdg >> vCount;

    vArray = new int[nCount+1];
    eArray = new int[dCount];

    /*for(int node=0; node < nCount+1; ++node)
		fEsdg >> vArray[node];
*/
	for(int node=0; node < nCount; ++node)
		fEsdg >> vArray[node];
	vArray[nCount]=dCount;
	for(int neighbor=0; neighbor < dCount; ++neighbor)
		fEsdg >> eArray[neighbor];

    fEsdg.close();

    /*cArray = new Graph[nCount];

    int m1,m2;
    fGraph>>vCount>>m1>>m2>>lCount;

    int id;
	for(int index=0; index < nCount; ++index)
		fGraph >> id >> cArray[index].u >> cArray[index].v >> cArray[index].t >> cArray[index].lambda >> cArray[index].levelNo;

	fGraph.close();*/

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
// getchar();
	cout << "\nNeighbors Array";
	for(int neighbor=0; neighbor < dCount; ++neighbor)
		cout << eArray[neighbor] << " ";

	/*cout << "\nConnections Array";
	for(int index=0; index < nCount; ++index)
		cout<<"\n"<< index<< " " << cArray[index].u << " "<< cArray[index].v<< " " << cArray[index].t << " " << cArray[index].lambda << " " << cArray[index].levelNo;
		// cout << "\n" << index << " " << G.u[index] << " " << G.v[index] << " " << G.t[index] << " " << G.lambda[index];
*/
	cout << "\nQuery Array";
	for(int index=0; index < noOfQueries; ++index)
		cout << "\n" << src[index];

	// cout<< "\nNumber of Vertices: "<< vCount <<"\nNumber of levels in ESDG Grpah:" <<lCount;

	// cout << "\n\n\n";

	
	/*for(int index=0; index < nCount; ++index)
	{
		cout << "\nNeighbors of "<< index<< " :";
		for(int neighbor=vArray[index]; neighbor < vArray[index+1]; ++neighbor)
			cout<<eArray[neighbor]<<" ";
		cout<<endl;
	}*/
}

int main(int argc, char*argv[])
{
	char *ip_esdg,*ip_qfile;
	esdg G;
	// int *fpd;				// Array to store the fastest path duration from source to the respective vertex
	double total_time=0.0,q_time=0.0;
    clock_t start,end;
    int nofConnectionsTouched=0;

	assert(argc==3);
    ip_esdg=argv[1];
    // ip_connections=argv[2];
    ip_qfile=argv[2];
    // op_fpdfile=argv[4];


	G.readESDGFromFile(ip_esdg,ip_qfile);
	
	// G.printESDG();
	// G.noOfQueries=1;

	// ofstream oFile;
	// oFile.open(op_fpdfile);

	int n;   //number of vertices in temporal graph
	// G.getVertexCount(n);

	// fpd	= new int[n];

	for(int queryIndex=0; queryIndex<G.noOfQueries; ++queryIndex){
		start=clock();
		G.esdg_bfs(G.src[queryIndex]);
		end=clock();
		q_time=double(end-start)/CLOCKS_PER_SEC;
		total_time+=q_time;
	}
	cout<<"\nBFS:\n";
	cout<<" Run Time:"<<(total_time/G.noOfQueries)*1000<<"ms"<<endl;
	// cout<<"Average Number of connections Touched : "<< nofConnectionsTouched/G.noOfQueries<<endl;
	
	total_time=0.0;
	q_time=0.0;
	cout<<"\nDFS:\n";
	for(int queryIndex=0; queryIndex<G.noOfQueries; ++queryIndex){
		start=clock();
		G.esdg_dfs(G.src[queryIndex]);
		end=clock();
		q_time=double(end-start)/CLOCKS_PER_SEC;
		total_time+=q_time;
	}
	cout<<" Run Time:"<<(total_time/G.noOfQueries)*1000<<"ms"<<endl;
	// cout<<"Average Number of connections Touched : "<< nofConnectionsTouched/G.noOfQueries<<endl;
	
	return 0;
}
