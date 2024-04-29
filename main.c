#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Define maze dimensions
int height;
int width;

// Define cache variables
int lastX;
int lastY;

// Define the the seed variable
time_t seed;

// Define the cell (grid square) struct
typedef struct cell
{
    int key;
    int xPos;
    int yPos;
    char type;
    int visited;
} cell;

// Define the graph edge struct
typedef struct edge
{
    int destKey;
} edge;

// Define the graph node struct
typedef struct node
{
    int key;
    int xPos;
    int yPos;
    int edgeCount;
    edge edges[4];
} node;

// Define the queue struct
typedef struct queue
{
    int pointer;
    node *arr[__INT16_MAX__*8192];
} queue;

// Instance the structs
queue nodeStack;
cell **grid;
node *graph;

// Define the stack push method
void push(node *n)
{
    // Increment the pointer (the top)
    nodeStack.pointer++;
    // Set the top to the address of the new element
    nodeStack.arr[nodeStack.pointer] = n;
}

// Define the pop method
void pop()
{
    // Decrement the pointer (the top)
    nodeStack.pointer--;
}

// Define the addNeighbour method
void addNeighbour(int key, int destKey)
{
    // Iterate through the height and the width
    for (int i = 0; i < height * width; i++)
    {
        // Find the matching node
        if (graph[i].key == key)
        {
            // Add the edge and increment the edgeCount
            edge e;
            e.destKey = destKey;

            graph[i].edges[graph[i].edgeCount] = e;
            graph[i].edgeCount++;
        }
    }
}

// Define the generateGraph method
void generateGraph()
{
    // Iterate through the height and the width
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get the key of the cell
            int key = grid[i][j].key;
            // Check each side of the current cell (1 row away)
            // If it exists add it as a neighbour
            if (i - 2 >= 0)
            {
                addNeighbour(key, grid[i - 2][j].key);
            }

            if (j - 2 >= 0)
            {
                addNeighbour(key, grid[i][j - 2].key);
            }

            if (i + 2 < height)
            {
                addNeighbour(key, grid[i + 2][j].key);
            }

            if (j + 2 < width)
            {
                addNeighbour(key, grid[i][j + 2].key);
            }
        }
    }
};

// Define the generate grid  method
void generateGrid()
{
    // Define the count variable
    int count = 0;
    // Allocate memory for the grid
    grid = malloc(height * sizeof(cell *));

    // Loop through each row
    for (int i = 0; i < height; i++)
    {
        // Allocate memory for the row
        grid[i] = malloc(width * sizeof(cell));

        // Generate a cell and node struct for each position
        for (int j = 0; j < width; j++)
        {
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

// Define the selectRand method
cell *selectRand(int x, int y)
{
    // Reference the selectedNode and define prerequisite variables
    node selectedNode = graph[grid[x][y].key];
    int freeEdges = 0;
    node edgeList[4];

    // Iterate through the selectedNodes edges
    for (int i = 0; i < selectedNode.edgeCount; ++i)
    {
        // Reference the targetNode and check to see if it's been visited
        node targetNode = graph[selectedNode.edges[i].destKey];
        if (grid[targetNode.xPos][targetNode.yPos].visited == 0)
        {
            // If it hasn't been visisted append to the list of freeEdges and increment the counter
            edgeList[freeEdges] = targetNode;
            freeEdges++;
        }
    };
    // If the freeEdges are 0 return NULL
    if (freeEdges == 0)
    {
        return NULL;
    }
    // Randomly select an edge and return the address of the cell corresponding to this
    int selection = (rand() % freeEdges);
    return &grid[edgeList[selection].xPos][edgeList[selection].yPos];
};

// Define the removeWall method
void removeWall(node *rootNode, node *selectedNode)
{
    // Calculate the difference in the X and Y of the current and selected node
    int deltaX = selectedNode->xPos - rootNode->xPos;
    int deltaY = selectedNode->yPos - rootNode->yPos;

    cell *targetCell;

    // Find the targetCell corresponding to the X and Y differences
    switch (deltaX < 0 ? 0 : deltaY < 0 ? 2 : deltaX > 0 ? 1 : 3)
    {
    case 0:
        targetCell = &grid[rootNode->xPos - 1][rootNode->yPos];
        break;
    case 1:
        targetCell = &grid[rootNode->xPos + 1][rootNode->yPos];
        break;
    case 2:
        targetCell = &grid[rootNode->xPos][rootNode->yPos - 1];
        break;
    case 3:
        targetCell = &grid[rootNode->xPos][rootNode->yPos + 1];
        break;
    }

    // Mark the target cell as visited
    targetCell->visited = 1;

    // Update the last x and y positions
    lastX = targetCell->xPos;
    lastY = targetCell->yPos;
    
    // Break the wall of the targetCell
    targetCell->type = ' ';

}

// Define the pathGeneration method
void pathGeneration()
{
    // Chose a random x and y for the initial start
    int randX = 1 + (rand() % (height - 1));
    int randY = 1 + (rand() % (width - 1));

    // Push the node of these position onto the stack
    push(&graph[grid[randX][randY].key]);
    
    // Mark the cell as visited and set its type to start
    grid[randX][randY].visited = 1;
    grid[randX][randY].type = 'S';

    // Iterate while there's items on the stack
    while (nodeStack.pointer > 0)
    {
        // Generate a pointer for the current cell and selected cells
        cell *currentCell = &grid[nodeStack.arr[nodeStack.pointer]->xPos][nodeStack.arr[nodeStack.pointer]->yPos];
        cell *selected = NULL;
        
        // Randomly select a cell and store the address
        selected = selectRand(currentCell->xPos, currentCell->yPos);

        // If the selected cell doens't exist turn break the wall of the cuurrent cell and pop from the stack
        if (selected == NULL)
        {
            if (currentCell->type != 'S')
            {
                currentCell->type = ' ';
            }
            pop();
        }
        // Mark the selected cell as visited and remove the wall between it and the current cell
        // Push the selected cell onto the stack
        else
        {
            selected->visited = 1;
            removeWall(nodeStack.arr[nodeStack.pointer], &graph[selected->key]);
            push(&graph[selected->key]);
        }
    }

    // If all cells have been visited mark the final one as the end
    grid[lastX][lastY].type = 'E';
};

// Define the main method
int main(int argc, char *argv[])
{
    if (argc < 3) { printf("./maze [height] [width]\n"); return 1; }

    // Get the height and width from the arguments
    height = atoi(argv[1]);
    width = atoi(argv[2]);

    // If a fourth argument is specified use this as the seed
    if (argc == 4)
    {
        seed = atoi(argv[3]);
        srand(seed);
    }
    // Generate a random seed based on the time
    else
    {
        seed = time(NULL);
        srand(seed);
    }

    // Allocate memory for the graph
    graph = malloc(height * width * sizeof(node));

    // Call the generation methods
    generateGrid();
    generateGraph();
    pathGeneration();

    // Output the maze and seed used
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            printf("%c ", grid[i][j].type);
        }
        printf("\n");
    }
    printf("Generated using seed: %li\n", seed);

    // Save the maze to a file
    char mazeFP[1024];

    sprintf(mazeFP, "maze_%li.txt", seed);

    FILE *fp = fopen(mazeFP, "w");

    if (fp == NULL)
    {
        printf("ERROR OPENING FILE\n%s\n", mazeFP);
    }

    for (int i = 0; i < height; ++i)
    {
        char line[width];
        for (int j = 0; j < width; ++j)
        {
            line[j] = grid[i][j].type;
        }
        line[width] = '\0';
        fprintf(fp, "%s\n", line);
    }

    fclose(fp);

    // Free all allocated memory
    for (int i = 0; i < height; ++i)
    {
        free(grid[i]);
    }
    free(grid);
    free(graph);

    return 0;
}