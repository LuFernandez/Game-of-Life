#ifndef CELL_H
#define CELL_H

#include <stdio.h>
#include <stdlib.h>

enum {FALSE, TRUE};
enum {DIFFERENT, SAME};
enum {NORTH_WEST = 1, NORTH, NORTH_EAST, WEST, CENTER, EAST, SOUTH_WEST, SOUTH, SOUTH_EAST};

typedef enum {DEAD, ALIVE} cellState_t;

typedef struct cell 
{
    cellState_t state;
    unsigned inhabited;
}cell_t;

typedef struct                      //dividimos el mapa en 9 cuadrantes, Noroeste, Norte, Noreste, Oeste, Centro, Este, Sudoeste, Sur, Sureste.
{
    unsigned firstParallel;         //indice de la matriz donde se encuentra el primer paralelo
    unsigned secondParallel;        //el segundo
    unsigned firstMeridian;         //indice de la matriz donde se encuentra el primer meridiano
    unsigned secondMeridian;        //el segundo
    unsigned NWcells;               //cantidad de celulas en el noroeste
    unsigned Ncells;                //norte
    unsigned NEcells;               //noreste
    unsigned Wcells;                //oeste
    unsigned Ccells;                //centro
    unsigned Ecells;                //este
    unsigned SWcells;               //suroeste
    unsigned Scells;                //sur
    unsigned SEcells;               //sureste
} cardinal_t;


//PROTOTIPOS

//actualiza los estados de cada celula (ALIVE or DEAD)
unsigned updateStates(cell_t** currentGen, cell_t** nextGen, unsigned rows, unsigned columns, unsigned suffocationNumber, unsigned isolationNumber, unsigned revivalNumber, cardinal_t* cardinalData);

//borra los bordes de la matriz
void killEdges(unsigned rows, unsigned columns, cell_t** cellMatrix);

//cambia el estado de una celula dada la posicion en la matriz
void toggleCell(cell_t** cellMatrix, unsigned i, unsigned j, unsigned* cellsLeft);

//se fija si hubo cambio entre la generacion actual y la siguiente
unsigned checkNoChange(cell_t** matrix1, cell_t** matrix2, unsigned rows, unsigned columns);

//devuelve el estado de la celula dada la posicion
cellState_t getCellState(cell_t** cellMatrix, unsigned i, unsigned j);

//desaloja el espacio reservado para la matriz, devuelve puntero a NULL
cell_t** deinitCellMatrix(cell_t** cellMatrix, unsigned rows);

//reserva espacio para la matriz y devuelve un puntero a ella
cell_t** initCellMatrix(unsigned rows, unsigned columns);

#endif /* CELL_H */

