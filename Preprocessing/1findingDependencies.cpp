//input is connections_sorted.txt 
//output is depencency.txt which is dependency graph without levels and root.txt which consists of the root nodes

/*g++ -o dep.out -std=c++14  1findingDependencies.cpp
./dep.out ../../../Data/dominated_conn_sort_t/sample1_connections_dom.txt ../../../Data/dependencies/sample1_dep.txt ../../../Data/dependencies/sample1_roots.txt ../../../Data/dependencies/sample1_esdg.txt
totalVertices in MAP with outgoing connection = 4
totalConnections in allConnection MAP = 11
totalDependencies = 13
*/

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


map<int,unordered_map<int,set<details,details>>> sortConnections(ifstream &connections, int totalConnections)
{
	
	map<int,unordered_map<int,set<details,details>>> m;
	int u,v,departure,duration;

	int id;
	for(int i=0;i<totalConnections;i++)
	{
		connections>>id>>u>>v>>departure>>duration;

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

void getEarliestArrivalDependencies(ifstream &connections, ofstream &dependencies, ofstream &root, ofstream &esdg_file)
{
	map<int,unordered_map<int,set<details,details>>> m;

	int totalVertices,totalConnections;
	connections>>totalVertices>>totalConnections;

	m = sortConnections(connections,totalConnections);

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

	//here m is sorted connections 2D MAP: unordered_map<int,unordered_map<int,set<details,details>>>
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

		/*cout<<"\n\n\n";

		cout<<"Current connection for which we are finding dependencies"<<endl;
		cout<<u<<" "<<v<<" "<<departure<<" "<<duration<<endl;
*/
		int reachingOfv = duration+departure;	

		auto vMap = m.find(v);  //vMap has all neighbours of v 


		if(vMap != m.end())
		{
			for(auto vNeighbour = vMap->second.begin(); vNeighbour!=vMap->second.end(); vNeighbour++) //for each neighbour of v
			{
				int w = vNeighbour->first;
				//search for best departure in vNeighbour

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

					auto neighbourId = getConnectionsId.find(temp);
					neighboursVector.push_back(neighbourId->second);

					//mark as not root
					rootVertices[neighbourId->second] = false;

					offset++;

				}
			}
		}

	/*cout<<"\n offsetOfNeighbours: \n";
	for(auto i:offsetOfNeighbours)
	{
		cout<<i << " ";
	}

	cout<<"\n neighboursVector: \n";
	for(auto i: neighboursVector)
	{
		cout<<i << " ";
	}

	cout<<"\n rootVertices \n";
	for(auto i: rootVertices)
	{
		cout<<i << " ";
	}*/

	}



	cout<<"totalVertices in MAP with outgoing connection = "<<m.size()<<endl;
	cout<<"totalConnections in allConnection MAP = "<<allConnections.size()<<endl;
	cout<<"totalDependencies = "<<neighboursVector.size()<<endl;

	// printing output to file

	dependencies<<totalVertices<<" "<<allConnections.size()<<" "<<neighboursVector.size()<<endl;

	for(int i=0; i<allConnections.size();i++)
	{
		auto currConnection = allConnections.find(i);
		dependencies<<i<<" "<<currConnection->second.u<<" "<<currConnection->second.v<<" "<<currConnection->second.departure<<" "<<currConnection->second.duration<<" "<<offsetOfNeighbours[i]<<endl;
	}

	for(int i=0;i<neighboursVector.size();i++)
	{
		dependencies<<neighboursVector[i]<<" ";
	}

	vector<int> actualRootVertices;
	for(int i=0;i<rootVertices.size();i++)
	{
		if(rootVertices[i]==true)
			actualRootVertices.push_back(i);
	}

	root<<actualRootVertices.size()<<endl;
	for(int i=0;i<actualRootVertices.size();i++)
	{
		root<<actualRootVertices[i]<<endl;
		// cout<<actualRootVertices[i]<<endl;
	}
	//all connections in allConnection unordered map , key is vertex  


	//printing ESDG in CSR Format
	esdg_file<<offsetOfNeighbours.size()<<" "<<neighboursVector.size()<<" "<<totalVertices<<endl;
	for(int i = 0; i<offsetOfNeighbours.size();i++)
	{
		esdg_file<<offsetOfNeighbours[i]<<" ";
	}
	esdg_file<<endl;
	for(int i = 0; i<neighboursVector.size();i++)
	{
		esdg_file<<neighboursVector[i]<<" ";
	}
	esdg_file<<endl;

	//printing all connections starting with u
/*
	int u2,v2,departure2,duration2;
	u2 = v;
	v2 = w;
	departure2 = earliestCandidate->departure;
	duration2 = earliestCandidate->duration;

	string temp = to_string(u2)+" "+to_string(v2)+" "+to_string(departure2)+" "+to_string(duration2);

	auto neighbourId = getConnectionsId.find(temp);
*/
	int uIndex=0;
	
	for(auto it=m.begin();it!=m.end();it++)
	{
		int u,v,departure,duration;
		u=it->first; 
		while(u!=uIndex)
		{
			
			// cout<<"Connections starting with "<<uIndex<<"not found "<< endl;
			esdg_file<<"-1"<<endl;
			uIndex++;
		}
		// cout<<"Connections starting with "<<u<<endl;
		for(auto jt = it->second.begin();jt!=it->second.end();jt++)
		{
			v = jt->first;
			// cout<<"Connections between "<<u<<" "<<v<<endl;
			for(auto kt = jt->second.begin(); kt!=jt->second.end();kt++)
			{
				departure = kt->departure;
				duration = kt->duration;
				string temp = to_string(u)+" "+to_string(v)+" "+to_string(departure)+" "+to_string(duration);
				auto u_Id = getConnectionsId.find(temp);
				// cout<<departure<<" "<<duration<<endl;
				esdg_file<<u_Id->second<<" "; 
			}
			// cout<<endl;
		}
		esdg_file<<endl;
		uIndex++;
	}
	while(uIndex<totalVertices){
		// cout<<"Connections starting with "<<uIndex<<endl;
		esdg_file<<"-1"<<endl;
		uIndex++;
	}

	
}

int main(int argc, char *argv[])
{
	ifstream connections; 
   	connections.open(argv[1]); 

   	ofstream dependencies;
   	dependencies.open(argv[2]);

	ofstream root;
   	root.open(argv[3]); 

   	ofstream esdg_file;
   	esdg_file.open(argv[4]);
	
	getEarliestArrivalDependencies(connections,dependencies,root,esdg_file);
}