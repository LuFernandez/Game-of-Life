#include "fileHandler.h"


static int comparfun(const void* a, const void* b);

int comparfun(const void* a, const void* b) //compara dos elementos
{
	score_t* orderA = (score_t*) a;
	score_t* orderB = (score_t*) b;

	return ((orderB->score - orderA->score) != 0) ? (orderB->score - orderA->score) : -1;
}

void logbookEntry(score_t* scoreData)
{
	char auxArray[CHARMAX];
	auxArray[0] = '\0';
	strcat(auxArray, asctime(scoreData->dateInfo));
	char* pointer = strchr(auxArray, '\n'); 			//porque la funcion asctime termina con \n
	*pointer = '\0'; 									//reemplazamos el '\n' con un terminador '\0'
	strcat(auxArray, ".txt"); 							//agregamos el .txt

	FILE* fscore = fopen(auxArray, "wb"); 				//creamos un archivo cuyo nombre sea la fecha y hora de la partida


	fprintf(fscore, "\n\t\t\t\t\tLOGBOOK\n\n\n\n"); 	//escribimos en el file los resultados de la partida y sus parametros
	fprintf(fscore, "\tEntry date: %s", asctime(scoreData->dateInfo)); 		//fecha y hora
	fprintf(fscore, "\n\tThe conditions for this experiment are the following:\n"		//parametros de la partida
			"\t-Initial number of cells: %d\n"
                        "\t-Size of the universe: %d X %d\n"
                        "\t-A cell surrounded by more than %d neighbor cells dies.\n"
			"\t-A cell surrounded by less than %d neighbor cells also dies.\n"
			"\t-A dead cell surrounded by %d neighbor cells, is engulfed by the void and brought back to life.\n"
			"\tMEMO: further studies needed to determine the cause of these.\n", scoreData->initialCells, scoreData->rows, scoreData->columns, scoreData->suffocationNumber, scoreData->isolationNumber, scoreData->revivalNumber);
	fprintf(fscore, "\n\tNumber of cells: %d\n", scoreData->score);
	fprintf(fscore, "\n\tNumber of generations: %d\n", scoreData->generation);
	fprintf(fscore, "\n\tBreeding rate: %.2f\n", scoreData->ratio);
	fprintf(fscore, "\n\tRemarks: \n");					

	if (scoreData->score == 0 || scoreData->cardinal == 0);			//zona mas poblada, (ejemplo, norte, noroeste, centro, etc)
	else
	{
		switch (scoreData->cardinal)
		{
			case 1:
				fprintf(fscore, "\tThere was a significant development of cells in the NORTHWESTERN area\n"
						"\twhich lead us to believe this area to be suitable for a biological attack with a virus.\n");
				break;
			case 2:
				fprintf(fscore, "\tThere was a significant development of cells in the NORTHERN area\n"
						"\twhich lead us to believe this area to be suitable for a biological attack with a virus.\n");
				break;
			case 3:
				fprintf(fscore, "\tThere was a significant development of cells in the NORTHEASTERN area\n"
						"\twhich lead us to believe this area to be suitable for a biological attack with a virus.\n");
				break;
			case 4:
				fprintf(fscore, "\tThere was a significant development of cells in the WESTERN area\n"
						"\twhich lead us to believe this area to be suitable for a biological attack with a virus.\n");
				break;
			case 5:
				fprintf(fscore, "\tThe cells appear to have developed conciousness. Inmediate action is needed.\n"
						"\tcells have taken over the CENTER area.\n");
				break;
			case 6:
				fprintf(fscore, "\tThe cells appear to have developed conciousness. Inmediate action is needed.\n"
						"\tcells have taken over the EASTERN area.\n");
				break;
			case 7:
				fprintf(fscore, "\tThe cells appear to have developed conciousness. Inmediate action is needed.\n"
						"\tcells have taken over the SOUTHWESTERN area.\n");
				break;
			case 8:
				fprintf(fscore, "\tThe cells appear to have developed conciousness. Inmediate action is needed.\n"
						"\tCells have taken over the SOUTHERN area.\n");
				break;
			case 9:
				fprintf(fscore, "\tThe cells appear to have developed conciousness. Inmediate action is needed.\n"
						"\tcells have taken over the SOUHTEASTERN area.\n");
				break;
		}
	}

	switch (rand() % 5)				//datos random de la partida
	{
		case 0:
			fprintf(fscore, "\n\tWARNING: Biological accident in the lab. Members of the crew are experiencing hunger for human flesh.\n"
					"\tReinforcements are needed inmediately \t I NNEED HELPD THEY ARE COMAING FOR  MY FLESHP!!!");
			break;
		case 1:
			fprintf(fscore, "\n\tCells have mutated into a mass of titanic proportions. \n"
					"\tThey have migrated to Asia and seem to have developed weapons.\n"
					"\tNuclear response is needed");
			break;
		case 2:
			fprintf(fscore, "\n\tCells appear to have settled in several human bodies \n"
					"\tand might have taken over some of them.\n"
					"\tMemo: questionable political decisions are being made (*coff*Trump*coff*).");
			break;
		case 3:
			fprintf(fscore, "\n\tPossible cure for cancer might have been discovered.\n"
					"\tMemo: Side effects are to be further studied.\n");
			break;
		case 4:
			fprintf(fscore, "\n\tA chemical was dropped on the initial subjects. The virus leaked through the ventilation duct.\n"
					"\tMany of us were infected. Plan 101HB to repopulate Earth needs to be put in motion.\n");
			break;
	}

	fprintf(fscore, "\n\n\n\n\t\t\t\t\tMAP diagram");			//dibujo que muestra el punto cardinal mas poblado, o la ausencia del mismo

	if (scoreData->score == 0 || scoreData->cardinal == 0)
		fprintf(fscore, "\n\t\t\t\t ___ ___ ___\n"
			"\t\t\t\t|___|___|___|            N\n"
			"\t\t\t\t|___|___|___|        W <-|-> E\n"
			"\t\t\t\t|___|___|___|            S\n");
	else
	{
		switch (scoreData->cardinal)
		{
			case 1:
				fprintf(fscore, "\n\t\t\t\t ___ ___ ___\n"
						"\t\t\t\t|_*_|___|___|            N\n"
						"\t\t\t\t|___|___|___|        W <-|-> E\n"
						"\t\t\t\t|___|___|___|            S\n");
				break;

			case 2:
				fprintf(fscore, "\n\t\t\t\t ___ ___ ___\n"
						"\t\t\t\t|___|_*_|___|            N\n"
						"\t\t\t\t|___|___|___|        W <-|-> E\n"
						"\t\t\t\t|___|___|___|            S\n");
				break;

			case 3:
				fprintf(fscore, "\n\t\t\t\t ___ ___ ___\n"
						"\t\t\t\t|___|___|_*_|            N\n"
						"\t\t\t\t|___|___|___|        W <-|-> E\n"
						"\t\t\t\t|___|___|___|            S\n");
				break;

			case 4:
				fprintf(fscore, "\n\t\t\t\t ___ ___ ___\n"
						"\t\t\t\t|___|___|___|            N\n"
						"\t\t\t\t|_*_|___|___|        W <-|-> E\n"
						"\t\t\t\t|___|___|___|            S\n");
				break;

			case 5:
				fprintf(fscore, "\n\t\t\t\t ___ ___ ___\n"
						"\t\t\t\t|___|___|___|            N\n"
						"\t\t\t\t|___|_*_|___|        W <-|-> E\n"
						"\t\t\t\t|___|___|___|            S\n");
				break;

			case 6:
				fprintf(fscore, "\n\t\t\t\t ___ ___ ___\n"
						"\t\t\t\t|___|___|___|            N\n"
						"\t\t\t\t|___|___|_*_|        W <-|-> E\n"
						"\t\t\t\t|___|___|___|            S\n");
				break;

			case 7:
				fprintf(fscore, "\n\t\t\t\t ___ ___ ___\n"
						"\t\t\t\t|___|___|___|            N\n"
						"\t\t\t\t|___|___|___|        W <-|-> E\n"
						"\t\t\t\t|_*_|___|___|            S\n");
				break;

			case 8:
				fprintf(fscore, "\n\t\t\t\t ___ ___ ___\n"
						"\t\t\t\t|___|___|___|             N\n"
						"\t\t\t\t|___|___|___|         W <-|-> E\n"
						"\t\t\t\t|___|_*_|___|             S\n");
				break;

			case 9:
				fprintf(fscore, "\n\t\t\t\t ___ ___ ___\n"
						"\t\t\t\t|___|___|___|            N\n"
						"\t\t\t\t|___|___|___|        W <-|-> E\n"
						"\t\t\t\t|___|___|_*_|            S\n");
				break;
		}
	}

	switch (rand() % 5)						//nombres random
	{
		case 0:
			fprintf(fscore, "\n\n\n\n\n\t\t\t\t\t\t Dr. Gregory Anderson\n");
			break;
		case 1:
			fprintf(fscore, "\n\n\n\n\n\t\t\t\t\t\t Professor Jekyll\n");
			break;
		case 2:
			fprintf(fscore, "\n\n\n\n\n\t\t\t\t\t\t Mr. Hyde\n");
			break;
		case 3:
			fprintf(fscore, "\n\n\n\n\n\t\t\t\t\t\t Dr. Curie\n");
			break;
		case 4:
			fprintf(fscore, "\n\n\n\n\n\t\t\t\t\t\t Victor Frankenstein\n");
			break;
	}

	fclose(fscore);
}





