#include "FSM.h"


static void doNothing(userData_t* userData, display_t* diplayData);
//setea el estado correspondiente de la celula que se clickeo
static void setCell(userData_t* userData, display_t* displayData);
//pausa el juego
static void pauseGame(userData_t* userData, display_t* displayData);
//unpause
static void resumeGame(userData_t* userData, display_t* displayData);
//aumenta la velocidad del juego
static void goFaster(userData_t* userData, display_t* displatData);
//disminuye la velocidad del juego
static void goSlower(userData_t* userData, display_t* displatData);
//dibuja la pantalla de juego
static void showSingleDisplay(userData_t* userData, display_t* displayData);
//avanza generacion y la muestra en pantalla
static void update(userData_t* userData, display_t* displayData);
//muestra pantalla de settings
static void showSettings(userData_t* userData, display_t* displayData);
//muestra pantalla de credits
static void showCredits(userData_t* userData, display_t* displayData);
//muestra pantalla principal (menu)
static void showHome(userData_t* userData, display_t* displayData);
//sale del juego
static void quit(userData_t* userData, display_t* displayData);
//muestra ayuda
static void help(userData_t* userData, display_t* displayData);
//reglas
//R1: sofocacion
static void r1Up(userData_t* userData, display_t* displayData);
static void r1Down(userData_t* userData, display_t* displayData);
//R2: Soledad 
static void r2Up(userData_t* userData, display_t* displayData);
static void r2Down(userData_t* userData, display_t* displayData);
//R3: Resurreccion
static void r3Up(userData_t* userData, display_t* displayData);
static void r3Down(userData_t* userData, display_t* displayData);
//R4 rows and columns
static void r4rUp(userData_t* userData, display_t* displayData);
static void r4rDown(userData_t* userData, display_t* displayData);
static void r4cUp(userData_t* userData, display_t* displayData);
static void r4cDown(userData_t* userData, display_t* displayData);
//R5 initial cells
static void r5Up(userData_t* userData, display_t* displayData);
static void r5Down(userData_t* userData, display_t* displayData);
//Tiempo de juego
static void timeDown(userData_t* userData, display_t* displayData);
static void timeUp(userData_t* userData, display_t* displayData);
//reserva memoria para las matrices e inicia variables para el comienzo de la partida
static void settingSinglePlaying(userData_t* userData, display_t* displayData);
//muestra cartel de game over y guarda las estadisticas
static void gameOver(userData_t* userData, display_t* displayData);
//inicializa la estructura cardinal
static void initCardinalData(userData_t* userData);
//calcula el cuadrante mas habitado
static void mostPopulatedCardinal(userData_t* userData);
//Callback de comparacion usado para conseguir el most populated cardinal
static int comparfun(const void * a, const void * b);


typedef struct cuadrants
{
	unsigned cuadrant;		//del 1 al 9 (1 = noroeste, 2 = norte, 3 = noreste, 4 = oeste, 5 = centro, 6 = este, 7 = sudoeste, 8 = sur, 9 = sureste)
	unsigned newBorns;		//cantidad de celulas totales por cuadrante
} cuadrants_t;



fsmCell_t** createFSM(void)
{
	unsigned i, j;
	fsmCell_t** FSMMatrix = (fsmCell_t**) malloc(sizeof (fsmCell_t*)*(STATECOUNT));		//malloc de tamaño de filas (ESTADOS)

	for (i = 0; i < (STATECOUNT); i++)
	{
		FSMMatrix[i] = (fsmCell_t*) malloc(sizeof (fsmCell_t)*(EVENTCOUNT));		//arreglo de malloc, para facilitar desrreferencia
	}
        
        fsmCell_t FSMTempMatrix[STATECOUNT][EVENTCOUNT] = {		//matriz fsm temporal

	//PLAY                          PAUSE                           FASTER                          SLOWER                          SINGLE                                  SETTINGS                        HOME                     CREDITS                         QUIT                            HELP                            MAP                             R1_UP                           R1_DOWN                         R2_UP                           R2_DOWN                         R3_UP                           R3_DOWN                         R4C_UP                          R4C_DOWN                        R4R_UP                          R4R_DOWN                        R5UP                            R5_DOWN                         TIMEUP                          TIMEDOWN                        NOEVENT                         GAMEOVER
	{{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{SINGLESETTING, settingSinglePlaying},	{SETTINGS, showSettings},       {HOME, doNothing},	{CREDITS, showCredits},		{QUIT, quit},			{HELP, help},			{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing},		{HOME, doNothing}},		//MENU	
	{{SETTINGS, doNothing},		{SETTINGS, doNothing},		{SETTINGS, doNothing},		{SETTINGS, doNothing},		{SETTINGS, doNothing},			{SETTINGS, doNothing},          {HOME, showHome},	{SETTINGS, doNothing},		{SETTINGS, doNothing},		{SETTINGS, doNothing},		{SETTINGS, doNothing},		{SETTINGS, r1Up},		{SETTINGS, r1Down},		{SETTINGS, r2Up},		{SETTINGS, r2Down},		{SETTINGS, r3Up},		{SETTINGS, r3Down},		{SETTINGS, r5Up},		{SETTINGS, r5Down},		{SETTINGS, r4rUp},		{SETTINGS, r4rDown},		{SETTINGS, r4cUp},		{SETTINGS, r4cDown},		{SETTINGS, timeUp},		{SETTINGS, timeDown},		{SETTINGS, doNothing},		{SETTINGS, doNothing}}, 	//SETTINGS	
	{{SINGLEPLAYING, doNothing},	{PAUSEGAME, pauseGame},		{SINGLEPLAYING, goFaster},	{SINGLEPLAYING, goSlower},	{SINGLEPLAYING, doNothing},		{SINGLEPLAYING, doNothing},     {HOME, showHome},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, doNothing},	{SINGLEPLAYING, update},	{HOME, gameOver}}, 		//SINGLEPLAYING	
	{{SINGLEPLAYING, resumeGame},	{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},			{PAUSEGAME, doNothing},		{HOME, showHome},	{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing},		{PAUSEGAME, doNothing}}, 	//PAUSEGAME	
	{{SINGLEPLAYING, update},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},		{SINGLESETTING, doNothing},	{HOME, showHome},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, setCell},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing},	{SINGLESETTING, doNothing}}, 	//SINGLESETTING
	{{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},			{CREDITS, doNothing},		{HOME, showHome},	{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing},		{CREDITS, doNothing}}, 		//CREDITS	
	{{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},			{HELP, doNothing},		{HOME, showHome},	{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing},		{HELP, doNothing}} 		//HELP
};
        for (i = 0; i < STATECOUNT; i++)	 //Inicializamos la FSM permanente
        {   
		for (j = 0; j < EVENTCOUNT; j++)
		{
			FSMMatrix[i][j] = FSMTempMatrix[i][j];
		}
        }
	return FSMMatrix;
}

void deinitFSMMatrix(fsmCell_t** FSMMatrix)
{
       
	unsigned i;
	if (FSMMatrix != NULL)							//liberamos punteros 
	{
		for (i = 0; i < STATECOUNT; i++)
		{
			free(FSMMatrix[i]);
		}
		free(FSMMatrix);
	}
}

void FSMCycle(fsmCell_t** FSMMatrix, userData_t* userData, display_t* displayData)			//avanza la fsm
{
	FSMMatrix[userData->state][userData->event].funcion(userData, displayData);
	userData->state = FSMMatrix[userData->state][userData->event].nxtstate;
}

void pauseGame(userData_t* userData, display_t* displayData)		
{
    displayData->resumeTimerCount = al_get_timer_count(displayData->timer);
    al_stop_timer(displayData->timer);
}

void goSlower(userData_t* userData, display_t* displayData) //Disminuye tiempo entre generaciones
{
	userData->timeLapse += 10;
}

void goFaster(userData_t* userData, display_t* displayData) //Aumenta tiempo entre generaciones
{
	if ((userData->timeLapse -= 10) > 10);
	else userData->timeLapse = 10;
}

void defaultSettings(userData_t* userData)
{
	srand(time(NULL));							//parametros por default de userData
	userData->currentGen = NULL;
	userData->nextGen = NULL;
	userData->currentScreen = HOMESCREEN;
	userData->inputCells = 10; 
	userData->buttonClicked = NOBUTTON;
	userData->event = NOEVENT;
	userData->state = HOME;
	userData->currentScore.score = 0;
	userData->currentScore.generation = 0;
	userData->columns = 10; 
	userData->rows = 10; 
	userData->isolationNumber = 3;
	userData->revivalNumber = 3;
	userData->suffocationNumber = 5;
	userData->gameTime = 600;
	userData->timeLapse = 30;
}



void settingSinglePlaying(userData_t* userData, display_t* displayData)
{
	userData->currentScreen = PLAYINGSCREEN;
	userData->currentGen = (cell_t**) initCellMatrix(userData->rows, userData->columns);		//reserva memoria para las matrices
	userData->nextGen = (cell_t**) initCellMatrix(userData->rows, userData->columns);
        userData->cellsNumber = userData->inputCells;
	initWorld(userData->rows, userData->columns, displayData);
	initCardinalData(userData);					//inicializa variables de la estructura
	showSingleDisplay(userData, displayData);	//muestra en pantalla la partida
}

void initCardinalData(userData_t* userData)
{
	userData->cardinalData.Ncells = 0;				
	userData->cardinalData.Ecells = 0;
	userData->cardinalData.Wcells = 0;
	userData->cardinalData.Scells = 0;
	userData->cardinalData.Ccells = 0;
	userData->cardinalData.NEcells = 0;
	userData->cardinalData.NWcells = 0;
	userData->cardinalData.SWcells = 0;
	userData->cardinalData.SEcells = 0;
	switch (userData->columns)					//dividimos el mapa en 9 cuadrantes, hay dos paralelos, y dos meridianos; inicializamos éstas
	{
		case 1:									//si hay una columna, ambos meridianos son 0
			userData->cardinalData.firstMeridian = 0;
			userData->cardinalData.secondMeridian = 0;
			break;
		case 2:									//en el caso en el que haya dos columnas, el primer meridiano es 1 y el segundo es 0
			userData->cardinalData.firstMeridian = 1;
			userData->cardinalData.secondMeridian = 0;
			break;
		default:								//cae en default cuando las columnas son tres o mas, entonces las dividimos en tres
			userData->cardinalData.firstMeridian = userData->columns / 3;
			userData->cardinalData.secondMeridian = (userData->columns / 3)*2;
			if ((userData->columns % 3) == 1)			//en el caso en que el resto es uno, o dos esas columnas las sumamos a la parte central
			{
				userData->cardinalData.secondMeridian += 1;
			}
			if ((userData->columns % 3) == 2)
			{
				userData->cardinalData.secondMeridian += 1;
				userData->cardinalData.firstMeridian -= 1;
			}
			break;
	}

	switch (userData->rows)					//el analisis para las filas es analogo al de las columnas
	{
		case 1:
			userData->cardinalData.firstParallel = 0;
			userData->cardinalData.secondParallel = 0;
			break;
		case 2:
			userData->cardinalData.firstParallel = 1;
			userData->cardinalData.secondParallel = 0;
			break;
		default:
			userData->cardinalData.firstParallel = userData->columns / 3;
			userData->cardinalData.secondParallel = (userData->columns / 3)*2;
			if ((userData->columns % 3) == 1)
			{
				userData->cardinalData.secondParallel += 1;
			}
			if ((userData->columns % 3) == 2)
			{
				userData->cardinalData.secondParallel += 1;
				userData->cardinalData.firstParallel -= 1;
			}
			break;
	}
}

void setCell(userData_t* userData, display_t* displayData)
{
	double x = 0, y = 0;
	unsigned int i, j;
	unsigned cellsLeft = userData->cellsNumber;

	x = userData->mouseX - displayData->worldData->initialX;				//avanzamos a la posicion de la celula

	for (j = 0; !((x >= j * displayData->worldData->tileWidth) && (x < (j + 1) * displayData->worldData->tileWidth)); j++); //Obtenemos el numero de columna donde se quiere poner/sacar la celula

	y = userData->mouseY - displayData->worldData->initialY;

	for (i = 0; !((y >= i * displayData->worldData->tileHeight) && (y < (i + 1) * displayData->worldData->tileHeight)); i++); //Obtenemos el numero de fila donde se quiere poner/sacar la celula

	if ((userData->cellsNumber != 0) || (getCellState(userData->currentGen, i + 1, j + 1) == ALIVE))
	{
		toggleCell(userData->currentGen, i + 1, j + 1, &cellsLeft); 		//pasamos fila+1, columna+1, porque tenemos en cuenta borde falso
		userData->cellsNumber = cellsLeft;									//actualizamos la cantidad de celulas restantes para ingresar
		updateGraphics(userData->currentGen, displayData, userData->currentScore.score, userData->currentScore.generation, userData->cellsNumber);
	}

}

void doNothing(userData_t* userData, display_t* diplayData)
{
}

void showSingleDisplay(userData_t* userData, display_t* displayData)
{
	updateGraphics(userData->currentGen, displayData, userData->currentScore.score, userData->currentScore.generation, userData->cellsNumber);
}

void update(userData_t* userData, display_t* displayData)
{
	cell_t** temp = NULL;
        unsigned newBorns = 0;
	if (displayData->gameTimeCount == 0)						//actualizamos timer
	{
		displayData->gameTimeCount = al_get_timer_count(displayData->timer);
	}
	if ((al_get_timer_count(displayData->timer) - displayData->timeLapseCount) > userData->timeLapse)		//si no se acabo el tiempo
	{
		killEdges(userData->rows, userData->columns, userData->currentGen);				//borramos bordes
		newBorns = updateStates(userData->currentGen, userData->nextGen, userData->rows, userData->columns, userData->suffocationNumber, userData->isolationNumber, userData->revivalNumber, &userData->cardinalData); //actualizamos estados de las celulas
                userData->currentScore.score += newBorns;		
		userData->gameover = checkNoChange(userData->currentGen, userData->nextGen, userData->rows, userData->columns);	//chequeamos si dos generaciones son iguales
                if((newBorns != 0)||(userData->currentScore.generation == 0))
                    userData->currentScore.generation++;					//update de la cantidad de generaciones
		else
                    userData->gameover = TRUE;
                
                displayData->timeLapseCount = al_get_timer_count(displayData->timer);	
		temp = userData->nextGen;								//swapeamos las matrices
		userData->nextGen = userData->currentGen;
		userData->currentGen = temp;
		updateGraphics(userData->currentGen, displayData, userData->currentScore.score, userData->currentScore.generation, userData->cellsNumber);
	}
}

void resumeGame(userData_t* userData, display_t* displayData)
{
    al_start_timer(displayData->timer);                                         //Resumes Timer
    al_set_timer_count(displayData->timer, displayData->resumeTimerCount);
    update(userData, displayData);
}
void showSettings(userData_t* userData, display_t* displayData)
{
	userData->currentScreen = SETTINGSSCREEN;
	settingsDisplay(displayData, userData->suffocationNumber, userData->isolationNumber, userData->revivalNumber, userData->rows, userData->columns, userData->inputCells, userData->gameTime);
}

void showCredits(userData_t* userData, display_t* displayData)
{
	userData->currentScreen = CREDITSSCREEN;
	creditsDisplay(displayData);
}

void showHome(userData_t* userData, display_t* displayData)
{
        al_start_timer(displayData->timer);
	userData->currentScreen = HOMESCREEN;
	userData->currentGen = deinitCellMatrix(userData->currentGen, userData->rows);			//reseteamos variables
	userData->nextGen = deinitCellMatrix(userData->nextGen, userData->rows);
	userData->currentScore.generation = 0;
	userData->currentScore.score = 0;
	menuDisplay(displayData);
}


//regla 1: SOFOCACION. cantidad de celulas adyacentes a partir de las cuales muere
void r1Up(userData_t* userData, display_t* displayData)
{
	if ((userData->suffocationNumber < 8) && (userData->suffocationNumber >= userData->isolationNumber))
		userData->suffocationNumber++;

	showSettings(userData, displayData);
}

void r1Down(userData_t* userData, display_t* displayData)
{
	if ((userData->suffocationNumber > 0) && (userData->suffocationNumber > userData->isolationNumber))
	{
		userData->suffocationNumber--;
		if ((userData->suffocationNumber + 1) == userData->revivalNumber)		//debemos ajustar las demas reglas (isolation y revival) para que tengan sentido entre si
			userData->revivalNumber = userData->suffocationNumber;
	}
	if (userData->suffocationNumber == 0)
	{
		userData->revivalNumber = 0;
		userData->isolationNumber = 0;
	}
	showSettings(userData, displayData);
}

//regla 2: SOLEDAD. debajo de esta cantidad de celulas adyacentes muere
void r2Up(userData_t* userData, display_t* displayData)
{
	if ((userData->isolationNumber < 8) && (userData->isolationNumber < userData->suffocationNumber))
	{
		userData->isolationNumber++;					//debemos ajustar las demas reglas (isolation y revival) para que tengan sentido entre si
		if ((userData->isolationNumber - 1) == userData->revivalNumber)
			userData->revivalNumber = userData->isolationNumber;
	}
	if (userData->isolationNumber == 8)
	{
		userData->revivalNumber = 8;
		userData->suffocationNumber = 8;
	}

	showSettings(userData, displayData);
}

void r2Down(userData_t* userData, display_t* displayData)
{
	if ((userData->isolationNumber > 0)&&(userData->isolationNumber <= userData->suffocationNumber))
		userData->isolationNumber--;

	showSettings(userData, displayData);
}

//regla 3: RESURRECCION. cantidad de celulas adyacentes con las que revive
void r3Up(userData_t* userData, display_t* displayData)
{
	if ((userData->revivalNumber < 8)&&(userData->revivalNumber < userData->suffocationNumber)	
			&&(userData->revivalNumber >= userData->isolationNumber))
		userData->revivalNumber++;
	if (userData->isolationNumber == 8)				//debemos ajustar las demas reglas (isolation y revival) para que tengan sentido entre si
		userData->revivalNumber = 8;
	showSettings(userData, displayData);
}

void r3Down(userData_t* userData, display_t* displayData)
{
	if ((userData->revivalNumber > 0)&&(userData->revivalNumber <= userData->suffocationNumber)
			&&(userData->revivalNumber > userData->isolationNumber))
		userData->revivalNumber--;
	if (userData->isolationNumber == 8)				//debemos ajustar las demas reglas (isolation y revival) para que tengan sentido entre si
		userData->revivalNumber = 8;
	showSettings(userData, displayData);
}

//regla 4: cantidad de celulas iniciales
void r5Up(userData_t* userData, display_t* displayData)
{
	if (userData->inputCells < MAXCELLS)
		userData->inputCells++;
	showSettings(userData, displayData);
}

void r5Down(userData_t* userData, display_t* displayData)
{
	if (userData->inputCells > 1)
		userData->inputCells--;
	showSettings(userData, displayData);
}

//regla 4 bis: cantidad de filas
void r4rUp(userData_t* userData, display_t* displayData)
{
	if (userData->rows < MAXROWS)
		userData->rows++;
	showSettings(userData, displayData);
}

void r4rDown(userData_t* userData, display_t* displayData)
{
	if (userData->rows > 1)
		userData->rows--;
	showSettings(userData, displayData);
}

//regla 5: cantidad de columnas
void r4cUp(userData_t* userData, display_t* displayData)
{
	if (userData->columns < MAXCOLUMNS)
		userData->columns++;
	showSettings(userData, displayData);
}

void r4cDown(userData_t* userData, display_t* displayData)
{
	if (userData->columns > 1)
		userData->columns--;
	showSettings(userData, displayData);
}

//Cantidad de tiempo de juego
void timeUp(userData_t* userData, display_t* displayData)
{
	userData->gameTime += 300;
	showSettings(userData, displayData);
}

void timeDown(userData_t* userData, display_t* displayData)
{
	if ((userData->gameTime -= 300) > 300);
	else
		userData->gameTime = 300;
	showSettings(userData, displayData);
}

void quit(userData_t* userData, display_t* displayData)
{
    displayDeInit(displayData);
    userData->currentGen = deinitCellMatrix(userData->currentGen, userData->rows);
    userData->nextGen = deinitCellMatrix(userData->nextGen, userData->rows);
}

void help(userData_t* userData, display_t* displayData)
{
	userData->currentScreen = HELPSCREEN;
	helpDisplay(displayData);
}

void gameOver(userData_t* userData, display_t* displayData)
{
	gameOverDisplay(displayData);				//muestra cuadro de experimento finalizado
	al_rest(2.5);
	userData->currentScore.ratio = userData->currentScore.score / userData->currentScore.generation;
	mostPopulatedCardinal(userData);			//calcula el cuadrante mas poblado
	time_t rawTime;								
	time(&rawTime);
	userData->currentScore.dateInfo = localtime(&rawTime);		//cargamos la fecha y hora 
	userData->currentScore.revivalNumber = userData->revivalNumber;
        userData->currentScore.isolationNumber = userData->isolationNumber;
        userData->currentScore.suffocationNumber = userData->suffocationNumber;
        userData->currentScore.initialCells = (userData->inputCells - userData->cellsNumber);
        userData->currentScore.rows = userData->rows;
        userData->currentScore.columns = userData->columns;
        logbookEntry(&userData->currentScore);						//escribimos las estadisticas en un .txt
	userData->currentScreen = HOMESCREEN;
	showHome(userData, displayData);				
}

void mostPopulatedCardinal(userData_t* userData)
{
	cuadrants_t array[9];
	array[0].cuadrant = NORTH_WEST;			
	array[1].cuadrant = NORTH;			
	array[2].cuadrant = NORTH_EAST;			
	array[3].cuadrant = WEST;			
	array[4].cuadrant = CENTER;			
	array[5].cuadrant = EAST;			
	array[6].cuadrant = SOUTH_WEST;			
	array[7].cuadrant = SOUTH;			
	array[8].cuadrant = SOUTH_EAST;			
	array[0].newBorns = userData->cardinalData.NWcells;
	array[1].newBorns = userData->cardinalData.Ncells;
	array[2].newBorns = userData->cardinalData.NEcells;
	array[3].newBorns = userData->cardinalData.Wcells;
	array[4].newBorns = userData->cardinalData.Ccells;
	array[5].newBorns = userData->cardinalData.Ecells;
	array[6].newBorns = userData->cardinalData.SWcells;
	array[7].newBorns = userData->cardinalData.Scells;
	array[8].newBorns = userData->cardinalData.SWcells;

	qsort(array, 9, sizeof (cuadrants_t), comparfun);		//ordenamos de mayor a menor

	if (userData->columns < 3 || userData->rows < 3)		//si tiene menos de 3 columnas o filas, no hay zona mas poblada (por convencion)
		userData->currentScore.cardinal = 0;
	else
		userData->currentScore.cardinal = array[0].cuadrant; //el primer elemento es el mayor
}

int comparfun(const void * a, const void * b)
{
	cuadrants_t* first = (cuadrants_t*) a;
	cuadrants_t* second = (cuadrants_t*) b;
	return (second->newBorns - first->newBorns);
}



