#include "header.h"
//@authors Dylan Bunch (djbzf5), Byron Smith (bjha43), Ben Blanquert (btb5q)

//Program description: 

int main(void) {
    
    //for some reason C decided that, if I didn't declare this here, compile 
    //errors would be thrown
    adjs** transposeAdjs = NULL;
    
    //read file in and store the edges inside a linked list
    edge* edges = openFile("input.txt");

    //reads the linked list of edges and returns the number of vertexes that exist
    int maxVerts = getMaxVerts(edges);

    //returns an array of vertex* that each point to nodes which contain the 
    //indices for the array
    vert** verts = getVertexList(edges, maxVerts);
    
    //returns an array of pointers that point to nodes which contain the adjacency 
    //list for each vertex
    adjs** adjs = getAdjs(edges);
    
    //prints the adjacency lists
    printf("Printing Adjacency List:\n\n");
    printAdjs(adjs, maxVerts);
    
    //create an array of pointers to correspond to the vertex array that contains 
    //the completion times
    vert** compTimes = getCompTimes(verts, adjs, maxVerts);

    //transpose the edges and  store them in the pointer array of reversed edges
    edge* transpose = transposeEdges(edges);
    
    //create a double pointer that points to an array of adjs structures 
    //for the transposed graph
    transposeAdjs = getAdjs(transpose);

    //count the number of components based on the original completion times and 
    //the transposed graph
    int numComps = findSCCs(verts, maxVerts, compTimes, transposeAdjs);
    
    //print the out-degree sequence of the components
    getOutDegs(verts, edges, numComps, maxVerts);
    



    return 0;
}