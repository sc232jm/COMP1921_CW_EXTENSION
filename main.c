#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **grid;
int height;
int width;

void fillGrid() {
    grid = malloc(height * sizeof(char *));

    for (int i = 0; i < height; ++i) {
        grid[i] = malloc(width * sizeof(char));
        memset(grid[i], '#', width*sizeof(char));
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    height = atoi(argv[1]);
    width = atoi(argv[2]);

    fillGrid();

    free(grid);
}