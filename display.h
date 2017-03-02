#ifndef DISPLAY_H
#define DISPLAY_H

#include "cell.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/timer.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/allegro_image.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

enum buttons {
    HOME_SP, HOME_CREDITS, HOME_HELP, HOME_EXIT, HOME_SETTINGS,
    SP_HOME, SP_MAP, SP_PLAY, SP_PAUSE, SP_FAST, SP_SLOW,
    R1_UP, R1_DOWN, R2_UP, R2_DOWN, R3_UP, R3_DOWN, R4C_UP, R4C_DOWN, R4R_UP, R4R_DOWN, R5_UP, R5_DOWN,
    SET_HOME, TIME_UP, TIME_DOWN,
    CREDITS_HOME, HELP_HOME,
    BUTTON_COUNT, NOBUTTON
};


#define ERROR -1
#define LEFT_ALIGN 15
#define MENU_L_ALIGN 445
#define MENU_TOP_ALIGN 200
#define MENU_SPACE 90
#define MENU_HELP_ALIGN 465
#define MENU_SET_ALIGN 585
#define MENU_EXIT_ALIGN_X 895
#define MENU_EXIT_ALIGN_Y 600
#define DISPLAYW 1090
#define DISPLAYH 720
#define MAPX 170
#define MAPY 60
#define BUTTON_SIZE 40,40
#define BUTTONW 40
#define SPACE 10
#define HOME_X_MULT 475+((BUTTONW+SPACE)/2)
#define HOME_Y_MULT 255
#define SETTINGS_X_MULT 475+((BUTTONW+SPACE)*3/2)
#define SETTINGS_Y_MULT 257
#define PLAY_X_MULT 475
#define PLAY_Y_MULT 200
#define PAUSE_X_MULT 475+BUTTONW+SPACE
#define PAUSE_Y_MULT 200

typedef struct worldData        //informacion para dibujar la matriz
{
    unsigned int rows;
    unsigned int columns;
    double initialX;            //x e y donde comenzara la matriz
    double initialY;
    double width;
    double height;
    double tileWidth;
    double tileHeight;
    double initialCellX;        //x e y de la celula
    double initialCellY;
} worldData_t;

typedef struct bitmap           //informacion de cada boton (bitmap)
{
    ALLEGRO_BITMAP* image;      //el archivo
    unsigned screen;            //a que pantalla pertenece
    unsigned int x;             //posicion en la pantalla
    unsigned int y;
    unsigned int width;         //ancho y largo de la misma
    unsigned int height;
} bitmap_t;

typedef struct display          //informacion del display
{
    ALLEGRO_DISPLAY* display;
    bitmap_t* buttons;
    ALLEGRO_EVENT_QUEUE* eventQueue;
    ALLEGRO_TIMER* timer;
    unsigned resumeTimerCount;
    unsigned timeLapseCount;
    unsigned gameTimeCount;
    ALLEGRO_FONT* font;
    ALLEGRO_FONT* titleFont;
    ALLEGRO_BITMAP* background;
    unsigned int backgroundWidth;
    unsigned int backgroundHeight;
    unsigned int width;
    unsigned int height;
    worldData_t* worldData;     //informacion grafica del mundo
} display_t;



//PROTOTIPOS

//Devuelve puntero a display_t si se pudo inicializar bien, y NULL si no se pudo inicializar
display_t* displayInit(void);

//desinicializa el display
void displayDeInit(display_t* display);

//dibuja la pantalla de juego
void updateGraphics(cell_t** cellMatrix, display_t* display, unsigned score, unsigned generation, unsigned inputCellsLeft);

//dibuja pantalla de single player
void singlePlayerDisplay(display_t* displayData);

//dibuja menu
void menuDisplay(display_t * displayData);

//Dibuja pantalla de configuracion
void settingsDisplay(display_t* displayData, int sofocation, int isolation, int revival, int rowR4, int columnR4, int aliveCells, int gametime);

//dibuja pantalla de ayuda
void helpDisplay(display_t* displayData);

//dibuja creditos
void creditsDisplay(display_t* displayData);

//dibuja cuadro de game over ("experiment finished")
void gameOverDisplay(display_t* displayData);

//dibuja las celulas vivas
void drawCells(cell_t** cellMatrix, worldData_t* worldData);

//inicializa worldData
void initWorld(unsigned int rows, unsigned int columns, display_t* display);

//dibuja mundo
void drawWorld(worldData_t* worldData);


#endif /* DISPLAY_H */

