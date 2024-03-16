#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int height;
int width;

typedef struct cell {
    int key;
    int xPos;
    int yPos;
    char type;
    int visited;
} cell;

typedef struct edge {
    int destKey;
    int weight;
} edge;

typedef struct node {
    int key;
    int xPos;
    int yPos;
    int edgeCount;
    edge edges[4];
} node;

typedef struct queue {
    int arrSize;
    int head;
    int tail;
    node *arr[1024];
} queue;

queue nodeQueue;
cell **grid;
node *graph;

void enqueue(node *n) {
    nodeQueue.arr[nodeQueue.tail] = n;
    nodeQueue.tail++;
    nodeQueue.arrSize++;
}

void dequeue() {
    nodeQueue.head++;
    nodeQueue.arrSize--;
}

void addNeighbour(int key, int destKey, int weight) {
    for(int i = 0; i < height*width; i++) {
        if (graph[i].key == key) {
            edge e;
            e.destKey = destKey;
            e.weight = weight;

            graph[i].edges[graph[i].edgeCount] = e; 
            graph[i].edgeCount++;
        }
    }
}

void generateGraph() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j ++ ) {
            int key = grid[i][j].key;
            int randWeight = (rand() % 5);
            if (i - 1 >= 0) {
                addNeighbour(key, grid[i-1][j].key, randWeight);
            }
            
            if (j - 1 >= 0) {
                addNeighbour(key, grid[i][j-1].key, randWeight);
            }

            if (i + 1 < height) {
                addNeighbour(key, grid[i+1][j].key, randWeight);
            }
            
            if (j + 1 < width) {
                addNeighbour(key, grid[i][j+1].key, randWeight);
            }
        }
            
    }
};

void generateGrid() {
    int count = 0;
    grid = malloc(height * sizeof(cell *));
    
    for (int i = 0; i < height; ++i) {
        grid[i] = malloc(width * sizeof(cell));
        
        for (int j = 0; j < width; j++) {
            cell c;
            c.key = count;
            c.xPos = i;
            c.yPos = j;
            c.type = '#';
            c.visited = 0;
            grid[i][j] = c;
            

            node n;
            n.key = count;
            n.xPos = i;
            n.yPos = j;
            n.edgeCount = 0;
            graph[count] = n;

            ++count;
        }
    }
}



cell* selectRand(int x, int y) {
    node selectedNode = graph[grid[x][y].key];
    int freeEdges = 0;
    for (int i = 0; i<selectedNode.edgeCount; i++) {
        node targetNode = graph[selectedNode.edges[i].destKey];
        if (grid[targetNode.xPos][targetNode.yPos].visited == 0) { freeEdges++; }
    };
    if (freeEdges == 0) { return NULL; }
    printf("Selecting");
    int selection = (rand() % 4);
    
    switch(selection) {
        case 0:
            return &grid[x-1][y];
        case 1:
            return &grid[x+1][y];
        case 2:
            return &grid[x][y-1];
        case 3:
            return &grid[x][y+1];
        default:
            return &grid[x+1][y];
    }
};

void flagWalls(int x, int y) {
    node selectedNode = graph[grid[x][y].key];
     for (int i = 0; i<selectedNode.edgeCount; i++) {
        node targetNode = graph[selectedNode.edges[i].destKey];
        grid[targetNode.xPos][targetNode.yPos].visited = 1;
    };
}

void pathGeneration() {
    int randX = 1 + (rand() % (height-1));
    int randY = 1 + (rand() % (width-1));

    enqueue(&graph[grid[randX][randY].key]);
    grid[randX][randY].visited = 1;
    
    while(nodeQueue.arrSize > 0) {
        printf("resetting\n");
        cell *selected = NULL;
        while(1>0) {
            selected = selectRand(nodeQueue.arr[nodeQueue.head]->xPos, nodeQueue.arr[nodeQueue.head]->yPos);
            if (selected == NULL) { break; }
            else if(selected != NULL && !selected->visited) { break; }
        }
        printf("passed");
        if (selected == NULL) {
            dequeue();
        } else {
            selected->visited = 1;
            grid[nodeQueue.arr[nodeQueue.head]->xPos][nodeQueue.arr[nodeQueue.head]->yPos].type = ' ';

            flagWalls(nodeQueue.arr[nodeQueue.head]->xPos, nodeQueue.arr[nodeQueue.head]->yPos);
            enqueue(&graph[selected->key]);
        }

        
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                printf("%c ", grid[i][j].type);
            }
            printf("\n");
        }
    }
};

int main(int argc, char *argv[]) {
    srand(time(NULL));

    height = atoi(argv[1]);
    width = atoi(argv[2]);

    graph = malloc(height*width*sizeof(node));

    generateGrid();
    generateGraph();
    pathGeneration();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            printf("%c ", grid[i][j].type);
        }
        printf("\n");
    }

    free(grid);
}