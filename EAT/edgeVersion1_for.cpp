#include<string.h>
#include<limits.h>
#include<iostream>
#include<stdio.h>
#include<malloc.h>
#include<fstream>
using namespace std;
#include<sys/time.h>
//#include<cuda.h>
#define INF INT_MAX
#define MAX_THREADS_PER_BLOCK 1024
#define PRINTFLAG true
//Reading Inputs
//
//
// editing
bool computeEarliestTimes(int vCount, int eCount, int *from, int *to, int *departure, int *duration, int *&earliestTime)
{

	int i,u,v,t,lambda;
	int count=0;

	for(i=0; i<=eCount-1; i++)
	{
		u = from[i];
		v = to[i];
		t = departure[i];
		lambda = duration[i];

		if((earliestTime[u]<=t) && (t + lambda < earliestTime[v]))
		{
			earliestTime[v]= t+lambda;
		}
	}
	//cout<<"No. of times updated is "<<count<<endl;
}

void initEarlistTime(int *&earliestTime, int vCount, int source, int departureTime)
{
	//earliestTime = new int[vCount];

	for(int i=0; i<=vCount-1; i++)
	{
		earliestTime[i] = INF;
	}

	earliestTime[source]=departureTime;
}

bool readInput(char *fileName, int &vCount, int &eCount, int *&from, int *&to, int *&departure, int *&duration)
{

	ifstream fin;

	fin.open(fileName);

	fin>>vCount>>eCount;

	from = new int[eCount];
	to = new int[eCount];
	departure = new int[eCount];
	duration = new int[eCount];

	for(int i=0; i<=eCount-1; i++)
		fin>>from[i]>>to[i]>>departure[i]>>duration[i];

	//cout<<"reading the input is over"<<endl;

return true;
}

void readQuery(char *fileName, int *&source, int *&departureTime, int &qCount)
{
	ifstream fin;

	fin.open(fileName);

	fin>>qCount;

	source = new int[qCount];
	departureTime = new int[qCount];

	for(int i=0; i<=qCount-1; i++)
		fin>>source[i]>>departureTime[i];
}

void printEarliestTimes(int earliestTime[], int vCount,	double runningTime, bool print)
{
//	ofstream file,etime;
//	file.open("sfc.txt",ios::app);
	//etime.open("sftime.txt",ios::app);
	if(print == true)
	{
		for(int i=0;i<=vCount-1;i++)
		{
			cout<<i<<" "<<earliestTime[i]<<endl;
			//fprintf(fp1,"Earliest time for %d is %d\n",i,earliest[i]);
		}
//		file<<endl;
	}
	cout<<"Time is "<<runningTime<<endl;
	//file.close();
	//etime.close();
}
void 	createHostMemory(int *&earliestTime, int vCount)
{
	earliestTime = new int[vCount];
}


int main(int argc, char *argv[])
{

	int vCount, eCount, qCount;
	int *from, *to, *departure, *duration, *earliestTime;
	int *source, *departureTime;
	//int source, departureTime;

	char fileName[100], queryFile[100];
	struct timeval start,stop;
	double time;


	strcpy(fileName, argv[1]);
	strcpy(queryFile, argv[2]);

	readInput(fileName,vCount, eCount, from, to, departure, duration);
	readQuery(queryFile, source, departureTime, qCount);
	//source=0; departureTime=0;

	createHostMemory(earliestTime,vCount);


	for(int i=0; i<=qCount-1; i++)
	{
		initEarlistTime(earliestTime,vCount,source[i],departureTime[i]);
		//initEarlistTime(earliestTime,vCount,source,departureTime);
		gettimeofday(&start,0);
		computeEarliestTimes(vCount,eCount,from,to,departure,duration,earliestTime);
		gettimeofday(&stop,0);
		time = (1000000.0*(stop.tv_sec-start.tv_sec) + stop.tv_usec-start.tv_usec)/1000.0;
		printEarliestTimes(earliestTime,vCount,time,PRINTFLAG);
	}



return 0;
}


