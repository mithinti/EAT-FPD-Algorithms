#include<iostream>
#include<bits/stdc++.h>

using namespace std;

typedef struct conn
{
	int u,v,departure,duration;
}conn;

void getData(ifstream &connections, ifstream &roots, vector<conn> &connectionsVector, vector<int> &dependency, vector<int> &offset, vector<int> &rootsVector,int &totalVertices)
{
	int totalConnections, totalDependencies;

	connections>>totalVertices>>totalConnections>>totalDependencies;

	for(int i=0;i<totalConnections;i++)
	{
		conn currConnection;
		connections >> currConnection.u >> currConnection.v >> currConnection.departure >> currConnection.duration;
		connectionsVector.push_back(currConnection);

		int currOffset;
		connections >> currOffset;

		offset.push_back(currOffset);
	}
	offset.push_back(totalDependencies);

	for(int i=0;i<totalDependencies;i++)
	{
		int currDependency;
		connections >> currDependency;
		dependency.push_back(currDependency);
	}

	int totalRoots;
	roots>>totalRoots;

	for(int i=0;i<totalRoots;i++)
	{
		int currRoot;
		roots>>currRoot;

		rootsVector.push_back(currRoot);
	}
}

void getLevels(ifstream &connections, ifstream &roots, ofstream &connectionsWithLevels)
{
	vector<conn> connectionsVector;
	vector<int> rootsVector; 
	vector<int> dependency; // total number of dependencies
	vector<int> level; //size equal to total number of connections
	vector<int> offset; //size equal to total number of connections
	int totalVertices;

	getData(connections,roots,connectionsVector,dependency,offset,rootsVector,totalVertices);

	int totalConnections = connectionsVector.size();
	int totalDependencies = dependency.size();

	vector<vector<int>> incoming;
	for(int i=0;i<totalConnections;i++)
	{
		vector<int> temp;
		incoming.push_back(temp);
	}

	for(int i=0;i<offset.size()-1;i++)	//adding incoming vertices information
	{
		for(int j=offset[i];j< offset[i+1];j++)
		{
			incoming[dependency[j]].push_back(i);
		}
	}

	queue<int> current;
	queue<int> next;

	
		// Intialization of level number to all connections
	// vector<bool> visited;
	for(int i=0;i<totalConnections;i++)
	{
		// visited.push_back(false);
		level.push_back(-1);
	}

	for(int i=0;i<rootsVector.size();i++)
	{
		current.push(rootsVector[i]);
		level[rootsVector[i]]=0;
	}
	int popednodeindex=0;


	do{
		cout<<"Queue Size: "<<current.size()<<endl;

		while(!current.empty())
		{
			int currentConnection = current.front(); current.pop();
			cout<<popednodeindex++<<" ";
			cout<<"poped root node: "<<currentConnection<<endl;

			for(int i=offset[currentConnection];i<offset[currentConnection+1];i++)
			{
				int currentOutgoing = dependency[i];
				// cout<<currentOutgoing<<" ";

				next.push(currentOutgoing);
				if(level[currentOutgoing] < level[currentConnection]+1){
					// next.push(currentOutgoing);
					level[currentOutgoing] = level[currentConnection]+1;
					// cout<<"\n level["<<currentOutgoing<<"] "<<level[cConn]+1<<endl;
				}
			}

		}
		next.swap(current);
	} while(!current.empty());

	cout<<"Level Number calculated"<<endl;

	connectionsWithLevels<<totalVertices<<" "<<totalConnections<<" "<<totalDependencies<<" "<<*max_element(level.begin(), level.end())+1<<endl;
	for(int i=0;i<totalConnections;i++)
	{
		// connectionsWithLevels<<i<<" "<<connectionsVector[i].u<<" "<<connectionsVector[i].v<<" "<<connectionsVector[i].departure<<" "<<connectionsVector[i].duration<<" "<<offset[i]<<" "<<level[i]<<endl;
		connectionsWithLevels<<i<<" "<<connectionsVector[i].u<<" "<<connectionsVector[i].v<<" "<<connectionsVector[i].departure<<" "<<connectionsVector[i].duration<<" "<<level[i]<<endl;
	}
	/*for(int i=0;i<totalDependencies;i++)
	{
		connectionsWithLevels<<dependency[i]<<endl;
	}*/

}

int main(int argc, char *argv[])
{
	ifstream connections; 
   	connections.open(argv[1]); 

   	cout<<"read connections done"<<endl;

   	ifstream roots; 
   	roots.open(argv[2]); 

   	cout<<"read roots done"<<endl;

	ofstream connectionsWithLevels;
   	connectionsWithLevels.open(argv[3]);

   	cout<<"outfile created "<<endl;   	
	
	getLevels(connections,roots,connectionsWithLevels);

	cout<<"Program executed sucessfully"<<endl;

	return 0;
}