#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libio.h>
#include <time.h>

#define CHARMAX 256

typedef struct scoreType {
    char cardinal; 			//punto cardinal mas poblado
    unsigned score; 			//cantidad de celulas vivas totales
    unsigned generation; 		//generaciones
    float ratio; 			//score/generation, da una idea de la tasa de crecimiento de las celulas
    struct tm *dateInfo; 		//estructura con informacion de la fecha y hora de la partida
    char date[CHARMAX]; 		//String con la fecha y hora
    unsigned revivalNumber; 		//cantidad de celulas adyacentes con las que revive
    unsigned suffocationNumber; 	//cantidad de celulas adyacentes a partir de las cuales muere
    unsigned isolationNumber; 		//debajo de esta cantidad de celulas adyacentes muere
    unsigned initialCells;              //Cantidad de celulas iniciales
    unsigned rows;                      //Cantidad de filas del mundo
    unsigned columns;                   //Cantidad de columnas del universo
} score_t;


//PROTOTIPOS

//escribe en un archivo la informacion de la partida
void logbookEntry(score_t* scoreData);


#endif /* FILEHANDLER_H */

