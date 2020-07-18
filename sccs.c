#include "header.h"

//read file in and store the edges inside a linked list
edge* openFile(char *file) {
    
    //create file pointer and open input.txt file in folder
    FILE *fptr = fopen(file, "r");
    
    //case to terminate if file pointer is NULL and file wasn't opened
    if (fptr == NULL) {
        printf("Could not open file %s. Exiting.\n", file);
        exit(0);
    }
    
    //initialize ints for storing values inside linked list
    int x = 0, y = 0;
    //create an edge pointer and initialize it to NULL
    edge *head = NULL;
    
    //read the file in line by line until we reach EOF
    while (!feof(fptr)) {
        
        fscanf(fptr, "%d %d\n", &x, &y);
        
        //create a new temporary pointer used for mallocating edges 
        edge *temp = malloc(sizeof(temp));
        
        //set values for (x y) in file per line
        temp->x = x, temp->y = y;
        //establish next pointer to null
        temp->next = NULL;
        
        //if head != null (not head case)
        if (head != NULL) 
            //set the temp's next pointer equal to the head pointer of the list
            temp->next = head;
        
        //set the head pointer equal to the new edge node created
        head = temp;
    }
    
    //close the file like a good boy
    fclose(fptr);
    
    //return head for safekeeping
    return head;
}

//reads through the list and returns the number of vertices that exist
int getMaxVerts(edge *head) {
        
    //integer to determine the maximum vertex number so we know how to count
    int max = 0;
    
    //create a temporary edge pointer and set it equal to the head of the list
    edge *temp = head;

    //loops until we reach the end of the linked list
    while (temp != NULL) {
        
        //check the y value of the current node for the largest vertex number
        if (temp->y > max) {
            max = temp->y;
        }        
        
        //check the x value of the current node for the largest vertex number
        if (temp->x > max) {
            max = temp->x;
        }
        
        //move to the next node
        temp = temp->next;
    }
    
    //return the  max vertex number
    return max;
}

//creates an array of vert* that point to nodes which contain the array index in
//counting terms
vert** getVertexList(edge *edgeList, int numverts) {
    
    //create an array of vert* equal to the size of the number of vertices
    vert **verts = malloc(sizeof(vert*)*numverts);
    
    //iterate through the vert array
    for (int i = 0; i < numverts; i++) {
        
        //at each index in the array, point that pointer to a new vert node
        verts[i] = malloc(sizeof(vert));
        //set that node's key to i+1 (0 would be 1, 1 would be 2, etc.) 
        //(simulates real counting)
        verts[i]->key = i+1;
        //establish it as not visited yet
        verts[i]->visited = NOT_VISITED;
        //set the node's next pointer to NULL
        verts[i]->next = NULL;
    }
    
    //return the array of vert pointers that point to singular nodes with NULL 
    //next pointers
    return verts;
}

//creates an array of pointers to adjs nodes that contain the adjacency list for 
//that vertex
adjs** getAdjs(edge *edgeList) {
    
    //this gets an array of adjs* structs of size equal to the number of
    //vertices. Each index in this array points to a node that corresponds to the 
    //vertex and contains two pointers to lists
    adjs** adjacent = initAdjs(getMaxVerts(edgeList));
    
    //create a temporary pointer  that points to the head of the edge list
    edge *head = edgeList;
    
    //iterate through the edge list
    while (head != NULL) {
        
        //point the adjList pointer to the point in the adjacent array 
        //that corresponds with the current vertex
        adjs *adjList = adjacent[(head->x) - 1];
        
        //create a right* that points to a new Vertex structure
        vert *mallocator = malloc(sizeof(vert));
        //set the key value of the vertex node equal to the right value of the 
        //edge list
        mallocator->key = head->y;
        //set the next pointer inside that node equal to the head of the unsorted 
        //list inside the node in the AdjacencyList
        mallocator->next = adjList->list;
        //set the list head of each list pointer equal to the the mallocated node, 
        //essentially an insertAtHead function
        adjList->list = mallocator;
       
        //iterate through to the next edge in the edge list
        head = head->next;
    }
    
    //return the array of adjs* structs that contain nodes with the vertex 
    //number and the adjacency list of that vertex in sorted and unsorted fashion
    return adjacent;
}

//receives the number of vertices and returns an array of adjs* structures
//that each point to a node that corresponds to a vertex, with each node containing
//a pointer to its adjacency list in sorted and unsorted fashions
adjs** initAdjs(int maxVerts) {
    
    //creates an array of adjs structures of size equal to the number of vertices
    adjs** list = malloc(sizeof(adjs*)*maxVerts);
    
    //iterate through the array of adjs structures
    for (int i = 0; i < maxVerts; i++) {
        //for each adjs* point it to a new adjs node
        list[i] = malloc(sizeof(adjs));
        //for each node that is created, set its value to i+1 (to simulate actual counting)
        list[i]->vertNum = i+1;
        //set that node's list pointer to NULL
        list[i]->list = NULL;
        //set that node's sorted list pointer to NULL
        list[i]->sortedList = NULL;
    }
    
    //return the array of adjs* structures that each point to a 
    //node that contains the corresponding vertex number and a pointer to an 
    //unsorted list and a sorted list
    return list;
}

//prints the adjacency lists
void printAdjs(adjs* lists[], int maxVerts) {
    //iterate through the array of pointers to adjs nodes
    for (int i = 0; i < maxVerts; i++) {
        //print the adjacency list for the current node
        printAdjList(lists[i]);
    }
}

//takes in an adjs node and prints the list held within
void printAdjList(adjs* adjs) {
    
    //set a temporary vertex pointer to point to the list held within the adjs struct
    vert *temp = adjs->list;
    
    //print the vertex number
    printf("%d->", adjs->vertNum);
    
    //iterate through the list while temp isn't NULL
    while (temp != NULL) {
        
        //this is for stylization, nothing else
        //makes the end of each line not have an arrow pointing to nothing
        if(temp->next == NULL)
            printf("%d", temp->key);
        
        //print out the key value, or the linked vertex to this one
        if(temp->next != NULL)
            printf("%d->", temp->key);
        
        //move to the next vertex node in the linked list
        temp = temp->next;
    }
    
    //print a newline for stylization purposes
    printf("\n");
}

//returns a vert** that contains the completion times of the components as they 
//finish in DFS
vert** getCompTimes(vert* verts[], adjs **adjsList, int maxVerts) {
    
    //set a temporary integer, index, equal to the number of vertices
    int max = maxVerts;
    
    //create a vert* result that points to an array of vertex* of size equal to 
    //the number of vertices
    vert **compTimes = malloc(sizeof(vert *)*maxVerts);
    
    //clear the visited vertices for DFS
    clearVisited(verts, maxVerts);
    
    //iterate from 0 to the highest vertex number
    for (int i = 0; i < maxVerts; i++) {
        
        //set the temporary pointer vertex to the current loop iteration's 
        //vertex
        vert *temp = verts[i];
        
        //if it hasn't been visited, call DFS on it
        if (temp->visited == NOT_VISITED) {
            DFS(temp, adjsList, maxVerts, compTimes, &max, verts);
        }
    }
    
    //return the array of vert pointers that contain the finish times of the 
    //vertices
    return compTimes;
}

//receives a vert* pointer and the number of vertices and clears the visitation 
//of the vertices for DFS
void clearVisited(vert **vertices, int vertexCount){
    
    //iterate through the number of vertices
    for (int i = 0; i < vertexCount; i++){
        
        //clear the visitation of the current vertex
        vertices[i]->visited = NOT_VISITED;
    }
    
    //return from void function
    return;
}

//depth first search on the current vertex that hasn't been visited
void DFS(vert *compTemp, adjs** adjsList, int maxVerts, vert** compTimes, 
        int *max, vert **verts) {
    
    //visit the vertex
    compTemp->visited = VISITED;

    //set a temporary vert equal to the index of the vert-1's list inside 
    //of the vertex structure
    vert *temp = adjsList[(compTemp->key)-1]->list;
    
    //iterate through that list until temp is null
    while (temp != NULL) {
        
        //declare temporary vertex pointer equal to the vertex's key - 1
        vert *compTemp2 = verts[(temp->key)-1];
        
        //if that vertex hasn't been visited, call dfs on that vertex
        if (compTemp2->visited == NOT_VISITED) {
            DFS(compTemp2, adjsList, maxVerts, compTimes, max, verts);
        }
        
        //iterate into the next vertex that hasn't been visited by this 
        //recursive call of dfs
        temp = temp->next;
    }

    //set temporary int tint equal to the value in max
    int tint = *max;
    //decrement t to access the corresponding index
    tint--;
    //access that index inside the vertex array of result and set that pointer 
    //equal to the corresponding vertex
    compTimes[tint] = compTemp;
    //set the value of max equal to tint, which has been decremented
    *max = tint;
    
    //return from the DFS
    return;
}

//short function to transpose the graph
edge* transposeEdges(edge *edgeList) {
    
    //initialize a new edge pointer to transpose the edge list with
    edge *revEdgeList = NULL;
    
    //temporary pointer to iterate through the edge list
    edge *temp = edgeList;
    
    //iterate through the edge list
    while (temp != NULL) {
        
        //use the new* to create a new edge and flip the sides of the standard graph
        //to represent a transpose of the graph
        edge *mallocator = malloc(sizeof(edge));
        mallocator->x = temp->y;
        mallocator->y = temp->x;
        mallocator->next = NULL;
        
        //when inserting into null pointer (case for empty list insertion)
        if (revEdgeList) {
            mallocator->next = revEdgeList;
            revEdgeList = mallocator;
        }
        
        //insert at head
        else 
            revEdgeList = mallocator;
        
        //move to the  next edge in the edge list
        temp = temp->next;
    }
    
    //return the reversed list of edges
    return revEdgeList;
}

//function to identify any SCCs in the graph based on the original graph's 
//completion times and the transposed list
int findSCCs(vert **verts, int maxVerts, vert **compTimes, 
        adjs **adjsList) {
    
    //clear the visitation of the graph for the second iteration of dfs to label
    //SCCs
    clearVisited(verts, maxVerts);
    
    //initialize the component count to 0
    int component = 0;
    
    //loop through the vert array and get the completion times for the 
    //transposed adjacencies
    printf("\nPrinting the vertexes and their derived SCCs\n");
    for (int i = 0; i < maxVerts; i++) {
        
        //use a temporary pointer to get the completion times of the transposed edges
        vert *temp = compTimes[i];
        
        //only call dfs if the color of the vertex hasn't been visited yet
        if (temp->visited == NOT_VISITED) {
            //add a component
            component++;
            //call modified dfs function to label the SCCs
            printf("\n");
            dfsSCCs(temp, component, adjsList, verts);
        }
    }
    
    //return the number of components
    return component;
}

//modified DFS function to label SCCs of the graph
void dfsSCCs(vert *temp, int compNum, adjs **adjsList, vert **verts) {
    
    //visit the vertex
    temp->visited = VISITED;
    
    //set the component number to the component value
    temp->compNum = compNum;
    
    //print the vertex and the part of the component that it is
    printf("Component %d consists of vertex %d.\n", compNum, temp->key);
    
    //temp2 is a temporary pointer that points to the adj struct for the 
    //specific vertex
    adjs *temp2 = adjsList[(temp->key) - 1];
    
    //temp3 is a temporary pointer that points to the list inside the adj 
    //structure
    vert *temp3 = temp2->list;
    
    //iterate through temp3 (the adjacency list of the vertex)
    while (temp3 != NULL) {
        
        //create a temporary pointer, temp4, to the corresponding vertex in the 
        //vertices array of vertexes
        vert *temp4 = verts[(temp3->key) - 1];
        
        //recursively call the modified DFS function on unvisited vertices in 
        //this iteration of DFS
        if (temp4->visited == NOT_VISITED)
            dfsSCCs(temp4, compNum, adjsList, verts);
        
        //move to the next neighbor in the vertex's adjacency list
        temp3 = temp3->next;
    }
    
    //return from the dfs function
    return;
}

//function to get the out-degree sequence of the components
void getOutDegs(vert **verts, edge *edgeList, int numComps, int maxVerts) {
    
    
    //create a matrix for this
    int** matrix = malloc(sizeof(int) * numComps);
    
    //loop through the matrix columns and calloc them, or initialize to 0
    for(int i = 0; i < maxVerts; i++){
        matrix[i] = calloc(sizeof(int), numComps);
    }

    //create an int array to hold the outdegrees of the components
    int outDegs[numComps];

    //set the values in it to 0
    for (int i = 0; i < numComps; i++) {
        outDegs[i] = 0;
    }

    //create a temporary compAdj pointer to hold the list of outdegrees
    compAdjs **temp = malloc(sizeof(compAdjs*)*numComps);

    //fill these with compAdjs to hold each out-degree of each component
    for (int i = 0; i < numComps; i++) {
        //make a new compAdj to fill
        temp[i] = malloc(sizeof(compAdjs));
        //set the compNum to the component number it corresponds to
        temp[i]->compNum = i;
        //initialize the outDegree
        temp[i]->outDeg = 0;
        //initialize the list* inside the struct
        temp[i]->list = NULL;
    }

    //create a temporary pointer to iterate through the edge list
    edge *tempEdgePtr = edgeList;
    
    //iterate through the edge list
    while (tempEdgePtr != NULL) {
        
        //holds the vertices in the list of vertices that correspond to the x and
        //y values of the edge list
        vert *tempX = verts[(tempEdgePtr->x) - 1];
        vert *tempY = verts[(tempEdgePtr->y) - 1];
        
        //check to see if there is a match between the compNum of x and y
        if (tempX->compNum != tempY->compNum) {
            
            //temporary pointer to hold the component's adjacency list
            compAdjs *tempCompAdj = temp[tempX->compNum];
            
            //initialize the sameComp variable to find a match between x and y
            //components
            int sameComp = 0;
            
            //temp2 holds a temporary compAdj list
            comp *temp2 = tempCompAdj->list;
            
            //iterate through the temporary compAdj list
            while (temp2 != NULL) {
                
                //see if the component number of x and y match, if they do
                //sameComp = 1
                if (tempY->compNum == temp2->compNum) {
                    sameComp = 1;
                }
                
                //move to the next item in the temporary compAdj list
                temp2 = temp2->next;
            }

            //if there isn't a match
            if (sameComp == 0) {
                if(matrix[(tempX->compNum)-1][(tempY->compNum)-1] != 1){
                    outDegs[(tempX->compNum)-1]++;
                }
                matrix[(tempX->compNum)-1][(tempY->compNum)-1] = 1;
            }

        }

        //iterate to the next edge in the list
        tempEdgePtr = tempEdgePtr->next;
    }
    
    //sort these out-degrees and print the sorted values
    countingSort(outDegs,numComps);
    
    //return from void function
    return;
}

//counts the number of edges in the edge list
int getNumEdges(edge *edgeList) {
    
    //initialize numEdges to 0
    int numEdges = 0;
    
    //set temp pointer edge equal to the head of the edge list
    edge *temp = edgeList;
    
    //iterate through the edge list and count the number of edge structures to get the number of edges total
    while (temp!= NULL) {
        numEdges++;
        temp = temp->next;
    }
    
    //return the number of edges
    return numEdges;
}

//implemented linear time counting sort for this program, receives an int array 
//and the number of components
void countingSort(int degSequence[],int numComps){
    
    //the two necessary arrays for the sort
    int sorting[numComps];
    int sorted[numComps];
    
    //for loop to initialize the arrays to 0;
    for(int i = 0; i < numComps; i++){
        sorting[i] = 0;
        sorted[i] = 0;
    }
    
    //for loop to count the number of occurrences in the array parameter, then store that number 
    //in the sorting array at the appropriate index
    for(int i = 0, temp = 0; i < numComps; i++){
        temp = degSequence[i];
        sorting[temp]++;
    }
    
    //sum an index with the next index, then store that sum in the second index
    for(int i = 0; i < numComps; i++){
        sorting[i+1] += sorting[i];
    }
    
    //insert the values as they correspond in the sorting and sorted arrays into the new sorted array
    for(int i = 0, temp = 0; i < numComps; i++){
        temp = degSequence[i];
        sorting[temp]--;
        temp = sorting[temp];
        sorted[temp] = degSequence[i];
    }
    
    //print it out
    printf("\nComponents\n");
    for(int i = numComps; i > 0; i--){
        printf("%d ", i);
    }
    
    printf("\nOut-Degrees\n");
    for(int i = 0; i < numComps; i++){
        printf("%d ", sorted[i]);
    }
    printf("\n");
    
    //returns from sort
    return;
}