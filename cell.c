#include "cell.h"

//devuelve la cantidad de celulas adyacentes 
static unsigned int howManyNeighbours(cell_t** cellMatrix, unsigned int x, unsigned int y);
//calcula a que cardinal pertenece la celula
static void calculateCardinal(unsigned i, unsigned j, unsigned rows, unsigned columns, cardinal_t* cardinalData);


cell_t** initCellMatrix(unsigned rows, unsigned columns)
{
	unsigned i, j;
	cell_t** cellMatrix = (cell_t**) malloc(sizeof (cell_t*)*(rows + 2));		//malloc de tamaño de filas mas los bordes

	for (i = 0; i < (rows + 2); i++)
	{
		cellMatrix[i] = (cell_t*) malloc(sizeof (cell_t)*(columns + 2));		//arreglo de malloc, para facil acceso
	}

	for (i = 0; i < (rows + 2); i++)				//las inicializamos como muertas, y cada posicion como inhabitada
		for (j = 0; j < (columns + 2); j++)
		{
			cellMatrix[i][j].state = DEAD;
			cellMatrix[i][j].inhabited = FALSE;
		}

	return cellMatrix;
}

cell_t** deinitCellMatrix(cell_t** cellMatrix, unsigned rows)
{
	unsigned i;
	if (cellMatrix != NULL)							//liberamos punteros 
	{
		for (i = 0; i < (rows + 2); i++)
		{
			free(cellMatrix[i]);
		}
		free(cellMatrix);
	}
        return NULL;
}

unsigned int howManyNeighbours(cell_t** cellMatrix, unsigned int x, unsigned int y) 
{
	unsigned i = 0, j = 0, aliveNeighbours = 0;

	for (i = (x - 1); i <= (x + 1); i++)	
	{
		for (j = (y - 1); j <= (y + 1); j++)		//chequea posiciones adyacentes en busca de celulas vivas y aumenta el contador
		{
			if (i == x && j == y)
				continue;
			else if (cellMatrix[i][j].state == ALIVE)
				aliveNeighbours++;
		}
	}

	return aliveNeighbours;
}

void killEdges(unsigned rows, unsigned columns, cell_t** cellMatrix)
{
	unsigned i, j;

	for (i = 0; i < (rows + 2); i++)			
	{
		for (j = 0; j < (columns + 2); j++)
		{
			if (i == 0 || i == (rows + 1) || j == 0 || j == (columns + 1))
				cellMatrix[i][j].state = DEAD;									//borramos los bordes 
			else
				continue;
		}
	}
}

unsigned updateStates(cell_t** currentGen, cell_t** nextGen, unsigned rows, unsigned columns, unsigned suffocationNumber, unsigned isolationNumber, unsigned revivalNumber, cardinal_t* cardinalData)
{
	unsigned i = 0, j = 0, aliveNeighbours = 0, newBorns = 0;
	for (i = 1; i <= rows; i++)
	{
		for (j = 1; j <= columns; j++)
		{
			aliveNeighbours = howManyNeighbours(currentGen, i, j);			//actualizamos el estado de cada celula de acuerdo a las reglas
			if (aliveNeighbours > suffocationNumber)
				nextGen[i][j].state = DEAD;
			else if (aliveNeighbours < isolationNumber)
				nextGen[i][j].state = DEAD;
			else if (aliveNeighbours == revivalNumber)
			{
				nextGen[i][j].state = ALIVE;
				nextGen[i][j].inhabited = TRUE;								//esa posicion fue habitada aunque sea una vez (para las estadisticas)
				calculateCardinal(i, j, rows, columns, cardinalData);		//nos fijamos a que cardinal pertenece la celula viva nueva
				if(currentGen[i][j].state == DEAD)
                                    newBorns++;
			} 
			else
				nextGen[i][j] = currentGen[i][j];
		}
	}
	return newBorns;
}

void calculateCardinal(unsigned i, unsigned j, unsigned rows, unsigned columns, cardinal_t* cardinalData)
{
	if ((rows < 3) || (columns < 3)); 						//por convencion, en este caso no se puede calcular la zona mas poblada
	else if (i < cardinalData->firstParallel)				//nos vamos fijando a cual de los 9 cuadrantes pertenece la celula
	{
		if (j < cardinalData->firstMeridian)
			cardinalData->NWcells++;
		else if (j > cardinalData->secondMeridian)
			cardinalData->NEcells++;
		else
			cardinalData->Ncells++;
	} 
	else if (i > cardinalData->secondParallel)
	{
		if (j < cardinalData->firstMeridian)
			cardinalData->SWcells++;
		else if (j > cardinalData->secondMeridian)
			cardinalData->SEcells++;
		else
			cardinalData->Scells++;
	} 
	else
	{
		if (j < cardinalData->firstMeridian)
			cardinalData->Wcells++;
		else if (j > cardinalData->secondMeridian)
			cardinalData->Ecells++;
		else
			cardinalData->Ccells++;
	}
}

void toggleCell(cell_t** cellMatrix, unsigned i, unsigned j, unsigned* cellsLeft)
{
	if ((cellMatrix[i][j].state = !cellMatrix[i][j].state) == ALIVE)
	{
		(*cellsLeft)--;							//restamos 1 a la cantidad de celulas restantes que puede ingresar el usuario
		cellMatrix[i][j].inhabited = TRUE;
	} 
	else										//caso contrario, le sumamos 1
		(*cellsLeft)++;
}

cellState_t getCellState(cell_t** cellMatrix, unsigned i, unsigned j)
{
	return cellMatrix[i][j].state;
}

unsigned checkNoChange(cell_t** matrix1, cell_t** matrix2, unsigned rows, unsigned columns)
{
	unsigned i, j, status = SAME;
	for (i = 1; i <= rows; i++)
	{
		for (j = 1; j <= columns; j++)
		{
			if (matrix1[i][j].state != matrix2[i][j].state) 			//si el estado de una celula en la Nueva Gen difiere del de Gen Actual setea status con DIFFERENT (game over)
				status = DIFFERENT; 						
		}
	}
	return status;
}