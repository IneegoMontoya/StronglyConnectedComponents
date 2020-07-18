#include <stdio.h>
#include <stdlib.h>
#define VISITED 0
#define NOT_VISITED 1

typedef struct edgeList {
    struct edgeList *next;
    int x;
    int y;
} edge;

typedef struct vertList {
    struct vertList *next;
    int key;
    int compNum;
    int visited;
} vert;

typedef struct adjsList {
    vert *sortedList;
    vert *list;
    int vertNum;
} adjs;

typedef struct compList {
    struct compList *next;
    int compNum;
} comp;

typedef struct compAdjsS {
    comp *list;
    int compNum;
    int outDeg;
} compAdjs;

edge* openFile(char*);
int getMaxVerts(edge*);
vert** getVertexList(edge*,int);
adjs** getAdjs(edge*);
adjs** initAdjs(int);
void printAdjs(adjs*[],int);
void printAdjList(adjs*);
vert** getCompTimes(vert*[],adjs**,int);
void clearVisited(vert**,int);
void DFS(vert*,adjs**,int,vert**,int*,vert**);
edge* transposeEdges(edge*);
int findSCCs(vert**,int,vert**,adjs**);
void dfsSCCs(vert*,int,adjs**,vert**);
void getOutDegs(vert**,edge*,int,int);
int getNumEdges(edge*);
void countingSort(int[],int);
© 2020 GitHub,