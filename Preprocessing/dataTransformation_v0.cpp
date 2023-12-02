#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <utility>
#include <fstream>
#define N 3
using namespace std;

struct connection
{
	int u, v, t, lambda;
};

void readGraph(ifstream &inputGraph, int &totalVertices, vector<connection> &connections)
{
	string line;
	int totalConnections;
	stringstream ss;

	getline(inputGraph, line);
	ss << line;

	ss >> totalVertices >> totalConnections;
	cout << "Total Vertices = " << totalVertices << " and Total Connections = " << totalConnections << endl;
	for (int i = 0; i < totalConnections; i++)
	{
		connection c;
		stringstream s1;
		getline(inputGraph, line);
		s1 << line;
		s1 >> c.u >> c.v >> c.t >> c.lambda;
		connections.push_back(c);
	}
}

unordered_map<string, int> getAllVertices(int totalVertices, vector<connection> connections, vector<set<int>> &S)
{
	vector<int> maxInput(totalVertices, 0);
	set<int>::iterator it;

	// max{t:t∈Tin(u)}.  LetV′out(u) =Vout(u)∪{< u, tm>} in case Tout(u) =∅ or tm>max{t:t∈Tout(u)}

	//1.1 Add primary vertices
	for (connection c : connections)
	{
		S[c.u].insert(c.t);
		maxInput[c.v] = max(maxInput[c.v] , c.t + c.lambda);
	}

	//1.2 Add all helper vertices.
	for (connection c : connections)
	{
		it = S[c.v].lower_bound(c.t + c.lambda);
		if (it == S[c.v].end())
		{
			S[c.v].insert(maxInput[c.v]);
		}
	}

	/*for (int i = 0; i < totalVertices; i++)
	{
		cout << " T out for  " << i << " = ";
		for (auto itx = S[i].begin() ; itx != S[i].end(); itx++)
		{
			cout << *itx << " ,";
		}
		cout << endl;
	}*/

	//1.3 create dictionary

	int count = 0;
	unordered_map<string, int> vertexToId;

	for (int i = 0; i < totalVertices; i++)
	{
		string vertexName = to_string(i);
		for (auto itx = S[i].begin() ; itx != S[i].end(); itx++)
		{
			vertexToId[vertexName + " " + to_string(*itx)] = count++;
		}
	}

	return vertexToId;
}

vector<string> getEdgesOfTransformedGraph(vector<connection> connections, unordered_map<string, int> vertexToId, vector<set<int>> S)
{
	int u, v, t, lambda;
	vector<string> result;
	string startingVertex, endingVertex, edge;
	for (connection c : connections)
	{
		u = c.u; v = c.v; t = c.t; lambda = c.lambda;
		startingVertex = to_string(u) + " " + to_string(t);
		endingVertex = to_string(v) + " " + to_string(*(S[v].lower_bound(t + lambda)));
		edge = to_string(vertexToId[startingVertex]) + " " + to_string(vertexToId[endingVertex]) + " " + to_string(lambda);
		result.push_back(edge);
	}
	return result;
}

vector<string> getChainEdges(vector<set<int>> S, unordered_map<string, int> vertexToId)
{
	string vertex;
	vector<string> edgeList;

	cout<<" S.size : "<<S.size();
	for (unsigned int i = 0; i < S.size(); i++)
	{
		cout<<i<<" ";
		vertex = to_string(i);
		auto it = S[i].begin();
		int prev = vertexToId[vertex + " " + to_string(*it)], curr;
		it++;
		while (it != S[i].end())
		{
			curr = vertexToId[vertex + " " + to_string(*it)];
			edgeList.push_back(to_string(prev) + " " + to_string(curr) + " 0");
			prev = curr;
			it++;
		}
	

		// for (string connection : edgeList)
		// {
		// 	cout << connection <<" ";
		// }
		
	}
	cout<<endl;
	return edgeList;
}


void writeGraph(unordered_map<string, int > vertexToId, vector<string> primaryEdgeList,  vector<string> chainEdgeList, int totalVertices, ofstream &output)
{
	map<int, string> idToVertex;
	string key;
	int value;
	for (auto it = vertexToId.begin(); it != vertexToId.end(); it++)
	{
		key = it->first;
		value = it->second;
		idToVertex[value] = key;
	}
	output << totalVertices << " " << vertexToId.size() << " " << primaryEdgeList.size() << " " << chainEdgeList.size() << endl;
	for (auto it = idToVertex.begin(); it != idToVertex.end(); it++)
	{
		output << it->first << " " << it->second << endl;
	}
	for (string connection : primaryEdgeList)
	{
		output << connection << endl;
	}
	for (string connection : chainEdgeList)
	{
		output << connection << endl;
	}
}

int main(int argc, char* argv[])
{
	ifstream inputGraph;
	inputGraph.open(argv[1]);

	ofstream output;
	output.open(argv[2]);

	vector<connection> connections;  //set of connections in the original graph
	int totalVertices;				// Number of vertices in the original graph

	//Step 0: Read connection stream [Temporal (Origi) Graph]
	cout << "Reading Graph" << endl;
	readGraph(inputGraph, totalVertices, connections);

	//Step 1: Get all vertices in Transformed graph
	cout << "Calculating all vertices in the transformed Graph" << endl;
	vector<set<int>> S(totalVertices);  // vector of sets of size number of vertices in the original graph
	unordered_map<string, int> vertexToId =  getAllVertices( totalVertices, connections, S);

	//Step 2: Create EdgeList of transformed graph
	cout << "Calculating all edges part of transformed graph" << endl;
	vector<string> primaryEdgeList = getEdgesOfTransformedGraph(connections, vertexToId, S);

	//Step 3: Get all chain edges
	cout << "Adding all the chain edges in the transformed graph." << endl;
	vector<string> chainEdgeList = getChainEdges(S, vertexToId);

	//Step 4: Write to the output File
	cout << "Writing the transformed graph to output File" << endl;
	writeGraph(vertexToId, primaryEdgeList, chainEdgeList, totalVertices , output);

	return 0;
}