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
	
	void readESDGFromFile(char* esdg);
	void printESDG();
	void getVertexCount(int &n);
	// void fastestPathDuration(int src, ofstream &oFile);
	// void fastestPathDuration(int src, int *duration,int &count);
	void esdg_bfs(int &count);
	void esdg_dfs(int &count);
	// void printfpd(ofstream &oFile, int *A);
	void getStstastics();


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

void esdg::getStstastics()
{
	cout<<"Number of Nodes: "<<nCount<<endl;
	cout<<"Number of Dependencies: "<<dCount<<endl;

	queue<int> q;
	bool *visited;
	int count=0,c=0,max=0,min=INT_MAX;
	visited= new bool[nCount];

	for(int node=0; node < nCount; ++node)
		visited[node]=false;



	for(int node=0; node < nCount; ++node)
	{
		if(visited[node]==false){
			q.push(node);
			visited[node]=true;
			count++;
			// cout<<endl;
		}
		c=0;
		while(!q.empty())
		{
			int currConnection; 
			currConnection= q.front(); q.pop();
			// cout<<currConnection<<" ";
			c++;
			for(int node=vArray[currConnection]; node < vArray[currConnection+1]; ++node)
			{
				if(visited[eArray[node]]==false)
				{
					q.push(eArray[node]);
					visited[eArray[node]]=true;
				}
			
			}
			
		}

		if(c>max)
			max=c;
		if(c<min && c!=0)
			min=c;
		// cout<<endl;
	}
	cout<<"No of Disconnected Components: "<<count<<endl;
	cout<<"Maximum Number of nodes in a component: "<<max<<endl;
	cout<<"Minimum Number of elements in a component: "<<min<<endl;
}
void esdg::esdg_bfs(int & count)
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


void esdg::esdg_dfs(int &count)
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


void esdg::readESDGFromFile(char* esdg)
{
    ifstream fEsdg(esdg);

    int n;
    fEsdg >> nCount;
    fEsdg >> dCount;
    fEsdg >> vCount;

    vArray = new int[nCount+1];
    eArray = new int[dCount];

	for(int node=0; node < nCount; ++node)
		fEsdg >> vArray[node];
	vArray[nCount]=dCount;
	for(int neighbor=0; neighbor < dCount; ++neighbor)
		fEsdg >> eArray[neighbor];

    fEsdg.close();
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
}

int main(int argc, char*argv[])
{
	char *ip_esdg;
	esdg G;
	// int *fpd;				// Array to store the fastest path duration from source to the respective vertex
	double total_time=0.0,q_time=0.0;
    clock_t start,end;
    int count1=0,count2=0;

	assert(argc==2);
    ip_esdg=argv[1];
    // ip_connections=argv[2];
    // ip_qfile=argv[2];
    // op_fpdfile=argv[4];


	G.readESDGFromFile(ip_esdg);
	G.getStstastics();
	
	// G.printESDG();
	// G.noOfQueries=1;

	// ofstream oFile;
	// oFile.open(op_fpdfile);

	// int n;   //number of vertices in temporal graph
	// G.getVertexCount(n);

	// fpd	= new int[n];

	// for(int queryIndex=0; queryIndex<G.noOfQueries; ++queryIndex){
		start=clock();
		// G.esdg_bfs(count1);
		end=clock();
		q_time=double(end-start)/CLOCKS_PER_SEC;
		// total_time+=q_time;
	// }
	// cout<<"Average Number of connections Touched : "<< nofConnectionsTouched/G.noOfQueries<<endl;
	
	
	// for(int queryIndex=0; queryIndex<G.noOfQueries; ++queryIndex){
		start=clock();
		// G.esdg_dfs(count2);
		end=clock();
		total_time=double(end-start)/CLOCKS_PER_SEC;
	// }

	// cout<<"\nBFS:\n";
	// cout<<" Run Time:"<<(q_time)*1000<<"ms"<<endl;
	// cout<<"No of Disconnected Components: "<<count1<<endl;
	// cout<<"\nDFS:\n";
	// cout<<" Run Time:"<<(total_time)*1000<<"ms"<<endl;
	// cout<<"No of Disconnected Components: "<<count2<<endl;
	// // cout<<"Average Number of connections Touched : "<< nofConnectionsTouched/G.noOfQueries<<endl;
	
	return 0;
}
