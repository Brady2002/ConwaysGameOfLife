/**
 * @file life.c
 * @brief Contains functions for Game of Life Program
 * @details
 * @author Brady Spencer
 * @date 2022-05-01
 * @todo
 * @bug
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "life.h"

#define MAX 20

/**
 * @brief Initializes matrix based on user defined size and seed from file
 * 
 * @param rows 
 * @param cols 
 * @param seed 
 * @param edge 
 * @param initX 
 * @param initY 
 * @return unsigned char** 
 */
unsigned char **init_matrix(int rows, int cols, char *seed, char *edge, int initX, int initY)
{
    unsigned char **a;
    
    /* allocate rows */
    a = malloc(rows * sizeof(unsigned char *));
    
    if(!a)
    {
        return NULL;
    }

    for(int i = 0; i < rows; i++)
    {
        /* allocate cols for each row */
        a[i] = malloc(cols * sizeof(unsigned char));
        
        if (!a[i])
        {
            for (int j = 0; j < i; j++)
            {
                free(a[j]);
                free(a);
            }
            
        return NULL;
        }
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            a[i][j] = 0;
        }
    }

    char folder[] = "seeds/";
    strcat(folder, seed);

    FILE *file = fopen(folder, "r");
    char line[MAX] = {0};
    const char space[2] = " ";
    int lineNum = 0;

    while(fgets(line, MAX, file))
    {
        if (lineNum == 0)
        {
            lineNum++;
            continue;
        }

        char *ptr = strtok(line, space);
        int count = 0;
        int x;
        int y;

        while(ptr != NULL)
        {
            if (count == 0)
            {
                x = atoi(ptr);
                count ++;
                ptr = strtok(NULL, space);
            }
            else
            {
                y = atoi(ptr);
                count = 0;
                ptr = strtok(NULL, space);
            }
        }

        int yPos = y + initY;
        int xPos = x + initX;

        a[yPos][xPos] = 1;
    }

    return a;
}

/**
 * @brief updates matrix, checking each cell and its adjacent cell values to see if it should be alive or dead.
 * 
 * @param gridA 
 * @param gridB 
 * @param rows 
 * @param cols 
 * @param edge 
 * @return unsigned char** 
 */
unsigned char **update_matrix(unsigned char **gridA, unsigned char **gridB, int rows, int cols, char *edge)
{
    int midCol = (cols - 1) / 2;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int aliveCount = 0;
            int deadCount = 0;

            int tempI = 0;
            int tempJ = 0;

            //Edge Style Checks
            if (strcmp(edge, "hedge") == 0)
            {
                //Hedge kills all cells on the edges of the grid
                if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1)
                {
                    gridA[i][j] = 0;
                    continue;
                }

                //N
                if (gridB[i-1][j] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //NE
                if (gridB[i-1][j+1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //E
                if (gridB[i][j+1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //SE
                if (gridB[i+1][j+1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //S
                if (gridB[i+1][j] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //SW
                if (gridB[i+1][j-1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //W
                if (gridB[i][j-1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //NW
                if (gridB[i-1][j-1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
            }
            else if (strcmp(edge, "torus") == 0)
            {                
                if (i == 0)
                {
                    tempI = rows - 2;
                }
                else if (i == rows - 1)
                {
                    tempI = -rows + 2;
                }

                if (j == 0)
                {
                    tempJ = cols - 2;
                }
                else if (j == cols - 1)
                {
                    tempJ = -cols + 2;
                }

                //N
                if (gridB[i+tempI-1][j+tempJ] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //NE
                if (gridB[i+tempI-1][j+tempJ+1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //E
                if (gridB[i+tempI][j+tempJ+1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //SE
                if (gridB[i+tempI+1][j+tempJ+1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //S
                if (gridB[i+tempI+1][j+tempJ] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //SW
                if (gridB[i+tempI+1][j+tempJ-1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //W
                if (gridB[i+tempI][j+tempJ-1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //NW
                if (gridB[i+tempI-1][j+tempJ-1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
            }
            else if (strcmp(edge, "klein") == 0)
            {
                int distToMid;

                if (j == 0)
                {
                    tempJ = cols - 2;
                }
                else if (j == cols - 1)
                {
                    tempJ = -cols + 2;
                }

                if (i == 0)
                {
                    tempI = rows - 2;
                    distToMid = j - midCol;
                    tempJ = midCol - distToMid - j;
                }
                else if (i == rows - 1)
                {
                    tempI = -rows + 2;
                    distToMid = j - midCol;
                    tempJ = midCol - distToMid - j;
                }

                //N
                if (gridB[i+tempI-1][j+tempJ] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //NE
                if (gridB[i+tempI-1][j+tempJ+1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //E
                if (gridB[i+tempI][j+tempJ+1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //SE
                if (gridB[i+tempI+1][j+tempJ+1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //S
                if (gridB[i+tempI+1][j+tempJ] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //SW
                if (gridB[i+tempI+1][j+tempJ-1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //W
                if (gridB[i+tempI][j+tempJ-1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
                //NW
                if (gridB[i+tempI-1][j+tempJ-1] == 1)
                {
                    aliveCount++;
                }
                else
                {
                    deadCount++;
                }
            }
            
            //Should grid be alive or dead
            if (gridB[i][j] == 1 && aliveCount < 2)
            {
                gridA[i][j] = 0;
            }
            else if (gridB[i][j] == 1 && aliveCount > 3)
            {
                gridA[i][j] = 0;
            }
            else if (gridB[i][j] == 1 && aliveCount == 3)
            {
                gridA[i][j] = 1;
            }
            else if (gridB[i][j] == 1 && aliveCount == 2)
            {
                gridA[i][j] = 1;
            }
            else if (gridB[i][j] == 0 && aliveCount == 3)
            {
                gridA[i][j] = 1;
            }
            else
            {
                gridA[i][j] = 0;
            }
        }
    }
    return gridA;
}