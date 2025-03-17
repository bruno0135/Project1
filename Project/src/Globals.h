#pragma once
//Displaying scale factor
#define GAME_SCALE_FACTOR	1.0f

//Function return values
enum class AppStatus { OK = 0, ERROR, QUIT };
enum class Directions { NONE = -1, RIGHT, LEFT, UP, DOWN };

//Main return values
#define EXIT_SUCCESS		0
#define EXIT_FAILURE		1

//Window size
#define WINDOW_WIDTH		464
#define WINDOW_HEIGHT		540

//Game constants
#define TILE_SIZE			8
#define MENU_ANIM_SIZE		32
#define LETTERS_SIZE		16
#define LEVEL_WIDTH			68
#define LEVEL_HEIGHT		62
#define EXTRA_DISPLAÇMENT	15
#define MARGIN_GUI_X		(WINDOW_WIDTH/2)-(LEVEL_WIDTH*TILE_SIZE)/2
#define MARGIN_GUI_Y		(WINDOW_HEIGHT/2)-(LEVEL_HEIGHT*TILE_SIZE)/2 + EXTRA_DISPLAÇMENT
#define TIME_IN_SCATTER		10
#define TIME_IN_CHASE		10
#define TIME_IN_FRIGHTENED	8
#define TIME_TO_BLINK		5
#define TIME_PUNT_SHOWED	1
#define SPAWN_POS_Y			184
#define RIGHT_SPAWN_POS_X	280
#define LEFT_SPAWN_POS_X	264
#define PUNTUATION_SIZE		48
#define BLINKY_PERCENT_TO_PERMANENTLY_CHASE		80
#define TIME_TO_START		2
#define TIME_WIN			3
#define PERCENT_TO_FRUIT	70
#define DISTANCE_COLLISION	16

//Teleports
#define RIGHT_TP_POS_X		496
#define RIGHT_TP_POS_Y		216
#define LEFT_TP_POS_X		16	
#define LEFT_TP_POS_Y		216

//Navigation Mesh
#define PATH_SIZE			16
#define MAX_ITERATIONS		200

//Entities animation delay
#define ANIM_DELAY			4

