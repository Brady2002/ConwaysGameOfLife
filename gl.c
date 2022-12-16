/**
 * @file gl.c
 * @brief main function for Game of Life
 * @details 
 * @author Brady Spencer
 * @date 2022-05-01
 * @todo
 * @bug
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "SDL2/SDL.h"
#include "sdl.h"
#include "life.h"

/**
 * @brief main function for Game of Life
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
	int width;
	int height;
	/* colors are RGB model valid values [0, 255] */
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	int sprite_size; /* either 2, 4, 8, or 16 */
	char *seed = NULL;
	char *edge = NULL;
	char *initCoords = NULL;
	int initX;
	int initY;

	int opt;
	while((opt = getopt(argc, argv, ":w:h:r:g:b:s:f:e:o:H")) != -1)
	{
		switch (opt)
		{
			case 'w':
				width = atoi(optarg);
				if (width < 0)
				{
					printf("Not a valid width, defaulting to 800...\n");
					width = 800;
				}
				break;
			case 'h':
				height = atoi(optarg);
				if (height < 0)
				{
					printf("Not a valid height, defaulting to 800...\n");
					height = 800;
				}
				break;
			case 'r':
				red = atoi(optarg);
				if (red > 255)
				{
					printf("Not a valid RGB Value, defaulting to 1...\n");
					red = 255;
				}
				else if (red < 0)
				{
					printf("Not a valid RGB Value, defaulting to 1...\n");
					red = 0;
				}
				break;
			case 'g':
				green = atoi(optarg);
				if (green > 255)
				{
					printf("Not a valid RGB Value, defaulting to 255...\n");
					green = 255;
				}
				else if (green < 0)
				{
					printf("Not a valid RGB Value, defaulting to 255...\n");
					green = 0;
				}
				break;
			case 'b':
				blue = atoi(optarg);
				if (blue > 255)
				{
					printf("Not a valid RGB Value, defaulting to 1...\n");
					blue = 1;
				}
				else if (blue < 0)
				{
					printf("Not a valid RGB Value, defaulting to 1...\n");
					blue = 1;
				}
				break;
			case 's':
				sprite_size = atoi(optarg);
				if (sprite_size != 2 || sprite_size != 4 || sprite_size != 8 || sprite_size != 16)
				{
					if (sprite_size != 4)
					{
						if (sprite_size != 8)
						{
							if (sprite_size != 16)
							{
								printf("Not a valid sprite size, defaulting to 4...\n");
								sprite_size = 4;
							}
						}
					}
				}
				break;
			case 'f':
				seed = optarg;
				break;
			case 'e':
				edge = optarg;
				if (strcmp(edge, "hedge") != 0)
				{
					if (strcmp(edge, "torus") != 0)
					{
						if (strcmp(edge, "klein") != 0)
						{
							printf("Not a recognized edge case, defaulting to torus...\n");
							edge = "torus";
						}
					}
				}
				break;
			case 'o':
				initCoords = optarg;
				break;
			case 'H':
				printf("\n-w: width of screen\n-h: height of screen\n-e: Edge type (Hedge, Torus, Klein)\n-r, -g, -b: RGB Values for Red, Green, and Blue respectively (0, 255)\n-s: Sprite Size (2, 4, 8, 16)\n-f: Filename for starting seed\n-o: X and Y coordinates for starting seed position (x,y)\n-H: Help\n\n");
				break;
			case ':':
				printf("Missing argument for %c\n", optopt);
				return 0;
				break;
			default:
				break;
		}
	}

    //this is needed to graphically display the game
    struct sdl_info_t sdl_info;
        
    /* set up SDL -- works with SDL2 */
	init_sdl_info(&sdl_info, width, height, sprite_size, red, green, blue);

	//Initialize gridA and gridB matrix
	int rows = (width / sprite_size);
	int cols = (height / sprite_size);

	char *ptr = strtok(initCoords, ",");
	int count = 0;

    while(ptr != NULL)
    {
    	if (count == 0)
        {
        	initX = atoi(ptr);
            count ++;
            ptr = strtok(NULL, ",");
        }
        else
        {
        	initY = atoi(ptr);
            count = 0;
            ptr = strtok(NULL, ",");
		}
    }

	if (initX > cols)
	{
		printf("Initial X value exceeds size of array, defaulting to the middle...\n");
		initX = cols / 2;
	}
	else if (initX < 0)
	{
		printf("Initial X value exceeds size of array, defaulting to the middle...\n");
		initX = cols / 2;
	}

	if (initY > rows)
	{
		printf("Initial Y value exceeds size of array, defaulting to the middle...\n");
		initY = rows / 2;
	}
	else if (initY < 0)
	{
		printf("Initial Y value exceeds size of array, defaulting to the middle...\n");
		initY = rows / 2;
	}

	unsigned char **gridA = init_matrix(rows, cols, seed, edge, initX, initY);
	unsigned char **gridB = init_matrix(rows, cols, seed, edge, initX, initY);
	
    /* Main loop: loop forever. */
	while (1)
	{
		/* your game of life code goes here  */		
		if (SDL_GetTicks() % 30 == 0)
		{
			sdl_render_life(&sdl_info, gridA);

			gridA = update_matrix(gridA, gridB, rows, cols, edge);

			for (int i = 0; i < rows; i++)
        	{
            	for (int j = 0; j < cols; j++)
            	{
                	gridB[i][j] = gridA[i][j];
            	}
        	}
		}
		
        /* Poll for events, and handle the ones we care about. 
        You can click the X button to close the window */
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
                        /* If escape is pressed, return (and thus, quit) */
				if (event.key.keysym.sym == SDLK_ESCAPE)
					return 0;
				break;
			case SDL_QUIT:
				return(0);
			}
		}
	}

	for (int i = 0; i < rows; i++)
	{
		free(gridA[i]);
	}
	free(gridA);

	for (int i = 0; i < rows; i++)
	{
		free(gridB[i]);
	}
	free(gridB);

	return 0;
}