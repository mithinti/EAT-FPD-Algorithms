#include<iostream>
#include<vector>
#include<queue>
#include<fstream>
#include <chrono> 

using namespace std::chrono;
using namespace std;
ofstream fout;
int vComponent,maxVComponent=0;
struct Vertex{
    int index;
    vector<Vertex*> list;
    Vertex* parent;
    int dist;
    char color;
};
struct Graph{
    int V;
    vector<Vertex*> Adj;
    void InitialiseGraph(int x){
        int i;
        for(i=0;i<x;i++){
            Vertex *v=new Vertex;
            v->index=i;
            Adj.push_back(v);
        }
    }
    void addEdge(int v,int u){
        Adj[v]->list.push_back(Adj[u]);
    }
};
/*void BFS(Graph G,int s){
    int i;
    vComponent=0;
    G.Adj[s]->dist=0;
    G.Adj[s]->color='G';
    queue<Vertex*> Q;
    Q.push(G.Adj[s]);
    while(Q.size()!=0){
        Vertex *v = Q.front();
        Q.pop();
        vComponent++;
        int j;
        for(j=0;j<v->list.size();j++){
            Vertex *b = v->list[j];
            if(b->color=='W'){
                b->parent=v;
                b->color='G';
                b->dist=v->dist+1;
                Q.push(b);
            }
        }
        v->color='B';
    }
    
}
int BFS_Source(Graph G){
    int i,Components=0;
    for(i=0;i<G.Adj.size();i++){
        G.Adj[i]->parent=NULL;
        G.Adj[i]->dist=-1;
        G.Adj[i]->color='W';
    }
    for(i=0;i<G.Adj.size();i++){
        Vertex *u=G.Adj[i];
        if(u->color=='W'){
            Components++;
            BFS(G,u->index);
            if(vComponent>maxVComponent){
                maxVComponent=vComponent;
            }
        }
    }
    return Components;
}
*/
int main(int argc, char* argv[]){
    Graph G;
    ifstream fin(argv[1]);
    fout.open(argv[2]);
    int vCount,eCount;
    fin>>vCount;
    fin>>eCount;
    cout<<vCount<<" "<<eCount<<" "<<endl;
    int *inD,*outD,*totD;
    inD=new int[vCount];
    outD=new int[vCount];
    totD=new int[vCount];
    int i;
    for(i=0;i<vCount;i++){
        inD[i]=0;
        outD[i]=0;
        totD[i]=0;
    }
    G.InitialiseGraph(vCount);
    cout<<"Graph initialised"<<endl;
    
    for(i=0;i<eCount;i++){
        int u,v;
        fin>>u>>v;
        if(u<vCount && v<vCount){
            G.addEdge(u,v);
            outD[u]++;
            inD[v]++;
            totD[u]++;
            totD[v]++;
        }
    }
    cout<<"Graph read successfully"<<endl;
    int maxD=0,minD=totD[0],n2D=0;
    int maxId=0,maxOd=0,n1D=0,n0D=0;
    int sumDg=0;
    for(i=0;i<vCount;i++){
        sumDg +=totD[i];
        if(totD[i]>maxD){
            maxD=totD[i];
        }
        if(inD[i]>maxId){
            maxId=inD[i];
        }
        if(outD[i]>maxOd){
            maxOd=outD[i];
        }
        if(totD[i]<minD){
            minD=totD[i];
        }
        if(totD[i]==2){
            n2D++;
        }
        if(inD[i]==1 && outD[i]==1){
            n1D++;
        }
        if(totD[i]==0){
            n0D++;
        }

    }
    int Components=BFS_Source(G);
    fout<<"Max Degree: "<<maxD<<"\nMin Degree: "<<minD<<"\nNumber of 2 Degree: "<<n2D<<"\nComponents: "<<Components<<endl;
    fout<<"Max Indegree: "<<maxId<<"\nMax Outdegree: "<<maxOd<<"\nAvg Degree: "<<sumDg/vCount<<"\n1I1O: "<<n1D<<"\nZero Degrees: "<<n0D<<endl;
    fout<<"Max Vertices in Component: "<<maxVComponent<<endl;
    cout<<"Max Degree: "<<maxD<<"\nMin Degree: "<<minD<<"\nNumber of 2 Degree: "<<n2D<<"\nComponents: "<<Components<<endl;
    cout<<"Max Indegree: "<<maxId<<"\nMax Outdegree: "<<maxOd<<"\nAvg Degree: "<<sumDg/vCount<<"\n1I1O: "<<n1D<<"\nZero Degrees: "<<n0D<<endl;
    cout<<"Max Vertices in Component: "<<maxVComponent<<endl;
    fin.close();
    fout.close();
    return 0;
    
}