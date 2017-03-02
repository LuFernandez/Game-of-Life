#ifndef FSM_H
#define FSM_H
#include "display.h"
#include "cell.h"
#include "fileHandler.h"

typedef enum {
    PLAY, PAUSE, FASTER, SLOWER, SINGLE, SETTINGSBUTTON, HOMEBUTTON, CREDITSBUTTON, QUIT, HELPBUTTON, MAP,
    RULE1UP, RULE1DOWN, RULE2UP, RULE2DOWN, RULE3UP, RULE3DOWN,
    RULE4COLUP, RULE4COLDOWN, RULE4ROWUP, RULE4ROWDOWN, RULE5UP, RULE5DOWN, TIMEUP, TIMEDOWN, NOEVENT, GAMEOVER, EVENTCOUNT
} event_t; //eventos

typedef enum {
    HOME, SETTINGS, SINGLEPLAYING, PAUSEGAME, SINGLESETTING, CREDITS, HELP, STATECOUNT
} state_t; //estados de la fsm

typedef enum {
    HOMESCREEN, SETTINGSSCREEN, HELPSCREEN, PLAYINGSCREEN, CREDITSSCREEN, SCREENCOUNT
} screen_t; //pantallas


#define MAXROWS 100
#define MAXCOLUMNS 100
#define MAXCELLS (MAXCOLUMNS*MAXROWS)



typedef struct userData {
    state_t state;
    screen_t currentScreen;
    event_t event;
    cell_t** currentGen;                //puntero a la matriz actual
    cell_t** nextGen;                   //puntero a la matriz de la generacion siguiente
    unsigned gameover;                  //flag para ver si la generacion siguiente y la actual son iguales
    unsigned gameTime;  
    unsigned timeLapse;
    unsigned inputCells;           //cantidad de celulas restantes que puede ingresar el usuario
    unsigned cellsNumber;          //Contador de celulas a posicionar en el mapa 
    unsigned rows;
    unsigned columns;
    int buttonClicked;                  //qué boton se clickeo
    double mouseX;                      //las coordenadas del mouse click
    double mouseY;
    unsigned revivalNumber;         //cantidad de celulas adyacentes con las que revive
    unsigned suffocationNumber;     //cantidad de celulas adyacentes a partir de las cuales muere
    unsigned isolationNumber;       //debajo de esta cantidad de celulas adyacentes muere
    cardinal_t cardinalData;
    score_t currentScore;               //informacion para las estadisticas
} userData_t;

typedef struct fsm_cell
{
	state_t nxtstate;		//estado
	void(*funcion)(userData_t* userData, display_t* display);		//puntero a la funcion de transicion
} fsmCell_t;

//PROTOTIPOS

//setea parametros por defecto para la partida
void defaultSettings(userData_t* userData);
//Crea la FSM
fsmCell_t** createFSM(void);
//avanza la fsm
void FSMCycle(fsmCell_t** FSMMatrix, userData_t* userData, display_t* displayData);
//Destruye la FSM
void deinitFSMMatrix(fsmCell_t** FSMMatrix);
#endif /* FSM_H */

