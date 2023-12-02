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


class Graph
{
	int vCount;			// Number of Vertices present in the Original Graph
	int nCount;			// Number of connections in the original Graph or nodes in  Graph
	int PECount;		// Number of primary edges in the transformed Graph
	int CECount;		// Number of chain edges in the transformed Graph
	int dCount;			// Number of edges in the transformed Graph	
	int *vArray;		// Offset array of Graph
	int *eArray;		// Neighbouring array of Vertices present in the Graph

	// Graph *cArray;		// Node to connection mapping

	
	//Functions
public: 
	
	void readGraphFromFile(char* Graph);
	void printGrpah();
	void getVertexCount(int &n);
	// void fastestPathDuration(int src, ofstream &oFile);
	// void fastestPathDuration(int src, int *duration,int &count);
	void bfs(int &count);
	void dfs(int &count);
	// void printfpd(ofstream &oFile, int *A);


};
void Graph::getVertexCount(int &n)
{
	n=vCount;
}

/*void ::printfpd(ofstream &oFile, int *A)
{
	for(int i=0;i<vCount;i++)
		oFile<<i<<" "<<A[i]<<endl;
}*/

void Graph::bfs(int & count)
{
	queue<int> q;
	bool *visited;
	visited= new bool[nCount];

	for(int node=0; node < nCount; ++node)
		visited[node]=false;

	for(int node=0; node < nCount; ++node)
	{
		if(visited[node]==false){
			q.push(node);
			visited[node]=true;
			count++;
			cout<<endl;
		}
		while(!q.empty())
		{
			int currConnection; 
			currConnection= q.front(); q.pop();
			cout<<currConnection<<" ";
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
	
}


void Graph::dfs(int &count)
{
	stack<int> S;
	bool *visited;
	visited= new bool[nCount];

	for(int node=0; node < nCount; ++node)
		visited[node]=false;

	for(int node=0; node < nCount; ++node)
	{
		if(visited[node]==false){
			S.push(node);
			count++;
			cout<<endl;
		}
		while(!S.empty())
		{
			int currConnection; 
			currConnection= S.top(); S.pop();
			if(visited[currConnection]==false)
			{
				cout<<currConnection<<" ";
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
	
}


void Graph::readGraphFromFile(char* Graph)
{
    ifstream fGraph(Graph);

    int n;
    fGraph >> vCount;
    fGraph >> nCount;
    fGraph >> PECount;
    fGraph >> CECount;
    dCount = PECount + CECount;
    vArray = new int[nCount+1];
    eArray = new int[dCount];

    for (int lineno = 2; getline (fGraph,line) && lineno <= nCount; lineno++);

	for(int node=0; node < nCount; ++node)
		fGraph >> vArray[node];
	vArray[nCount]=dCount;
	for(int neighbor=0; neighbor < dCount; ++neighbor)
		fGraph >> eArray[neighbor];

    fGraph.close();
}

void Graph:: printGraph()
{
	cout << "\nOffset Array";
	for(int node=0; node < nCount+1; ++node)
		cout << vArray[node] << " ";
// getchar();
	cout << "\nNeighbors Array";
	for(int neighbor=0; neighbor < dCount; ++neighbor)
		cout << eArray[neighbor] << " ";
}

int main(int argc, char*argv[])
{
	char *ip_Graph;
	Graph G;
	// int *fpd;				// Array to store the fastest path duration from source to the respective vertex
	double total_time=0.0,q_time=0.0;
    clock_t start,end;
    int count1=0,count2=0;

	assert(argc==2);
    ip_Graph=argv[1];
    // ip_connections=argv[2];
    // ip_qfile=argv[2];
    // op_fpdfile=argv[4];


	G.readGraphFromFile(ip_Graph);
	
	// G.printGraph();
	// G.noOfQueries=1;

	// ofstream oFile;
	// oFile.open(op_fpdfile);

	// int n;   //number of vertices in temporal graph
	// G.getVertexCount(n);

	// fpd	= new int[n];

	// for(int queryIndex=0; queryIndex<G.noOfQueries; ++queryIndex){
		start=clock();
		G.bfs(count1);
		end=clock();
		q_time=double(end-start)/CLOCKS_PER_SEC;
		// total_time+=q_time;
	// }
	// cout<<"Average Number of connections Touched : "<< nofConnectionsTouched/G.noOfQueries<<endl;
	
	
	// for(int queryIndex=0; queryIndex<G.noOfQueries; ++queryIndex){
		start=clock();
		G.dfs(count2);
		end=clock();
		total_time=double(end-start)/CLOCKS_PER_SEC;
	// }

	cout<<"\nBFS:\n";
	cout<<" Run Time:"<<(q_time)*1000<<"ms"<<endl;
	cout<<"No of Disconnected Components: "<<count1<<endl;
	cout<<"\nDFS:\n";
	cout<<" Run Time:"<<(total_time)*1000<<"ms"<<endl;
	cout<<"No of Disconnected Components: "<<count2<<endl;
	// cout<<"Average Number of connections Touched : "<< nofConnectionsTouched/G.noOfQueries<<endl;
	
	return 0;
}
