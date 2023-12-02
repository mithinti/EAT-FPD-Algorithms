/*
Given an ESDG graph along with source vertex, compute the earlieast arrival time from the given source vertex to all other vertices present in the temporal graph.
Status : Completed
*/
#include <iostream>
#include<queue>
#include<utility>
#include<cassert>
#include<fstream>
#include<limits.h>
using namespace std;

// temporal graph
typedef struct connections
{
	int *u,*v,*st,*lambda;
	int n,m;
}connections;

//ESDG Graph
typedef struct esdg
{
	int *offset,*neighbor;
	int n,m;
}esdg;

// Quries : source ready_time
typedef struct query
{
	int *sv,*t;
	int n;
}query;

void read_input(char *ip_esdg, char *ip_qfile, esdg &E, connections &G, query &Q)
{
    
    ifstream ipFile(ip_esdg);
    // ifstream ipFile1(ip_conn);
    ifstream ipFile2(ip_qfile);

    int temp;
    ipFile>> G.n;
    ipFile>> E.n;
    ipFile>> E.m;
    G.m=E.n;
    // ipFile>> G.m;
    ipFile>> temp;

    E.offset=new int[E.n+1];
    E.neighbor=new int[E.m];

    for(int i=0;i<=E.n;i++)
    {
        ipFile>>E.offset[i];
    }

    for(int i=0;i<E.m;i++)
    {
        ipFile>>E.neighbor[i];
    }

    // ipFile.close();


    // ipFile1>>G.n>>G.m;

    G.u=new int[G.m];
    G.v=new int[G.m];
    G.st=new int[G.m];
    G.lambda=new int[G.m];

    // int temp;

    for(int i=0;i<G.m;i++)
    {
        ipFile>>temp>>G.u[i]>>G.v[i]>>G.st[i]>>G.lambda[i]>>temp;
    }
    
    ipFile.close();

    ipFile2>>Q.n;

    Q.sv=new int[Q.n];
    Q.t=new int[Q.n];

    for(int i=0;i<Q.n;i++)
    {
    	ipFile2>>Q.sv[i]>>Q.t[i];
    }

    ipFile2.close();

}

bool relax(connections &G, int *eat, int i,int &count)
{
	int et=G.lambda[i]+G.st[i];

	count++;

	if(eat[G.u[i]]<=G.st[i] && eat[G.v[i]]>et)
	{
		eat[G.v[i]]=et;
		return true;
	}

	return false;
}

bool relax(connections &G, int *eat, int i)
{
	int et=G.lambda[i]+G.st[i];

	if(eat[G.u[i]]<=G.st[i] && eat[G.v[i]]>et)
	{
		eat[G.v[i]]=et;
		return true;
	}

	return false;
}


void earliestArrivalTime(esdg &E,connections &G,int s,int ts,int *&eat)
{
	queue<int> q;
	vector<bool> visited(G.m,false);

	//Intialization 
	// step 1: Push vertex in esdg graph having source s and starting time more that ts from all the connections in the graph
	// step 2: intialization of EAT[v] v in V

	for(int i=0;i<G.m;i++)
	{
		if(G.u[i]==s && G.st[i]>=ts)
		{
			q.push(i);
		}
	}

	//EAT Intilization
	for(int i=0;i<G.n;i++)
	{
		eat[i]=INT_MAX;
	}
	eat[s]=ts;

	// step 3: BFS

	while(!q.empty())
	{
		int currConnection = q.front(); q.pop();

		/*while(visited[currConnection] && !q.empty())
		{
			currConnection = q.front(); q.pop();
		}
		if(visited[currConnection])
			break;*/
		// cout<<"c_c:"<<currConnection<<endl;
		// visited[currConnection]=true;

		if(relax(G,eat,currConnection))
		{
			visited[currConnection]=true;
			// cout<<"Realx Sucessful"<<endl;
			for(int i=E.offset[currConnection];i<E.offset[currConnection+1];i++)
			{
				if(visited[E.neighbor[i]]==false)
					q.push(E.neighbor[i]);
				// if(visited[E.neighbor[i]])
				// 	cout<<"Error: "<<currConnection<<" "<<E.neighbor[i]<<endl;
			}
		}
	}
}


void earliestArrivalTime(esdg &E,connections &G,int s,int ts,int *&eat,int &count)
{
	queue<int> q;
	vector<bool> visited(G.m,false);

	//Intialization 
	// step 1: Push vertex in esdg graph having source s and starting time more that ts from all the connections in the graph
	// step 2: intialization of EAT[v] v in V

	for(int i=0;i<G.m;i++)
	{
		if(G.u[i]==s && G.st[i]>=ts)
		{
			q.push(i);
		}
	}

	//EAT Intilization
	for(int i=0;i<G.n;i++)
	{
		eat[i]=INT_MAX;
	}
	eat[s]=ts;

	// step 3: BFS

	while(!q.empty())
	{
		int currConnection = q.front(); q.pop();

		/*while(visited[currConnection] && !q.empty())
		{
			currConnection = q.front(); q.pop();
		}
		if(visited[currConnection])
			break;*/
		// cout<<"c_c:"<<currConnection<<endl;
		// visited[currConnection]=true;

		if(relax(G,eat,currConnection,count))
		{
			visited[currConnection]=true;
			// cout<<"Realx Sucessful"<<endl;
			for(int i=E.offset[currConnection];i<E.offset[currConnection+1];i++)
			{
				if(visited[E.neighbor[i]]==false)
					q.push(E.neighbor[i]);
				// if(visited[E.neighbor[i]])
				// 	cout<<"Error: "<<currConnection<<" "<<E.neighbor[i]<<endl;
			}
		}
	}
}

void print(ofstream &oFile, int *A,int n,int s, int t)
{
	for(int i=0;i<n;i++)
		oFile<<i<<" "<<A[i]<<endl;
		// oFile<<"EAT["<<i<<"] from source "<<s<<" at departure time "<<t<<" is: "<<A[i]<<endl;
}

int main(int argc, char*argv[])
{
	char *ip_esdg, *ip_qfile, *op_eatfile;
	connections G;
	esdg E;
	query Q;
	int count=0;
    double total_time=0.0, q_time=0.0;
    clock_t start,end;
    

    assert(argc==4);

    ip_esdg=argv[1];
    // ip_connections=argv[2];
    ip_qfile=argv[2];
    op_eatfile=argv[3];

	read_input(ip_esdg,ip_qfile,E,G,Q);

	int *eat=new int[G.n];

	ofstream oFile;
	oFile.open(op_eatfile);


	for(int i=0;i<Q.n;i++)
	{
		start=clock();
		// earliestArrivalTime(E,G,Q.sv[i],Q.t[i],eat,count);
		earliestArrivalTime(E,G,Q.sv[i],Q.t[i],eat);
		end=clock();
		q_time=double(end-start)/CLOCKS_PER_SEC;

		print(oFile,eat,G.n,Q.sv[i],Q.t[i]);

		total_time+=q_time;

	}
	
	cout<<op_eatfile<<" Average Run Time:"<<(total_time/Q.n)*1000<<" Seconds"<<endl;

	// cout<<"Number of Connections Processed "<<count/Q.n<<" Out of "<<G.m<<endl;

	oFile.close();


return 0;
}
