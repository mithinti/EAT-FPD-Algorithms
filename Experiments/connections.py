import random
import sys
import networkx as nx
import numpy as np


inputFile = sys.argv[1]
outputFile = sys.argv[2]
conn_output = sys.argv[3]

inputF = open(inputFile, 'r')
outputF = open(outputFile, 'w')
conn_outputF= open(conn_output,'w')

content = inputF.readlines()

nVE = list(map(lambda x:int(x), content[0].split()))
nVertices, nEdges, nESDGedges, nLevels = nVE[0], nVE[1], nVE[2], nVE[3]

connections = []
for line in content[1:]:
    connTuple = tuple(map(lambda x:int(x), line.split()))
    connections.append(connTuple)

connections = sorted(connections, key=lambda x:x[5])

final = []
# print('Sorted Tuples ')
for index, x in enumerate(connections):
    newTuple = (index,) + x[1:6]
    final.append(newTuple)


for x in final:
    print(x)
conn_outputF.write(str(nVertices) + ' ' + str(nEdges) + ' ' + str(nESDGedges) + ' ' + str(nLevels) +'\n')
for x in final:
    conn_outputF.write(' '.join(map(str, x)) + '\n')

edges = []
offset = []
neighbors = []
offset.append(0)
for connection in final:
    index, u, v, w, duration, lno = connection
    
    # print('U being Considered : ', u)
    limit = w + duration
    
    vList = list(map(lambda x:x[1], final))
    indices = np.where(np.array(vList) == v)[0]
    
    # print('Indices : ', indices)
    
    for edgeEnd in indices:
        if final[edgeEnd][3] >= limit:
            edges.append((index, edgeEnd)) 
            neighbors.append(edgeEnd)
            
    offset.append(len(neighbors))
            
 

# print('Edges : ', edges)
# print('Offset : ', offset)
# print('Neighbors : ', neighbors)


outputF.write(str(nEdges) + ' ' + str(nESDGedges) + ' ' + str(nVertices)+'\n')
outputF.write(' '.join(map(str, offset)) + '\n')
outputF.write(' '.join(map(str,neighbors)) + '\n')

# conn_outputF.write(' '.join(map(str, edges)) + '\n')

outputF.close()
conn_outputF.close()
# print('No.of Vertices : ', len(G.nodes))
# print('No.of Edges : ', len(G.edges))
# print('Nodes : ',G.nodes)
# print('Edges : ', G.edges)


# G = nx.DiGraph()
# G.add_edges_from(edges)
# cycles = nx.simple_cycles(G)
# print('Cycles : ', list(cycles))

