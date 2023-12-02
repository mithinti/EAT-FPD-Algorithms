#include<iostream>
#include<fstream>
#include<cstdlib>
using namespace std;
int main(int argc, char*argv[])
{
	char *outfile;
	if(argc!=3)
	{
		cout<<"./a.out output_query_filename.txt maxNumberofVertices"<<endl;
		return -1;
	}
	outfile =argv[1];

	int n=atoi(argv[2]);
	cout<<"Number of Vertices in the given graph:"<<n<<endl;
	srand(time(0));
	ofstream oFile;
	oFile.open(outfile);
	oFile<<100<<endl;
	int t=n/100;
	for(int queryIndex=0; queryIndex<n; queryIndex=queryIndex+t){
		oFile<<queryIndex<<endl;
	}
	oFile.close();
	return 0;
}