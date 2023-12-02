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

struct vertexPair
{
	int u, t;
};

struct TRG
{
	int *offset, *neighbours, *weight, totalVertices, totalVerticesInTRG, totalEdgesInTRG, totalChainEdgesInTRG;
	vector<string> vertexInformation;
};

void readGraph(ifstream &inputGraph, TRG &G)
{
	cout << "**** 1. Started Reading Graph****" << endl;
	string line;
	stringstream ss;
	getline(inputGraph, line);
	ss << line;
	ss >> G.totalVertices >> G.totalVerticesInTRG >> G.totalEdgesInTRG >> G.totalChainEdgesInTRG;

	G.vertexInformation.reserve(G.totalVerticesInTRG);
	G.offset = new int[G.totalVerticesInTRG + 1];
	G.neighbours = new int[G.totalEdgesInTRG + G.totalChainEdgesInTRG];
	G.weight = new int[G.totalEdgesInTRG + G.totalChainEdgesInTRG];
	cout << "**** 2. Assigned Memory****" << endl;

	for (int i = 0; i < G.totalVerticesInTRG; i++)
	{
		getline(inputGraph, line);
		G.vertexInformation.push_back(line);
	}
	cout << "**** 3. Read all vertices labels****" << endl;


	vector<vector<pair<int, int>>> neighboursVector(G.totalVerticesInTRG);
	int u, v, w;
	for (int i = 0; i < G.totalEdgesInTRG; i++)
	{
		stringstream curr;
		getline(inputGraph, line);
		curr << line;
		curr >> u >> v >> w;
		neighboursVector[u].push_back(make_pair(v, w));
	}
	for (int i = 0; i < G.totalChainEdgesInTRG; i++)
	{
		stringstream curr;
		getline(inputGraph, line);
		curr << line;
		curr >> u >> v >> w;
		neighboursVector[u].push_back(make_pair(v, w));
	}

	cout << "**** 4. Created Neighbour Vector****" << endl;


	G.offset[0] = 0;
	int pointer = 0;
	for (int i = 0; i < G.totalVerticesInTRG; i++)
	{
		G.offset[i + 1] = G.offset[i] + neighboursVector[i].size();
		for (unsigned int j = 0; j < neighboursVector[i].size(); j++)
		{
			G.neighbours[pointer] = neighboursVector[i][j].first;
			G.weight[pointer] = neighboursVector[i][j].second;
			pointer++;
		}
	}

	cout << "**** 5. Created final offset, neighbour, and weight array****" << endl;
}

void writeGraph(ofstream &output, TRG &G)
{
	output << G.totalVertices << " " << G.totalVerticesInTRG << " " << G.totalEdgesInTRG << " " << G.totalChainEdgesInTRG << endl;
	for (unsigned int i = 0; i < G.vertexInformation.size() ; i++)
	{
		output << G.vertexInformation[i] << endl;
	}
	for (int i = 0; i < G.totalVerticesInTRG + 1; i++)
	{
		output << G.offset[i] << " ";
	}
	output << endl;

	for (int i = 0; i < G.totalEdgesInTRG + G.totalChainEdgesInTRG; i++)
	{
		output << G.neighbours[i] << " ";
	}
	output << endl;

	for (int i = 0; i < G.totalEdgesInTRG + G.totalChainEdgesInTRG; i++)
	{
		output << G.weight[i] << " ";
	}
	output << endl;
}

int main(int argc, char* argv[])
{
	ifstream inputGraph;
	inputGraph.open(argv[1]);

	ofstream output;
	output.open(argv[2]);

	cout << "****************I. Reading Graph****************" << endl;
	TRG G;
	readGraph(inputGraph, G);

	cout << "****************II. Printing graph information ****************" << endl;
	cout << "Total Vertices in original graph = " << G.totalVertices << endl;
	cout << "Total Vertices in TRG transformed graph = " << G.totalVerticesInTRG << endl;
	cout << "Total Primary Edges in TRG transformed graph = " << G.totalEdgesInTRG << endl;
	cout << "Total Chain Edges in TRG transformed graph = " << G.totalChainEdgesInTRG << endl;

	//write output file
	cout << "****************III. Writing to output file ****************" << endl;
	writeGraph(output, G);
	cout << "****************IV. Completed Successfully ****************" << endl;
	return 0;
}