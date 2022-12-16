/**
 * @file life.h
 * @brief Header file for Game of Life program
 * @author Brady Spencer
 * @date 2022-05-01
 */

#ifndef LIFE_H_
#define LIFE_H_

unsigned char **init_matrix(int rows, int cols, char* seed, char *edge, int initX, int initY);
unsigned char **update_matrix(unsigned char **gridA, unsigned char **gridB, int rows, int cols, char *edge);



#endif
