#include "display.h"

typedef enum {HOMESCREEN, SETTINGSSCREEN, HELPSCREEN, PLAYINGSCREEN, CREDITSSCREEN, SCREENCOUNT} screen_t;

//carga botones del menu
static void initMenuUtilities(bitmap_t* bitmapArray);
//carga botones de la pantalla de juego Single Player
static void initSinglePlayerUtilities(bitmap_t*);
//carga botones de la pantalla de creditos
static void initCreditsUtilities(bitmap_t* bitmapArray);
//carga botones de la pantalla ayuda
static void initHelpUtilities(bitmap_t* bitmapArray);
//carga botones de la pantalla de configuraciones
static void initSettingsUtilities(bitmap_t* bitmapArray);
//actualiza el numero de score en la pantalla
static void updateScore(display_t* displayData, unsigned score);
//actualiza el numero de generaciones en la pantalla
static void updateGeneration(display_t* displayData, unsigned generation);
//actualiza el numero de celulas restantes que puede ingresar el usuario en la pantalla
static void updateInputCells(display_t* displayData, unsigned inputCells);
//calcula el minimo entre dos elementos
static unsigned int min(unsigned int a, unsigned int b);


display_t* displayInit(void)
{
	display_t* display = malloc(sizeof (display_t));
	if (display != NULL)
	{
		display->display = NULL;				//cargamos datos en la estructura display
		display->eventQueue = NULL;
		display->font = NULL;
		display->titleFont = NULL;
		display->timer = NULL;
		display->background = NULL;
		display->width = DISPLAYW;
		display->height = DISPLAYH;

		if (al_init())
		{
			if (al_init_primitives_addon())
			{
				if (al_init_image_addon())
				{
					al_init_font_addon();
					if (al_init_ttf_addon())
					{
						display->display = al_create_display(display->width, display->height);
						if (display->display != NULL)
						{
							al_set_window_title(display->display, "Game of Life");					//titulo 
							display->background = al_load_bitmap("background.jpg");
							if (display->background != NULL)
							{
								display->backgroundWidth = al_get_bitmap_width(display->background);
								display->backgroundHeight = al_get_bitmap_height(display->background);
								al_draw_scaled_bitmap(display->background, 0, 0, display->backgroundWidth, display->backgroundHeight, 0, 0, display->width, display->height, 0); //dibujo fondo

								display->font = al_load_ttf_font("Bellota-Bold.otf", 20, 0);
								if (display->font != NULL)
								{
									display->timer = al_create_timer(1.0 / 60);
									if (display->timer != NULL)
									{
										if (al_install_mouse())
										{
											if (al_install_keyboard())
											{
												display->eventQueue = al_create_event_queue();
												if (display->eventQueue != NULL)
												{
													al_register_event_source(display->eventQueue, al_get_mouse_event_source());
													al_register_event_source(display->eventQueue, al_get_keyboard_event_source());
													al_register_event_source(display->eventQueue, al_get_display_event_source(display->display));
													display->titleFont = al_load_ttf_font("Bellota-Bold.otf", 80, 0);
													if (display->titleFont != NULL)
													{
														al_draw_bitmap(display->background, 0, 0, 0);
														al_start_timer(display->timer);
													} 
													else
													{
														al_destroy_bitmap(display->background);
														al_destroy_event_queue(display->eventQueue);
														al_destroy_font(display->font);
														al_destroy_timer(display->timer);
														al_destroy_display(display->display);
														al_shutdown_ttf_addon();
														al_shutdown_primitives_addon();
														al_shutdown_image_addon();
														al_shutdown_font_addon();
														al_uninstall_mouse();
														al_uninstall_keyboard();
														free(display);
														display = NULL;
													}
												} 
												else
												{
													al_destroy_bitmap(display->background);
													al_destroy_font(display->font);
													al_destroy_timer(display->timer);
													al_destroy_display(display->display);
													al_shutdown_ttf_addon();
													al_shutdown_primitives_addon();
													al_shutdown_image_addon();
													al_shutdown_font_addon();
													al_uninstall_mouse();
													al_uninstall_keyboard();
													free(display);
													display = NULL;
												}
											} 
											else
											{
												al_destroy_bitmap(display->background);
												al_destroy_font(display->font);
												al_destroy_timer(display->timer);
												al_destroy_display(display->display);
												al_uninstall_mouse();
												al_shutdown_ttf_addon();
												al_shutdown_primitives_addon();
												al_shutdown_image_addon();
												al_shutdown_font_addon();
												free(display);
												display = NULL;
											}
										} 
										else
										{
											al_destroy_bitmap(display->background);
											al_destroy_font(display->font);
											al_destroy_timer(display->timer);
											al_destroy_display(display->display);
											al_shutdown_ttf_addon();
											al_shutdown_primitives_addon();
											al_shutdown_image_addon();
											al_shutdown_font_addon();
											free(display);
											display = NULL;
										}
									} 
									else
									{
										al_destroy_bitmap(display->background);
										al_destroy_font(display->font);
										al_destroy_display(display->display);
										al_shutdown_ttf_addon();
										al_shutdown_primitives_addon();
										al_shutdown_image_addon();
										al_shutdown_font_addon();
										free(display);
										display = NULL;
									}
								} 
								else
								{
									al_destroy_bitmap(display->background);
									al_destroy_display(display->display);
									al_shutdown_ttf_addon();
									al_shutdown_primitives_addon();
									al_shutdown_image_addon();
									al_shutdown_font_addon();
									free(display);
									display = NULL;
								}
							} 
							else
							{
								al_destroy_display(display->display);
								al_shutdown_ttf_addon();
								al_shutdown_primitives_addon();
								al_shutdown_image_addon();
								al_shutdown_font_addon();
								free(display);
								display = NULL;
							}
						} 
						else
						{
							al_shutdown_ttf_addon();
							al_shutdown_primitives_addon();
							al_shutdown_image_addon();
							al_shutdown_font_addon();
							free(display);
							display = NULL;
						}
					} 
					else
					{
						al_shutdown_primitives_addon();
						al_shutdown_image_addon();
						al_shutdown_font_addon();
						free(display);
						display = NULL;
					}
				} 
				else
				{
					al_shutdown_primitives_addon();
					al_shutdown_image_addon();
					free(display);
					display = NULL;
				}
			} 
			else
			{
				al_shutdown_primitives_addon();
				free(display);
				display = NULL;
			}
		} 
		else
		{
			free(display);
			display = NULL;
		}
	} 
	else
	{
		free(display);
		display = NULL;
	}

	display->buttons = (bitmap_t*) malloc(sizeof (bitmap_t) * BUTTON_COUNT);
	initMenuUtilities(display->buttons);
	initSinglePlayerUtilities(display->buttons);
	initCreditsUtilities(display->buttons);
	initHelpUtilities(display->buttons);
	initSettingsUtilities(display->buttons);

	return display;
}

void displayDeInit(display_t* display)
{
        unsigned i;
        for(i=0;i<BUTTON_COUNT;i++)
        {
            al_destroy_bitmap(display->buttons[i].image);
        }

	al_destroy_bitmap(display->background);
	al_destroy_event_queue(display->eventQueue);
	al_destroy_font(display->font);
        al_destroy_font(display->titleFont);
	al_destroy_timer(display->timer);
	al_destroy_display(display->display);
	al_shutdown_ttf_addon();
	al_shutdown_primitives_addon();
	al_shutdown_image_addon();
	al_shutdown_font_addon();
	al_uninstall_mouse();
	free(display);
	display = NULL;
}

unsigned int min(unsigned int a, unsigned int b) 			//calculo el minimo de dos numeros
{
	return a <= b ? a : b;
}

void menuDisplay(display_t * displayData)
{
	al_set_target_backbuffer(displayData->display); 	//Vuelve al display

	al_draw_bitmap(displayData->background, 0, 0, 0); 	//dibujo fondo
	al_draw_bitmap(displayData->buttons[HOME_SP].image, displayData->buttons[HOME_SP].x, displayData->buttons[HOME_SP].y, 0); 	//dibujo boton single player

	al_draw_bitmap(displayData->buttons[HOME_CREDITS].image, displayData->buttons[HOME_CREDITS].x, displayData->buttons[HOME_CREDITS].y, 0); 	//dibujo boton credits
	al_draw_bitmap(displayData->buttons[HOME_HELP].image, displayData->buttons[HOME_HELP].x, displayData->buttons[HOME_HELP].y, 0); 	//dibujo help
	al_draw_bitmap(displayData->buttons[HOME_SETTINGS].image, displayData->buttons[HOME_SETTINGS].x, displayData->buttons[HOME_SETTINGS].y, 0); 	//dibujo settings
	al_draw_bitmap(displayData->buttons[HOME_EXIT].image, displayData->buttons[HOME_EXIT].x, displayData->buttons[HOME_EXIT].y, 0); 	//dibujo exit

	al_draw_text(displayData->titleFont, al_map_rgb(143, 91, 3), 545, 30, ALLEGRO_ALIGN_CENTRE, "GAME OF LIFE");

	al_flip_display();
}

void singlePlayerDisplay(display_t* displayData)
{
	al_draw_scaled_bitmap(displayData->background, 0, 0, displayData->backgroundWidth, displayData->backgroundHeight, 0, 0, displayData->width, displayData->height, 0); //dibujo fondo
	al_set_target_bitmap(displayData->buttons[SP_MAP].image); 			//Dibujo sobre el mapa un marco
	al_draw_rectangle(0, 0, displayData->buttons[SP_MAP].width, displayData->buttons[SP_MAP].height, al_map_rgb(97, 86, 33), 5);
	al_set_target_backbuffer(displayData->display); 					//Vuelve al display
	al_draw_bitmap(displayData->buttons[SP_MAP].image, displayData->buttons[SP_MAP].x, displayData->buttons[SP_MAP].y, 0);
	al_draw_bitmap(displayData->buttons[SP_HOME].image, displayData->buttons[SP_HOME].x, displayData->buttons[SP_HOME].y, 0); 		//home button
	al_draw_bitmap(displayData->buttons[SP_PLAY].image, displayData->buttons[SP_PLAY].x, displayData->buttons[SP_PLAY].y, 0); 		//play button
	al_draw_bitmap(displayData->buttons[SP_PAUSE].image, displayData->buttons[SP_PAUSE].x, displayData->buttons[SP_PAUSE].y, 0); 	//pause button
	al_draw_bitmap(displayData->buttons[SP_FAST].image, displayData->buttons[SP_FAST].x, displayData->buttons[SP_FAST].y, 0);
	al_draw_bitmap(displayData->buttons[SP_SLOW].image, displayData->buttons[SP_SLOW].x, displayData->buttons[SP_SLOW].y, 0);
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), MAPX / 2, 100, ALLEGRO_ALIGN_CENTER, "SCORE");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), MAPX / 2, 200, ALLEGRO_ALIGN_CENTER, "GENERATION");
	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), MAPX / 2, 550, ALLEGRO_ALIGN_CENTER, "Remaining Cells:");
}

void settingsDisplay(display_t* displayData, int sofocation, int isolation, int revival, int rowR4, int columnR4, int aliveCells, int gametime)
{
	al_draw_scaled_bitmap(displayData->background, 0, 0, displayData->backgroundWidth, displayData->backgroundHeight, 0, 0, displayData->width, displayData->height, 0); //dibujo fondo
	al_draw_text(displayData->titleFont, al_map_rgb(143, 91, 3), DISPLAYW / 2, 10, ALLEGRO_ALIGN_CENTER, "Settings");					//escribimos las reglas
	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2, MENU_TOP_ALIGN, ALLEGRO_ALIGN_CENTRE, "One cell with more than   %d   neighbor cells DIES", sofocation);
	al_draw_bitmap(displayData->buttons[R1_DOWN].image, displayData->buttons[R1_DOWN].x, displayData->buttons[R1_DOWN].y, 0);
	al_draw_bitmap(displayData->buttons[R1_UP].image, displayData->buttons[R1_UP].x, displayData->buttons[R1_UP].y, 0);

	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2, MENU_TOP_ALIGN + 175, ALLEGRO_ALIGN_CENTRE, "One cell with less than  %d  neighbor cells DIES", isolation);
	al_draw_bitmap(displayData->buttons[R2_DOWN].image, displayData->buttons[R2_DOWN].x, displayData->buttons[R2_DOWN].y, 0);
	al_draw_bitmap(displayData->buttons[R2_UP].image, displayData->buttons[R2_UP].x, displayData->buttons[R2_UP].y, 0);

	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2, MENU_TOP_ALIGN + 90, ALLEGRO_ALIGN_CENTRE, "One cell with exactly  %d  neighbor cells REVIVES", revival);
	al_draw_bitmap(displayData->buttons[R3_DOWN].image, displayData->buttons[R3_DOWN].x, displayData->buttons[R3_DOWN].y, 0);
	al_draw_bitmap(displayData->buttons[R3_UP].image, displayData->buttons[R3_UP].x, displayData->buttons[R3_UP].y, 0);

	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2 - 100, MENU_TOP_ALIGN + 250, ALLEGRO_ALIGN_CENTRE, "Universe size:");
	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2 + 45, MENU_TOP_ALIGN + 250, ALLEGRO_ALIGN_CENTRE, " %d ", rowR4);
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2 + 75, MENU_TOP_ALIGN + 250, ALLEGRO_ALIGN_CENTRE, " x ");
	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2 + 105, MENU_TOP_ALIGN + 250, ALLEGRO_ALIGN_CENTRE, " %d ", columnR4);
	al_draw_bitmap(displayData->buttons[R4R_DOWN].image, displayData->buttons[R4R_DOWN].x, displayData->buttons[R4R_DOWN].y, 0);
	al_draw_bitmap(displayData->buttons[R4R_UP].image, displayData->buttons[R4R_UP].x, displayData->buttons[R4R_UP].y, 0);
	al_draw_bitmap(displayData->buttons[R4C_DOWN].image, displayData->buttons[R4C_DOWN].x, displayData->buttons[R4C_DOWN].y, 0);
	al_draw_bitmap(displayData->buttons[R4C_UP].image, displayData->buttons[R4C_UP].x, displayData->buttons[R4C_UP].y, 0);

	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2, MENU_TOP_ALIGN + 330, ALLEGRO_ALIGN_CENTRE, "Initial alive cells:");
	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2 + 100, MENU_TOP_ALIGN + 330, ALLEGRO_ALIGN_CENTRE, "  %d  ", aliveCells);
	al_draw_bitmap(displayData->buttons[R5_DOWN].image, displayData->buttons[R5_DOWN].x, displayData->buttons[R5_DOWN].y, 0);
	al_draw_bitmap(displayData->buttons[R5_UP].image, displayData->buttons[R5_UP].x, displayData->buttons[R5_UP].y, 0);

	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2 - 50, MENU_TOP_ALIGN + 410, ALLEGRO_ALIGN_CENTRE, "The duration of the experiment will be:");
	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2 + 170, MENU_TOP_ALIGN + 410, ALLEGRO_ALIGN_CENTRE, "  %d  ", gametime / 60);
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW / 2 + 200, MENU_TOP_ALIGN + 410, ALLEGRO_ALIGN_LEFT, "seconds");
	al_draw_bitmap(displayData->buttons[TIME_DOWN].image, displayData->buttons[TIME_DOWN].x, displayData->buttons[TIME_DOWN].y, 0);
	al_draw_bitmap(displayData->buttons[TIME_UP].image, displayData->buttons[TIME_UP].x, displayData->buttons[TIME_UP].y, 0);
	al_draw_bitmap(displayData->buttons[SET_HOME].image, displayData->buttons[SET_HOME].x, displayData->buttons[SET_HOME].y, 0);

	al_flip_display();
}

void helpDisplay(display_t* displayData)
{
	al_draw_scaled_bitmap(displayData->background, 0, 0, displayData->backgroundWidth, displayData->backgroundHeight, 0, 0, displayData->width, displayData->height, 0); //dibujo fondo

	al_draw_text(displayData->titleFont, al_map_rgb(143, 91, 3), DISPLAYW / 2, 10, ALLEGRO_ALIGN_CENTRE, "Help");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 15, MENU_TOP_ALIGN - 50, ALLEGRO_ALIGN_LEFT, "This is The Game of Life, where you can create your own cells and watch them grow, reproduce and DIE...or not.");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 15, MENU_TOP_ALIGN - 10, ALLEGRO_ALIGN_LEFT, "Your cells will survive or not according to a set of rules that you can determine,");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 15, MENU_TOP_ALIGN + 20, ALLEGRO_ALIGN_LEFT, "you have all the power my friend, use it wisely.");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 15, MENU_TOP_ALIGN + 60, ALLEGRO_ALIGN_LEFT, "If a cell is surrounded by too many neighbor cells, it runs out of fuel and oxygen, and so it dies.");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 15, MENU_TOP_ALIGN + 100, ALLEGRO_ALIGN_LEFT, "If a cell is surrounded by too few neighbor cells, it falls into a state of deep depression,");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 15, MENU_TOP_ALIGN + 130, ALLEGRO_ALIGN_LEFT, "and ultimately dies as well");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 15, MENU_TOP_ALIGN + 170, ALLEGRO_ALIGN_LEFT, "But if a dead cell is surrounded by a certain number of neighbor cells, quite mysteriously,");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 15, MENU_TOP_ALIGN + 200, ALLEGRO_ALIGN_LEFT, "it comes back from the dead.     (MEMO: reasons for this should be further studied)");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 15, MENU_TOP_ALIGN + 240, ALLEGRO_ALIGN_LEFT, "You will also be able to set the size of the confined space where your cells will develop their lives, ");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 15, MENU_TOP_ALIGN + 270, ALLEGRO_ALIGN_LEFT, "and also the amount of initial alive cells that will first procreate and generate your own cell colony");

	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 15, MENU_TOP_ALIGN + 310, ALLEGRO_ALIGN_LEFT, "Go visit our friends in the Settings Department where they'll show you how to do all this and more.");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), DISPLAYW - 350, MENU_TOP_ALIGN + 370, ALLEGRO_ALIGN_LEFT, "Good luck, my friend. Good luck.");

	al_draw_bitmap(displayData->buttons[HELP_HOME].image, displayData->buttons[HELP_HOME].x, displayData->buttons[HELP_HOME].y, 0);

	al_flip_display();
}

void creditsDisplay(display_t* displayData)
{
	al_draw_scaled_bitmap(displayData->background, 0, 0, displayData->backgroundWidth, displayData->backgroundHeight, 0, 0, displayData->width, displayData->height, 0); //dibujo fondo

	al_draw_text(displayData->titleFont, al_map_rgb(143, 91, 3), DISPLAYW / 2, 10, ALLEGRO_ALIGN_CENTRE, "CREDITS");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 300, MENU_TOP_ALIGN - 30, ALLEGRO_ALIGN_LEFT, "This game was created by LuciferTheLightOfOurLives&Co.");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 300, MENU_TOP_ALIGN, ALLEGRO_ALIGN_LEFT, "All rights reserved.");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 300, MENU_TOP_ALIGN + 40, ALLEGRO_ALIGN_LEFT, "We'd like to thank our friends and family, and Pepe, always en el cora.");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 300, MENU_TOP_ALIGN + 70, ALLEGRO_ALIGN_LEFT, "We can't stress this enough. Pepe always en el cora.");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 300, MENU_TOP_ALIGN + 100, ALLEGRO_ALIGN_LEFT, "We mean it Pepe, if you are out there somewhere. En el cora.");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 300, MENU_TOP_ALIGN + 150, ALLEGRO_ALIGN_LEFT, "The members of this amazing crew are:");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 350, MENU_TOP_ALIGN + 190, ALLEGRO_ALIGN_LEFT, "Juan Bautista Arenaza Manzo");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 350, MENU_TOP_ALIGN + 220, ALLEGRO_ALIGN_LEFT, "Benjamín Carlos Lin");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 350, MENU_TOP_ALIGN + 250, ALLEGRO_ALIGN_LEFT, "Lucifer Guadalupe Fernandez");
	al_draw_text(displayData->font, al_map_rgb(0, 0, 0), 350, MENU_TOP_ALIGN + 280, ALLEGRO_ALIGN_LEFT, "Roberto Chá");

	al_draw_bitmap(displayData->buttons[CREDITS_HOME].image, displayData->buttons[CREDITS_HOME].x, displayData->buttons[CREDITS_HOME].y, 0);

	al_flip_display();
}

void initCreditsUtilities(bitmap_t* bitmapArray)
{
	bitmapArray[CREDITS_HOME].image = al_load_bitmap("home.png");			//boton para volver a home en los creditos
	bitmapArray[CREDITS_HOME].screen = CREDITSSCREEN;

	bitmapArray[CREDITS_HOME].height = al_get_bitmap_height(bitmapArray[CREDITS_HOME].image);
	bitmapArray[CREDITS_HOME].width = al_get_bitmap_width(bitmapArray[CREDITS_HOME].image);

	bitmapArray[CREDITS_HOME].x = MENU_EXIT_ALIGN_X;
	bitmapArray[CREDITS_HOME].y = MENU_EXIT_ALIGN_Y;
}

void initHelpUtilities(bitmap_t* bitmapArray)
{
	bitmapArray[HELP_HOME].image = al_load_bitmap("home.png");				//boton para volver a home en ayuda
	bitmapArray[HELP_HOME].screen = HELPSCREEN;

	bitmapArray[HELP_HOME].height = al_get_bitmap_height(bitmapArray[HELP_HOME].image);
	bitmapArray[HELP_HOME].width = al_get_bitmap_width(bitmapArray[HELP_HOME].image);

	bitmapArray[HELP_HOME].x = MENU_EXIT_ALIGN_X;
	bitmapArray[HELP_HOME].y = MENU_EXIT_ALIGN_Y + 30;
}

void initSettingsUtilities(bitmap_t* bitmapArray)
{
	bitmapArray[R1_UP].image = al_load_bitmap("up.png");					//boton up para la regla 1
	bitmapArray[R1_UP].screen = SETTINGSSCREEN;
	bitmapArray[R1_UP].height = al_get_bitmap_height(bitmapArray[R1_UP].image);
	bitmapArray[R1_UP].width = al_get_bitmap_width(bitmapArray[R1_UP].image);
	bitmapArray[R1_UP].x = LEFT_ALIGN + BUTTONW + 495;
	bitmapArray[R1_UP].y = MENU_TOP_ALIGN - 28;

	bitmapArray[R1_DOWN].image = al_load_bitmap("down.png"); 				//boton down para la regla 1
	bitmapArray[R1_DOWN].screen = SETTINGSSCREEN;
	bitmapArray[R1_DOWN].height = al_get_bitmap_height(bitmapArray[R1_DOWN].image);
	bitmapArray[R1_DOWN].width = al_get_bitmap_width(bitmapArray[R1_DOWN].image);
	bitmapArray[R1_DOWN].x = LEFT_ALIGN + BUTTONW + 495;
	bitmapArray[R1_DOWN].y = MENU_TOP_ALIGN + 20;

	bitmapArray[R2_UP].image = al_load_bitmap("up.png");					//boton up para la regla 2
	bitmapArray[R2_UP].screen = SETTINGSSCREEN;
	bitmapArray[R2_UP].height = al_get_bitmap_height(bitmapArray[R2_UP].image);
	bitmapArray[R2_UP].width = al_get_bitmap_width(bitmapArray[R2_UP].image);
	bitmapArray[R2_UP].x = LEFT_ALIGN + BUTTONW + 490; 
	bitmapArray[R2_UP].y = MENU_TOP_ALIGN + 147; 

	bitmapArray[R2_DOWN].image = al_load_bitmap("down.png");				//boton down para la regla 2
	bitmapArray[R2_DOWN].screen = SETTINGSSCREEN;
	bitmapArray[R2_DOWN].height = al_get_bitmap_height(bitmapArray[R2_DOWN].image);
	bitmapArray[R2_DOWN].width = al_get_bitmap_width(bitmapArray[R2_DOWN].image);
	bitmapArray[R2_DOWN].x = LEFT_ALIGN + BUTTONW + 490; 
	bitmapArray[R2_DOWN].y = MENU_TOP_ALIGN + 195; 

	bitmapArray[R3_UP].image = al_load_bitmap("up.png");					//boton up para la regla 3
	bitmapArray[R3_UP].screen = SETTINGSSCREEN;
	bitmapArray[R3_UP].height = al_get_bitmap_height(bitmapArray[R3_UP].image);
	bitmapArray[R3_UP].width = al_get_bitmap_width(bitmapArray[R3_UP].image);
	bitmapArray[R3_UP].x = LEFT_ALIGN + BUTTONW + 462; 
	bitmapArray[R3_UP].y = MENU_TOP_ALIGN + 62; 

	bitmapArray[R3_DOWN].image = al_load_bitmap("down.png");				//boton down para la regla 3
	bitmapArray[R3_DOWN].screen = SETTINGSSCREEN;
	bitmapArray[R3_DOWN].height = al_get_bitmap_height(bitmapArray[R3_DOWN].image);
	bitmapArray[R3_DOWN].width = al_get_bitmap_width(bitmapArray[R3_DOWN].image);
	bitmapArray[R3_DOWN].x = LEFT_ALIGN + BUTTONW + 462; 
	bitmapArray[R3_DOWN].y = MENU_TOP_ALIGN + 110; 

	bitmapArray[R4C_UP].image = al_load_bitmap("up.png"); 					//boton up para la regla 4, para las columnas
	bitmapArray[R4C_UP].screen = SETTINGSSCREEN;
	bitmapArray[R4C_UP].height = al_get_bitmap_height(bitmapArray[R4C_UP].image);
	bitmapArray[R4C_UP].width = al_get_bitmap_width(bitmapArray[R4C_UP].image);
	bitmapArray[R4C_UP].x = LEFT_ALIGN + BUTTONW + 555 + 15; 
	bitmapArray[R4C_UP].y = MENU_TOP_ALIGN + 302; 

	bitmapArray[R4C_DOWN].image = al_load_bitmap("down.png");				//boton down para la regla 4, para las columnas
	bitmapArray[R4C_DOWN].screen = SETTINGSSCREEN;
	bitmapArray[R4C_DOWN].height = al_get_bitmap_height(bitmapArray[R4C_DOWN].image);
	bitmapArray[R4C_DOWN].width = al_get_bitmap_width(bitmapArray[R4C_DOWN].image);
	bitmapArray[R4C_DOWN].x = LEFT_ALIGN + BUTTONW + 555 + 15; 
	bitmapArray[R4C_DOWN].y = MENU_TOP_ALIGN + 350; 

	bitmapArray[R4R_UP].image = al_load_bitmap("up.png");					//boton up para la regla 4, para las filas
	bitmapArray[R4R_UP].screen = SETTINGSSCREEN;
	bitmapArray[R4R_UP].height = al_get_bitmap_height(bitmapArray[R4R_UP].image);
	bitmapArray[R4R_UP].width = al_get_bitmap_width(bitmapArray[R4R_UP].image);
	bitmapArray[R4R_UP].x = LEFT_ALIGN + BUTTONW + 515;
	bitmapArray[R4R_UP].y = MENU_TOP_ALIGN + 222;

	bitmapArray[R4R_DOWN].image = al_load_bitmap("down.png");				//boton down para la regla 4, para las filas
	bitmapArray[R4R_DOWN].screen = SETTINGSSCREEN;
	bitmapArray[R4R_DOWN].height = al_get_bitmap_height(bitmapArray[R4R_DOWN].image);
	bitmapArray[R4R_DOWN].width = al_get_bitmap_width(bitmapArray[R4R_DOWN].image);
	bitmapArray[R4R_DOWN].x = LEFT_ALIGN + BUTTONW + 515;
	bitmapArray[R4R_DOWN].y = MENU_TOP_ALIGN + 270;

	bitmapArray[R5_UP].image = al_load_bitmap("up.png");					//boton up para la regla 5
	bitmapArray[R5_UP].screen = SETTINGSSCREEN;
	bitmapArray[R5_UP].height = al_get_bitmap_height(bitmapArray[R4R_UP].image);
	bitmapArray[R5_UP].width = al_get_bitmap_width(bitmapArray[R4R_UP].image);
	bitmapArray[R5_UP].x = LEFT_ALIGN + BUTTONW + 573 + 3; 
	bitmapArray[R5_UP].y = MENU_TOP_ALIGN + 222; 

	bitmapArray[R5_DOWN].image = al_load_bitmap("down.png");				//boton down para la regla 5
	bitmapArray[R5_DOWN].screen = SETTINGSSCREEN;
	bitmapArray[R5_DOWN].height = al_get_bitmap_height(bitmapArray[R4R_DOWN].image);
	bitmapArray[R5_DOWN].width = al_get_bitmap_width(bitmapArray[R4R_DOWN].image);
	bitmapArray[R5_DOWN].x = LEFT_ALIGN + BUTTONW + 573 + 3; 
	bitmapArray[R5_DOWN].y = MENU_TOP_ALIGN + 270; 

	bitmapArray[SET_HOME].image = al_load_bitmap("home.png");				//boton home para volver a la pantalla principal
	bitmapArray[SET_HOME].screen = SETTINGSSCREEN;
	bitmapArray[SET_HOME].height = al_get_bitmap_height(bitmapArray[SET_HOME].image);
	bitmapArray[SET_HOME].width = al_get_bitmap_width(bitmapArray[SET_HOME].image);
	bitmapArray[SET_HOME].x = MENU_EXIT_ALIGN_X;
	bitmapArray[SET_HOME].y = MENU_EXIT_ALIGN_Y;

	bitmapArray[TIME_UP].image = al_load_bitmap("up.png");					//boton up para aumentar el tiempo
	bitmapArray[TIME_UP].screen = SETTINGSSCREEN;
	bitmapArray[TIME_UP].height = al_get_bitmap_height(bitmapArray[SET_HOME].image);
	bitmapArray[TIME_UP].width = al_get_bitmap_width(bitmapArray[SET_HOME].image);
	bitmapArray[TIME_UP].x = DISPLAYW / 2 + 150;
	bitmapArray[TIME_UP].y = MENU_TOP_ALIGN + 380;

	bitmapArray[TIME_DOWN].image = al_load_bitmap("down.png");				//boton down para disminuir el tiempo
	bitmapArray[TIME_DOWN].screen = SETTINGSSCREEN;
	bitmapArray[TIME_DOWN].height = al_get_bitmap_height(bitmapArray[SET_HOME].image);
	bitmapArray[TIME_DOWN].width = al_get_bitmap_width(bitmapArray[SET_HOME].image);
	bitmapArray[TIME_DOWN].x = DISPLAYW / 2 + 150;
	bitmapArray[TIME_DOWN].y = MENU_TOP_ALIGN + 430;
}

void initSinglePlayerUtilities(bitmap_t * bitmapArray)
{
	bitmapArray[SP_MAP].image = al_load_bitmap("map.jpg");				//fondo
	bitmapArray[SP_MAP].screen = PLAYINGSCREEN;
	bitmapArray[SP_MAP].width = al_get_bitmap_width(bitmapArray[SP_MAP].image);
	bitmapArray[SP_MAP].height = al_get_bitmap_height(bitmapArray[SP_MAP].image);
	bitmapArray[SP_MAP].x = MAPX;
	bitmapArray[SP_MAP].y = MAPY;

	bitmapArray[SP_PLAY].image = al_load_bitmap("playicon.png");		//boton play
	bitmapArray[SP_PLAY].screen = PLAYINGSCREEN;	
	bitmapArray[SP_PLAY].width = al_get_bitmap_width(bitmapArray[SP_PLAY].image);
	bitmapArray[SP_PLAY].height = al_get_bitmap_height(bitmapArray[SP_PLAY].image);
	bitmapArray[SP_PLAY].x = LEFT_ALIGN;
	bitmapArray[SP_PLAY].y = 400;

	bitmapArray[SP_PAUSE].image = al_load_bitmap("pauseicon.png");		//boton pause
	bitmapArray[SP_PAUSE].screen = PLAYINGSCREEN;
	bitmapArray[SP_PAUSE].width = al_get_bitmap_width(bitmapArray[SP_PAUSE].image);
	bitmapArray[SP_PAUSE].height = al_get_bitmap_height(bitmapArray[SP_PAUSE].image);
	bitmapArray[SP_PAUSE].x = LEFT_ALIGN + bitmapArray[SP_PLAY].width;
	bitmapArray[SP_PAUSE].y = 400;

	bitmapArray[SP_FAST].image = al_load_bitmap("up.png");				//boton up para aumentar la velocidad de juego
	bitmapArray[SP_FAST].screen = PLAYINGSCREEN;
	bitmapArray[SP_FAST].width = al_get_bitmap_width(bitmapArray[SP_FAST].image);
	bitmapArray[SP_FAST].height = al_get_bitmap_height(bitmapArray[SP_FAST].image);
	bitmapArray[SP_FAST].x = LEFT_ALIGN + bitmapArray[SP_PLAY].width * 2;
	bitmapArray[SP_FAST].y = 380;

	bitmapArray[SP_SLOW].image = al_load_bitmap("down.png");				//boton down para disminuir la velocidad de juego
	bitmapArray[SP_SLOW].screen = PLAYINGSCREEN;
	bitmapArray[SP_SLOW].width = al_get_bitmap_width(bitmapArray[SP_SLOW].image);
	bitmapArray[SP_SLOW].height = al_get_bitmap_height(bitmapArray[SP_SLOW].image);
	bitmapArray[SP_SLOW].x = LEFT_ALIGN + bitmapArray[SP_SLOW].width * 2;
	bitmapArray[SP_SLOW].y = 420;
        
        bitmapArray[SP_HOME].image = al_load_bitmap("home.png");			//boton home
	bitmapArray[SP_HOME].screen = PLAYINGSCREEN;
	bitmapArray[SP_HOME].width = al_get_bitmap_width(bitmapArray[SP_HOME].image);
	bitmapArray[SP_HOME].height = al_get_bitmap_height(bitmapArray[SP_HOME].image);
	bitmapArray[SP_HOME].x = LEFT_ALIGN + (bitmapArray[SP_PLAY].width / 2) + 20;
	bitmapArray[SP_HOME].y = 450;

}

void initMenuUtilities(bitmap_t* bitmapArray)
{
	bitmapArray[HOME_SP].image = al_load_bitmap("sgplayer.png");			//boton de single player
	bitmapArray[HOME_SP].screen = HOMESCREEN;
	bitmapArray[HOME_SP].width = al_get_bitmap_width(bitmapArray[HOME_SP].image);
	bitmapArray[HOME_SP].height = al_get_bitmap_height(bitmapArray[HOME_SP].image);
	bitmapArray[HOME_SP].x = MENU_L_ALIGN;
	bitmapArray[HOME_SP].y = MENU_TOP_ALIGN + MENU_SPACE;

	bitmapArray[HOME_SETTINGS].image = al_load_bitmap("settings.png");		//boton settings
	bitmapArray[HOME_SETTINGS].screen = HOMESCREEN;
	bitmapArray[HOME_SETTINGS].width = al_get_bitmap_width(bitmapArray[HOME_SETTINGS].image);
	bitmapArray[HOME_SETTINGS].height = al_get_bitmap_height(bitmapArray[HOME_SETTINGS].image);
	bitmapArray[HOME_SETTINGS].x = MENU_SET_ALIGN;
	bitmapArray[HOME_SETTINGS].y = MENU_TOP_ALIGN + 4 * MENU_SPACE;

	bitmapArray[HOME_HELP].image = al_load_bitmap("help.png");				//boton help
	bitmapArray[HOME_HELP].screen = HOMESCREEN;
	bitmapArray[HOME_HELP].width = al_get_bitmap_width(bitmapArray[HOME_HELP].image);
	bitmapArray[HOME_HELP].height = al_get_bitmap_height(bitmapArray[HOME_HELP].image);
	bitmapArray[HOME_HELP].x = MENU_HELP_ALIGN;
	bitmapArray[HOME_HELP].y = MENU_TOP_ALIGN + 4 * MENU_SPACE;

	bitmapArray[HOME_CREDITS].image = al_load_bitmap("credits.png");		//boton credits
	bitmapArray[HOME_CREDITS].screen = HOMESCREEN;
	bitmapArray[HOME_CREDITS].width = al_get_bitmap_width(bitmapArray[HOME_CREDITS].image);
	bitmapArray[HOME_CREDITS].height = al_get_bitmap_height(bitmapArray[HOME_CREDITS].image);
	bitmapArray[HOME_CREDITS].x = MENU_L_ALIGN;
	bitmapArray[HOME_CREDITS].y = MENU_TOP_ALIGN + 2 * MENU_SPACE;

	bitmapArray[HOME_EXIT].image = al_load_bitmap("exit.png");				//boton exit
	bitmapArray[HOME_EXIT].screen = HOMESCREEN;
	bitmapArray[HOME_EXIT].width = al_get_bitmap_width(bitmapArray[HOME_EXIT].image);
	bitmapArray[HOME_EXIT].height = al_get_bitmap_height(bitmapArray[HOME_EXIT].image);
	bitmapArray[HOME_EXIT].x = MENU_EXIT_ALIGN_X;
	bitmapArray[HOME_EXIT].y = MENU_EXIT_ALIGN_Y;
}

void initWorld(unsigned int rows, unsigned int columns, display_t* display)
{
	display->worldData = (worldData_t*) malloc(sizeof(worldData_t));

	display->worldData->rows = rows;
	display->worldData->columns = columns;
	display->worldData->width = (double) display->buttons[SP_MAP].width; 				//ancho del mundo
	display->worldData->height = (double) display->buttons[SP_MAP].height; 				//largo del mundo
	display->worldData->tileWidth = ((double) display->buttons[SP_MAP].width) / (double) columns;
	display->worldData->tileHeight = ((double) display->buttons[SP_MAP].height) / (double) rows;
	display->worldData->initialX = (double) display->buttons[SP_MAP].x;
	display->worldData->initialY = (double) display->buttons[SP_MAP].y;
	display->worldData->initialCellX = display->worldData->initialX + (display->worldData->tileWidth) / 2; 		//apunto a donde iria la primera celula
	display->worldData->initialCellY = display->worldData->initialY + (display->worldData->tileHeight) / 2;
}

void updateGraphics(cell_t** cellMatrix, display_t* displayData, unsigned score, unsigned generation, unsigned inputCells)
{
	singlePlayerDisplay(displayData); 					//pantalla "vanilla"
	drawWorld(displayData->worldData); 					//dibujo celdas
	drawCells(cellMatrix, displayData->worldData); 		//dibujo celulas vivas
	updateScore(displayData, score);					//escribimos numero de score y generation, como la cantidad de celulas remanentes
	updateGeneration(displayData, generation);
	updateInputCells(displayData, inputCells);
	al_set_target_backbuffer(displayData->display);
	al_flip_display();
}

void drawWorld(worldData_t* worldData)
{
	double x1 = worldData->initialX; 								//punto 1 - inicio del mundo
	double y1 = worldData->initialY;
	double x2 = worldData->initialX + worldData->width; 			//punto 2 - fin del mundo
	double y2 = worldData->initialY + worldData->height;

	for (; x1 <= x2; x1 += worldData->tileWidth) 					//dibujo lineas verticales
		al_draw_line(x1, y1, x1, y2, al_map_rgb(0, 0, 0), 1);

	for (x1 = worldData->initialX; y1 <= y2; y1 += worldData->tileHeight) //dibujo lineas horizontales
		al_draw_line(x1, y1, x2, y1, al_map_rgb(0, 0, 0), 1);
}

void drawCells(cell_t** cellMatrix, worldData_t* worldData)
{
	double x, y;
	double r = 0.45 * (worldData->tileWidth < worldData->tileHeight ? worldData->tileWidth : worldData->tileHeight); //radio de los circulos (celulas)
	unsigned int i, j;

	for (i = 1; i <= worldData->rows; i++) 				//recorro la matriz de celulas e imprimo las vivas
	{
		for (j = 1; j <= worldData->columns; j++)
		{
			if (cellMatrix[i][j].state == ALIVE)
			{
				x = worldData->initialCellX + (double) (j - 1) * worldData->tileWidth; 	//de la primer celula, me corro
				y = worldData->initialCellY + (double) (i - 1) * worldData->tileHeight; //las filas y columnas que hagan falta
				al_draw_filled_circle(x, y, r, al_map_rgb(200, 20, 0)); //dibujo celula
			}
		}
	}
}

void updateScore(display_t* displayData, unsigned score)
{
	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), MAPX / 2, 140, ALLEGRO_ALIGN_CENTER, "%d", score);
}

void updateGeneration(display_t* displayData, unsigned generation)
{
	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), MAPX / 2, 240, ALLEGRO_ALIGN_CENTER, "%d", generation);
}

void updateInputCells(display_t* displayData, unsigned inputCells)
{
	al_draw_textf(displayData->font, al_map_rgb(0, 0, 0), MAPX / 2, 580, ALLEGRO_ALIGN_CENTER, "%d", inputCells);
}

void gameOverDisplay(display_t* displayData)
{
	al_draw_filled_rectangle(DISPLAYW / 2 - 280, DISPLAYH / 2 - 180, DISPLAYW / 2 + 280, DISPLAYH / 2 + 140, al_map_rgb(233, 219, 123));
	al_draw_rectangle(DISPLAYW / 2 - 280, DISPLAYH / 2 - 180, DISPLAYW / 2 + 280, DISPLAYH / 2 + 140, al_map_rgb(97, 86, 33), 6);

	al_draw_text(displayData->titleFont, al_map_rgb(0, 0, 0), DISPLAYW / 2, DISPLAYH / 2 - 155, ALLEGRO_ALIGN_CENTRE, "EXPERIMENT");
	al_draw_text(displayData->titleFont, al_map_rgb(0, 0, 0), DISPLAYW / 2, DISPLAYH / 2 - 70, ALLEGRO_ALIGN_CENTRE, "FINISHED");
	al_flip_display();
}



