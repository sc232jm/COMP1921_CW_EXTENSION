#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int height;
int width;

int lastX;
int lastY;

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
    int pointer;
    node *arr[1024];
} queue;

queue nodeQueue;
cell **grid;
node *graph;

void push(node *n) {
    nodeQueue.pointer++;
    nodeQueue.arr[nodeQueue.pointer] = n;
    nodeQueue.arrSize++;
}

void pop() {
    nodeQueue.pointer--;
}

void addNeighbour(int key, int destKey) {
    for(int i = 0; i < height*width; i++) {
        if (graph[i].key == key) {
            edge e;
            e.destKey = destKey;
            
            graph[i].edges[graph[i].edgeCount] = e; 
            graph[i].edgeCount++;
        }
    }
}

void generateGraph() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j ++ ) {
            int key = grid[i][j].key;
            if (i - 2 >= 0) {
                addNeighbour(key, grid[i-2][j].key);
            }
            
            if (j - 2 >= 0) {
                addNeighbour(key, grid[i][j-2].key);
            }

            if (i + 2 < height) {
                addNeighbour(key, grid[i+2][j].key);
            }
            
            if (j + 2 < width) {
                addNeighbour(key, grid[i][j+2].key);
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
    node edgeList[4];

    for (int i = 0; i<selectedNode.edgeCount; i++) {
        node targetNode = graph[selectedNode.edges[i].destKey];
        if (grid[targetNode.xPos][targetNode.yPos].visited == 0) { edgeList[freeEdges] = targetNode; freeEdges++; }
    };
    if (freeEdges == 0) { return NULL; }
    int selection = (rand() % freeEdges);
    return &grid[edgeList[selection].xPos][edgeList[selection].yPos];
};

void removeWall(node *rootNode, node *selectedNode) {
    int deltaX = selectedNode->xPos - rootNode->xPos;
    int deltaY = selectedNode->yPos - rootNode->yPos;

    cell *targetCell;

    switch(deltaX < 0 ? 0 : deltaY < 0 ? 2 : deltaX > 0 ? 1 : 3) {
        case 0:
            targetCell = &grid[rootNode->xPos-1][rootNode->yPos];
            break;
        case 1:
            targetCell = &grid[rootNode->xPos+1][rootNode->yPos];
            break;
        case 2:
            targetCell = &grid[rootNode->xPos][rootNode->yPos-1];
            break;
        case 3:
            targetCell = &grid[rootNode->xPos][rootNode->yPos+1];
            break;
    }

    targetCell->visited = 1;
    if (targetCell->type != 'S') {
        
        lastX = targetCell->xPos;
        lastY = targetCell->yPos;
       targetCell->type = ' ';
    }
}

void pathGeneration() {
    int randX = 1 + (rand() % (height-1));
    int randY = 1 + (rand() % (width-1));

    push(&graph[grid[randX][randY].key]);
    grid[randX][randY].visited = 1;
    grid[randX][randY].type = 'S';
    
    while(nodeQueue.pointer > 0) {
        cell *selected = NULL;
        selected = selectRand(nodeQueue.arr[nodeQueue.pointer]->xPos, nodeQueue.arr[nodeQueue.pointer]->yPos);

        if (selected == NULL) {
            if (grid[nodeQueue.arr[nodeQueue.pointer]->xPos][nodeQueue.arr[nodeQueue.pointer]->yPos].type != 'S') {
                grid[nodeQueue.arr[nodeQueue.pointer]->xPos][nodeQueue.arr[nodeQueue.pointer]->yPos].type = ' ';
            }
            pop();
        } else {
            selected->visited = 1;
            removeWall(nodeQueue.arr[nodeQueue.pointer], &graph[selected->key]);
            push(&graph[selected->key]);
        }
    }

    grid[lastX][lastY].type = 'E';
};

int main(int argc, char *argv[]) {
    height = atoi(argv[1]);
    width = atoi(argv[2]);
    if(argc == 4) {
        srand(atoi(argv[3]));
    } else {
        srand(time(NULL));
    }

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