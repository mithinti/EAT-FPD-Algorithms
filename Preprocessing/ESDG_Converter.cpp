//input is connections_sorted.txt 
//output is depencency.txt which is dependency graph without levels and root.txt which consists of the root nodes


#include<iostream>
#include<fstream>
#include<bits/stdc++.h>

using namespace std;

typedef struct details
{
	int departure;
	int duration;

	bool operator()(details a,details b)
	{
		return a.departure<b.departure;
	}
}details;

typedef struct conn
{
	int u,v,departure,duration;

}conn;


unordered_map<int,unordered_map<int,set<details,details>>> sortConnections(ifstream &connections, int totalConnections)
{
	
	unordered_map<int,unordered_map<int,set<details,details>>> m;
	int u,v,departure,duration;

	for(int i=0;i<totalConnections;i++)
	{
		connections>>u>>v>>departure>>duration;

		auto it = m.find(u);
		
		details d;
		d.departure = departure;
		d.duration = duration;
		
		if(it!=m.end())
		{
			auto jt = it->second.find(v);
			
			if(jt!=it->second.end()) //jt first is v and second is set
				jt->second.insert(d);
			
			else
			{
				set<details,details> s;
				s.insert(d);

				it->second.insert({v,s});
			}
		}
		else
		{
			set<details,details> s;
			s.insert(d);

			unordered_map<int,set<details,details>> inside;
			inside.insert({v,s});

			m.insert({u,inside});
		}

	}
	return m;
}

void getEarliestArrivalDependencies(ifstream &connections, ofstream &dependencies, ofstream &root)
{
	unordered_map<int,unordered_map<int,set<details,details>>> m;

	int totalVertices,totalConnections;
	connections>>totalVertices>>totalConnections;

	m = sortConnections(connections,totalConnections); // creating Adjacency List

	/*for(auto it=m.begin();it!=m.end();it++)
	{
		int u,v,departure,duration;
		u=it->first; 
		for(auto jt = it->second.begin();jt!=it->second.end();jt++)
		{
			v = jt->first;
			cout<<"Connections between "<<u<<" "<<v<<endl;
			for(auto kt = jt->second.begin(); kt!=jt->second.end();kt++)
			{
				departure = kt->departure;
				duration = kt->duration;
				cout<<departure<<" "<<duration<<endl; 
			}
		}
		cout<<endl;
	}
*/
	//verified 

	//sorted completed 
	int connId = 0;
	
	unordered_map<int,conn> allConnections;
	unordered_map<string,int> getConnectionsId;
	
	vector<int> offsetOfNeighbours;
	vector<int> neighboursVector;

	vector<bool> rootVertices;



	for (auto i = m.begin();i!=m.end();i++) //creates allConnections
	{
		int u,v,departure,duration;

		u = i->first;

		for(auto j = i->second.begin();j!=i->second.end();j++)
		{
			v = j->first;

			for(auto k = j->second.begin(); k!=j->second.end();k++)
			{
				departure = k->departure;
				duration = k->duration;

				//for every edge
				conn c;
				c.u = u;
				c.v = v;
				c.departure = departure;
				c.duration = duration;
				allConnections.insert({connId,c});
				
				string connectionString = to_string(u)+" "+to_string(v)+" "+to_string(departure)+" "+to_string(duration);
				getConnectionsId.insert({connectionString,connId});

				connId++;

			}

		}
	}

	for(int i=0;i<allConnections.size();i++) //initialize root vertiex
		rootVertices.push_back(true);


	int offset=0;

	// for(auto i = allConnections.begin();i!=allConnections.end();i++)	//adds dependency 
	for(int it = 0;it<allConnections.size();it++)
	{
		auto i = allConnections.find(it);

		offsetOfNeighbours.push_back(offset);  //puts offset of connection

		int u,v,duration,departure;

		u = i->second.u;
		v = i->second.v;
		duration = i->second.duration;
		departure = i->second.departure; 	//find its respective u,v,t,lambda

		// cout<<"Current connection for which we are finding dependencies"<<endl;
		// cout<<u<<" "<<v<<" "<<departure<<" "<<duration<<endl;

		int reachingOfv = duration+departure;	

		auto vMap = m.find(v);  //vMap has all neighbours of v 
		if(vMap != m.end())
		{
			for(auto vNeighbour = vMap->second.begin(); vNeighbour!=vMap->second.end(); vNeighbour++) //for each neighbour of v
			{
				int w = vNeighbour->first;
				//search for best departure in vNeighbour
				// cout<<"Current neighbour = "<<w<<endl;
				//doubtful part
				details candidateDetails;
				candidateDetails.departure = reachingOfv-1;
				candidateDetails.duration = INT_MAX;
				auto earliestCandidate = vNeighbour->second.upper_bound(candidateDetails);



				if(earliestCandidate != vNeighbour->second.end())
				{
					// cout<<"found Dependency"<<endl;
					int u2,v2,departure2,duration2;
					u2 = v;
					v2 = w;
					departure2 = earliestCandidate->departure;
					duration2 = earliestCandidate->duration;
				
					string temp = to_string(u2)+" "+to_string(v2)+" "+to_string(departure2)+" "+to_string(duration2);
					// cout<<temp<<endl;
					auto neighbourId = getConnectionsId.find(temp);
					neighboursVector.push_back(neighbourId->second);

					//mark as not root
					rootVertices[neighbourId->second] = false;

					offset++;

				}
			}
		}
	}
	offsetOfNeighbours.push_back(offset);

	cout<<"totalVertices in MAP with outgoing connection = "<<m.size()<<endl;
	cout<<"totalConnections in allConnection MAP = "<<allConnections.size()<<endl;
	cout<<"totalDependencies = "<<neighboursVector.size()<<endl;


	//printing ESDG in CSR Format
	dependencies<<offsetOfNeighbours.size()<<" "<<neighboursVector.size()<<endl;
	for(int i = 0; i<offsetOfNeighbours.size();i++)
	{
		dependencies<<offsetOfNeighbours[i]<<" ";
	}
	dependencies<<endl;

	for(int i = 0; i<neighboursVector.size();i++)
	{
		dependencies<<neighboursVector[i]<<" ";
	}
	dependencies<<endl;

	//printing connections

	root<<allConnections.size()<<endl;
	for(int i=0; i<allConnections.size();i++)
	{
		auto currConnection = allConnections.find(i);
		root<<i<<" "<<currConnection->second.u<<" "<<currConnection->second.v<<" "<<currConnection->second.departure<<" "<<currConnection->second.duration<<endl;
	}

	
	//all connections in allConnection unordered map , key is vertex  

}

int main(int argc, char *argv[])
{
	ifstream connections; 
   	connections.open(argv[1]); 

   	ofstream dependencies;
   	dependencies.open(argv[2]);

	ofstream root;
   	root.open(argv[3]);   	
	
	getEarliestArrivalDependencies(connections,dependencies,root);
}