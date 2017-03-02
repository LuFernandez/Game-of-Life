#include "FSM.h"
#include "display.h"
#include <allegro5/allegro5.h>
#include <allegro5/events.h>
#include <stdio.h>
#include <stdlib.h>


//analiza que boton se clickeo
void checkClick(display_t* display, userData_t* userData, ALLEGRO_EVENT ev, unsigned state);
//reparte eventos
void eventGenerator(userData_t* userData, display_t* displayData);

int main(int argc, char** argv)
{
	display_t *display = displayInit(); //inicializamos la parte grafica
	userData_t userData;
        fsmCell_t** FSMMatrix = createFSM();
	al_start_timer(display->timer);
	display->timeLapseCount = 0;        //Tiempo entre generaciones
	display->gameTimeCount = 0;         //Tiempo total de juego
	defaultSettings(&userData);
	menuDisplay(display);				//mostramos pantalla principal

	ALLEGRO_EVENT preEvent;
	unsigned state = 0;

	while (userData.event != QUIT)		//mientras el usuario no salga del juego
	{
		userData.event = NOEVENT;
		state = al_get_next_event(display->eventQueue, &preEvent);			

		checkClick(display, &userData, preEvent, state);			//analizamos que boton se toco
		eventGenerator(&userData, display);							//generamos el evento
		FSMCycle(FSMMatrix, &userData, display);								//avanzamos la fsm
	}
       deinitFSMMatrix(FSMMatrix);
       
       return EXIT_SUCCESS;
}

void checkClick(display_t* display, userData_t* userData, ALLEGRO_EVENT ev, unsigned state) //chequea si se toco algun boton y devuelve la posicion en el arreglo del boton q se toco
{
	unsigned i;

	if ((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) && state) //Se analizan solo eventos de mouse y solo en el caso de que un evento haya ocurrido			
	{
		for (i = 0; i < BUTTON_COUNT; i++)			//recorre el arreglo de los botones y se fija si se toco dentro del espacio del boton 
		{
			if (display->buttons[i].screen != userData->currentScreen) //Si la pantalla del boton en cuestion no coincide con la pantalla actual de juego no genera evento
				continue;
			else if (((ev.mouse.x >= display->buttons[i].x) && (ev.mouse.x <= (display->buttons[i].x + display->buttons[i].width))) &&
					((ev.mouse.y >= display->buttons[i].y) && (ev.mouse.y <= (display->buttons[i].y + display->buttons[i].height))))
			{
				userData->buttonClicked = i;    //Indice en el arreglo de botones
				userData->mouseX = ev.mouse.x;
				userData->mouseY = ev.mouse.y;
				break;
			}
		}
	} 
	else
		userData->buttonClicked = NOBUTTON; //No se clickeo ningun boton

	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)			//si el usuario quiere salir del juego
		userData->buttonClicked = HOME_EXIT;



}

void eventGenerator(userData_t* userData, display_t* displayData) 		//crea el evento
{
    
	switch (userData->buttonClicked)
	{
		case SP_HOME:							//analiza que boton se clickeo y genera el evento correspondiente a la FSM
		case SET_HOME:
		case HELP_HOME:
		case CREDITS_HOME: userData->event = HOMEBUTTON;
			break;
		case HOME_HELP: userData->event = HELPBUTTON;
			break;
		case HOME_SETTINGS: userData->event = SETTINGSBUTTON;
			break;
		case HOME_CREDITS: userData->event = CREDITSBUTTON;
			break;
		case HOME_EXIT: userData->event = QUIT;
			break;
		case HOME_SP: userData->event = SINGLE;
			break;
		case SP_MAP: userData->event = MAP;
			break;
		case SP_PAUSE: userData->event = PAUSE;
			break;
		case SP_PLAY: userData->event = PLAY;
			break;
		case R1_DOWN: userData->event = RULE1DOWN;
			break;
		case R1_UP: userData->event = RULE1UP;
			break;
		case R2_DOWN: userData->event = RULE2DOWN;
			break;
		case R2_UP: userData->event = RULE2UP;
			break;
		case R3_DOWN: userData->event = RULE3DOWN;
			break;
		case R3_UP: userData->event = RULE3UP;
			break;
		case R4R_DOWN: userData->event = RULE4ROWDOWN;
			break;
		case R4R_UP: userData->event = RULE4ROWUP;
			break;
		case R4C_DOWN: userData->event = RULE4COLDOWN;
			break;
		case R4C_UP: userData->event = RULE4COLUP;
			break;
		case R5_DOWN: userData->event = RULE5DOWN;
			break;
		case R5_UP: userData->event = RULE5UP;
			break;
		case TIME_DOWN: userData->event = TIMEDOWN;
			break;
		case TIME_UP: userData->event = TIMEUP;
			break;
		case SP_FAST: userData->event = FASTER;
			break;
		case SP_SLOW: userData->event = SLOWER;
			break;
		case NOBUTTON: userData->event = NOEVENT;
			break;
		default: userData->event = NOEVENT;
			break;
	};

	if (userData->gameover) 					//Si por determinada razon el juego debe terminar, se genera el evento GAMEOVER
	{
		userData->event = GAMEOVER;
		userData->gameover = 0;
	}
	else if (((al_get_timer_count(displayData->timer) - displayData->gameTimeCount) > userData->gameTime) && userData->state == SINGLEPLAYING)
	{
		displayData->gameTimeCount = 0; 		//Si el tiempo de juego, impuesto por el usuario, se termino, el juego termina
		userData->event = GAMEOVER;
	}
}